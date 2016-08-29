#include <lua.h>
#include <lauxlib.h>

#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <dirent.h>
#include <pthread.h>
#include <assert.h>

#define ONE_MB	(1024*1024)
#define DEFAULT_ROLL_SIZE (1024*ONE_MB)		// ��־�ļ��ﵽ1G������һ�����ļ�
#define DEFAULT_BASENAME "default"
#define DEFAULT_DIRNAME "."
#define DEFAULT_INTERVAL 5					// ��־ͬ�������̼��ʱ��

#define LOG_MAX 4*1024					// ����LOG�4K
#define LOG_BUFFER_SIZE 4*1024*1024			// һ��LOG������4M

enum logger_level
{
	DEBUG = 0,
	INFO = 1,
	WARNING = 2,
	ERROR = 3,
	FATAL = 4
};

struct buffer
{
	struct buffer* next;
	char data[LOG_BUFFER_SIZE];
	int size;					// ��������ʹ���ֽ���
};

struct buffer_list
{
	struct buffer* head;
	struct buffer* tail;
	int size;					// ����������
};

struct logger
{
	FILE* handle;
	int loglevel;
	int rollsize;
	char basename[64];
	char dirname[64];
	size_t written_bytes;	// ��д���ļ����ֽ���
	int roll_mday;			// ͬstruct tm�е�tm_mday
	int flush_interval;		// �첽��־���д���ļ�ʱ����

	struct buffer* curr_buffer;		// ��ǰ������
	struct buffer* next_buffer;		// ���û�����
	struct buffer_list buffers;		// ��д���ļ��Ļ������б�

	int running;
	pthread_t thread;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} inst;

char* get_log_filename(const char* basename, int index)
{
	static char filename[128];			// ֻ��һ���̷߳��ʣ����õ����̰߳�ȫ����
	memset(filename, 0, sizeof(filename));
	char timebuf[64];
	struct tm tm;
	time_t now = time(NULL);
	localtime_r(&now, &tm);
	strftime(timebuf, sizeof(timebuf), ".%Y%m%d", &tm);
	snprintf(filename, sizeof(filename), "%s%s-%d.log", basename, timebuf, index);
	return filename;
}

/*
size_t get_filesize(const char* path)
{
	off_t size = 0;
	struct stat statbuf;
	if (stat(path, &statbuf) == 0)
		size = statbuf.st_size;
	else
	{
		int saved_errno = errno;
		fprintf(stderr, "stat error: %s\n", strerror(saved_errno));
		exit(EXIT_FAILURE)
	}
	return size;
}*/

void rollfile()
{
	if (inst.handle == stdin || inst.handle == stdout || inst.handle == stderr)
		return;

	if (inst.handle != NULL && inst.written_bytes > 0)
	{
		fflush(inst.handle);
		fclose(inst.handle);
	}

	char filename[128];
	// ��������ڣ������ļ���
	DIR* dir;
	dir = opendir(inst.dirname);
	if (dir == NULL)
	{
		int saved_errno = errno;
		if (saved_errno == ENOENT)
		{
			if (mkdir(inst.dirname, 0755) == -1)
			{
				saved_errno = errno;
				fprintf(stderr, "mkdir error: %s\n", strerror(saved_errno));
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			fprintf(stderr, "opendir error: %s\n", strerror(saved_errno));
			exit(EXIT_FAILURE);
		}
	}
	else
		closedir(dir);
	int index = 0;
	while (1)
	{
		snprintf(filename, sizeof(filename), "%s/%s", inst.dirname, get_log_filename(inst.basename, index++));
		inst.handle = fopen(filename, "a+");
		if (inst.handle == NULL)
		{
			int saved_errno = errno;
			fprintf(stderr, "open file error: %s\n", strerror(saved_errno));
			inst.handle = stdout;
			break;
		}
		else
		{
			if (inst.written_bytes >= inst.rollsize)
			{
				// ������־�ļ�
				fclose(inst.handle);
				continue;
			}

			struct tm tm;
			time_t now = time(NULL);
			localtime_r(&now, &tm);
			inst.roll_mday = tm.tm_mday;
			break;
		}
	}
}

void append(const char* logline, size_t len)
{
	pthread_mutex_lock(&inst.mutex);
	if (inst.curr_buffer->size + len < LOG_BUFFER_SIZE)
	{
		// ��ǰ������δ����������׷�ӵ�ĩβ
		memcpy(inst.curr_buffer->data + inst.curr_buffer->size, logline, len);
		inst.curr_buffer->size += len;
	}
	else
	{
		// ��ǰ����������������ǰ��������ӵ���д���ļ��������б�
		if (!inst.buffers.head)
		{
			assert(inst.buffers.tail == NULL);
			inst.buffers.head = inst.curr_buffer;
			inst.buffers.tail = inst.curr_buffer;
		}
		else
		{
			inst.buffers.tail->next = inst.curr_buffer;
			inst.buffers.tail = inst.curr_buffer;
		}
		inst.buffers.size++;
		assert(inst.buffers.tail->next == NULL);

		// ��Ԥ������������Ϊ��ǰ������
		if (inst.next_buffer)
		{
			inst.curr_buffer = inst.next_buffer;
			inst.next_buffer = NULL;
		}
		else
		{
			// ������������ٷ�����ǰ��д���ٶ�̫�죬һ���Ӱ����黺������д�꣬
			// ��ô��ֻ�÷���һ���µĻ�������
			inst.curr_buffer = (struct buffer*)calloc(1, sizeof(struct buffer));
		}
		memcpy(inst.curr_buffer->data + inst.curr_buffer->size, logline, len);
		inst.curr_buffer->size += len;

		pthread_cond_signal(&inst.cond);	// ֪ͨ��˿�ʼд����־
	}

	pthread_mutex_unlock(&inst.mutex);
}

// ��־�̴߳�����
static inline void* worker_func(void* p)
{
	rollfile();
	struct timespec ts;
	struct buffer_list buffers_to_write;
	memset(&buffers_to_write, 0, sizeof(buffers_to_write));
	// ׼��������л�����
	struct buffer* new_buffer1 = (struct buffer*)calloc(1, sizeof(struct buffer));
	struct buffer* new_buffer2 = (struct buffer*)calloc(1, sizeof(struct buffer));
	while (inst.running)
	{
		assert(buffers_to_write.head == NULL);
		assert(new_buffer1->size == 0);
		assert(new_buffer2->size == 0);

		pthread_mutex_lock(&inst.mutex);
		if (inst.buffers.head == NULL)
		{
			clock_gettime(CLOCK_REALTIME, &ts);
	    	ts.tv_sec += inst.flush_interval;
			pthread_cond_timedwait(&inst.cond, &inst.mutex, &ts);
		}

		struct tm tm;
		time_t now = time(NULL);
		localtime_r(&now, &tm);
		// �µ�һ�죬������־
		if (inst.roll_mday != tm.tm_mday)
			rollfile();

		// ����ǰ����������buffers
		if (!inst.buffers.head)
		{
			inst.buffers.head = inst.curr_buffer;
			inst.buffers.tail = inst.curr_buffer;
		}
		else
			inst.buffers.tail = inst.curr_buffer;
		inst.buffers.size += 1;

		inst.curr_buffer = new_buffer1;				// �����е�newBuffer1��Ϊ��ǰ������
		new_buffer1 = NULL;

		// buffers��buffers_to_write������
		// ��������Ĵ���������ٽ���֮�ⰲȫ�ط���buffers_to_write
		buffers_to_write.head = inst.buffers.head;
		buffers_to_write.tail = inst.buffers.tail;
		buffers_to_write.size = inst.buffers.size;
		inst.buffers.head = 0;
		inst.buffers.tail = 0;
		inst.buffers.size = 0;

		if (!inst.next_buffer)
		{
			// ȷ��ǰ��ʼ����һ��Ԥ��buffer�ɹ����䣬
			// ����ǰ���ٽ��������ڴ�ĸ��ʣ�����ǰ���ٽ������ȡ�
			inst.next_buffer = new_buffer2;
			new_buffer2 = NULL;
		}

		pthread_mutex_unlock(&inst.mutex);

		assert(buffers_to_write.size > 0);

		if (buffers_to_write.size > 25)
		{
			char timebuf[64];
			struct tm tm;
			time_t now = time(NULL);
			localtime_r(&now, &tm);
			strftime(timebuf, sizeof(timebuf), "%Y%m%d-%H%M%S", &tm);

			char buf[256];
			snprintf(buf, sizeof(buf), "Dropped log messages at %s, %d larger buffers\n",
				timebuf, buffers_to_write.size-2);
			fprintf(stderr, "%s", buf);
			append(buf, strlen(buf));

			// ����������־�����ڳ��ڴ棬���������黺����
			struct buffer* new_tail = buffers_to_write.head->next;
			struct buffer* node = new_tail->next;
			while (node != NULL)
			{
				struct buffer* p = node;
				node = node->next;
				free(p);
			}
			buffers_to_write.tail = new_tail;
			buffers_to_write.tail->next = NULL;
			buffers_to_write.size = 2;
		}
		struct buffer* node;
		for (node = buffers_to_write.head; node != NULL; node = node->next)
		{
			if (inst.handle)
			{
				fwrite_unlocked(node->data, 1, node->size, inst.handle);
				inst.written_bytes += node->size;
			}
		}

		fflush(inst.handle);
		if (inst.written_bytes > inst.rollsize)
		{
			rollfile();
			inst.written_bytes = 0;
		}

		if (!new_buffer1)
		{
			assert(buffers_to_write.size > 0);
			new_buffer1 = buffers_to_write.head;
			buffers_to_write.head = buffers_to_write.head->next;
			memset(new_buffer1, 0, sizeof(struct buffer));
			buffers_to_write.size -= 1;
		}

		if (!new_buffer2)
		{
			assert(buffers_to_write.size > 0);
			new_buffer2 = buffers_to_write.head->next;
			buffers_to_write.head = buffers_to_write.head->next;
			memset(new_buffer2, 0, sizeof(struct buffer));
			buffers_to_write.size -= 1;
		}

		// ���buffers_to_write
		node = buffers_to_write.head;
		while (node != NULL)
		{
			struct buffer* p = node;
			node = node->next;
			free(p);
		}
		buffers_to_write.head = 0;
		buffers_to_write.tail = 0;
		buffers_to_write.size = 0;
		fflush(inst.handle);

	}
	fflush(inst.handle);

	return NULL;
}

static void log_exit()
{
	inst.running = 0;
	pthread_join(inst.thread, NULL);
	pthread_mutex_destroy(&inst.mutex);
	pthread_cond_destroy(&inst.cond);
	if (inst.handle)
		fclose(inst.handle);
}

int linit(lua_State *L)
{
	memset(&inst, 0, sizeof(inst));
	inst.loglevel = lua_tointeger(L, 1);
	inst.rollsize = lua_tointeger(L, 2);
	inst.flush_interval = lua_tointeger(L, 3);
	inst.rollsize = inst.rollsize > 0 ? inst.rollsize * ONE_MB : DEFAULT_ROLL_SIZE;
	inst.flush_interval = inst.flush_interval > 0 ? inst.flush_interval : DEFAULT_INTERVAL;
	inst.curr_buffer = (struct buffer*)calloc(1, sizeof(struct buffer));
	inst.next_buffer = (struct buffer*)calloc(1, sizeof(struct buffer));

	const char *dirname = lua_tolstring(L, 4, NULL);
	if (dirname == NULL)
		strncpy(inst.dirname, DEFAULT_DIRNAME, sizeof(inst.dirname));
	else
		strncpy(inst.dirname, dirname, sizeof(inst.dirname));

	const char *basename = lua_tolstring(L, 5, NULL);
	if (basename == NULL)
		strncpy(inst.basename, DEFAULT_BASENAME, sizeof(inst.basename));
	else
		strncpy(inst.basename, basename, sizeof(inst.basename));

	if (pthread_mutex_init(&inst.mutex, NULL) != 0)
	{
		int saved_errno = errno;
		fprintf(stderr, "pthread_mutex_init error: %s\n", strerror(saved_errno));
		exit(EXIT_FAILURE);
	}
	if (pthread_cond_init(&inst.cond, NULL) != 0)
	{
		int saved_errno = errno;
		fprintf(stderr, "pthread_cond_init error: %s\n", strerror(saved_errno));
		exit(EXIT_FAILURE);
	}

	inst.running = 1;
	if (pthread_create(&inst.thread, NULL, worker_func, NULL) != 0)
	{
		int saved_errno = errno;
		fprintf(stderr, "pthread_create error: %s\n", strerror(saved_errno));
		exit(EXIT_FAILURE);
	}

	return 0;
}

int lexit(lua_State *L)
{
	log_exit();
	return 0;
}

int ldebug(lua_State *L)
{
	if (inst.loglevel <= DEBUG)
	{
		char* data = (char*)lua_tolstring(L, 1, NULL);
		if (data == NULL)
		{
			return 0;
		}
		char msg[LOG_MAX];
		snprintf(msg, sizeof(msg), "[DEBUG] %s\n", data);
		append(msg, strlen(msg));
	}
	return 0;
}

int linfo(lua_State *L)
{
	if(inst.loglevel <= INFO)
	{
		char* data = (char*)lua_tolstring(L, 1, NULL);
		if (data == NULL)
		{
			return 0;
		}
		char msg[LOG_MAX];
		snprintf(msg, sizeof(msg), "[INFO] %s\n", data);
		append(msg, strlen(msg));
	}
	return 0;
}

int lwarning(lua_State *L)
{
	char* data = (char*)lua_tolstring(L, 1, NULL);
	if (data == NULL)
	{
		return 0;
	}
	char msg[LOG_MAX];
	snprintf(msg, sizeof(msg), "[WARNING] %s\n", data);
	append(msg, strlen(msg));
	return 0;
}

int lerror(lua_State *L)
{
	char* data = (char*)lua_tolstring(L, 1, NULL);
	if (data == NULL)
	{
		return 0;
	}
	char msg[LOG_MAX];
	snprintf(msg, sizeof(msg), "[ERROR] %s\n", data);
	append(msg, strlen(msg));
	return 0;
}

int lfatal(lua_State *L)
{
	char* data = (char*)lua_tolstring(L, 1, NULL);
	if (data == NULL)
	{
		return 0;
	}
	char msg[LOG_MAX];
	snprintf(msg, sizeof(msg), "[FATAL] %s\n", data);
	append(msg, strlen(msg));
	log_exit();
	abort();
	return 0;
}

int luaopen_log_core(lua_State *L)
{
	luaL_checkversion(L);
	luaL_Reg l[] =
	{
		{ "init", linit },
		{ "exit", lexit },
		{ "debug", ldebug },
		{ "info", linfo },
		{ "warning", lwarning },
		{ "error", lerror },
		{ "fatal", lfatal },
		{ NULL, NULL },
	};
	luaL_newlib(L, l);

	return 1;
}
