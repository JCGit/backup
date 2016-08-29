#include "skynet.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

// 日志文件大小限制
#define LOG_MAX_SIZE 1024*1024
#define ONE_DAY_SECONDS (24*60*60)
#define LOG_MAX_INDEX 100

struct mylogger {
	FILE * handle;
	int close;
	int writen_bytes;
	int index;
	time_t log_create_time;
	char log_dir[256];
	char log_prefix[256];
};

static int is_sameday(time_t t1, time_t  t2)
{
   // 中国是东八区，比格林威治时间早8小时
   t1 = ( t1 + 8*3600 ) / 86400;
   t2 = ( t2 + 8*3600 ) / 86400;
   if  (t1 == t2) return 1;
   return 0;
}

int _update_log_name(struct mylogger * inst);

struct mylogger *
mylogger_create(void) {
	struct mylogger * inst = skynet_malloc(sizeof(*inst));
	inst->handle = NULL;
	inst->close = 0;
	inst->index = 1;
	inst->writen_bytes = 0;
	return inst;
}

void
mylogger_release(struct mylogger * inst) {
	if (inst->close) {
		fclose(inst->handle);
	}
	skynet_free(inst);
}

static int
_logger(struct skynet_context * context, void *ud, int type, int session, uint32_t source, const void * msg, size_t sz) {
	char timebuf[64];
	struct tm tm;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	localtime_r(&tv.tv_sec, &tm);
	strftime(timebuf, sizeof(timebuf), "%T", &tm);
	
	struct mylogger * inst = ud;
	_update_log_name(inst);
	fprintf(inst->handle, "[%s-%ldms : %08x] ",timebuf,tv.tv_usec/1000,source);
	fwrite(msg, sz , 1, inst->handle);
	fprintf(inst->handle, "\n");
	fflush(inst->handle);

	return 0;
}

int _create_log_dir(char* dir_name)
{
	if (access(dir_name,F_OK)!=0) {
		int saved_errno = errno;
		if (ENOENT == saved_errno)
		{
			if (mkdir(dir_name, 0755) == -1)
			{
				saved_errno = errno;
				fprintf(stderr, "mkdir error: %d\n", saved_errno);
				return -1;
			}
		}
		else
		{
			fprintf(stderr, "opendir error: %d\n", saved_errno);
			return -1;
		}
	}
	
	return 0;
}

int open_log_file(struct mylogger * inst,int index)
{
	char timebuf[64];
	char filename[512];
	struct tm tm;
	time_t now = time(NULL);
	localtime_r(&now, &tm);
	strftime(timebuf, sizeof(timebuf), "%Y%m%d", &tm);

	_create_log_dir(inst->log_dir);
		
	while (index<LOG_MAX_INDEX)
	{
		snprintf(filename, sizeof(filename), "%s/%s.%s-%d.log", inst->log_dir, inst->log_prefix, timebuf, index++);
		if((access(filename,F_OK))==0){
			continue;
		}
		inst->handle = fopen(filename, "a+");
		if (inst->handle == NULL)
		{
			int saved_errno = errno;
			fprintf(stderr, "open file error: %d\n", saved_errno);
			fprintf(stderr, filename);
			inst->handle = stdout;
		}
		break;
	}
	
	return 0;
}

int _update_log_name(struct mylogger * inst)
{
	int new_index = 1;
	int need_create = 1;

	time_t now = time(NULL);
	struct tm* now_tm = localtime(&now);
	time_t now_local = mktime(now_tm);
	
	// 首次打开文件
	if(NULL==inst->handle) {
		inst->log_create_time = now_local;
	}
	// 日期不同了
	else if(is_sameday(now_local, inst->log_create_time) == 0) {
		new_index = 1;
		inst->index = 1;
		inst->log_create_time = now_local;
	}
	// 写的量超过了上限
	else if(LOG_MAX_SIZE<inst->writen_bytes && inst->index < LOG_MAX_INDEX) {
		inst->index += 1;
		new_index = inst->index;
		inst->writen_bytes = 0;
		inst->log_create_time = now_local;
	}
	else{
		need_create = 0;
	}
	
	if(0==need_create){
		return 0;
	}
		
	if(inst->handle != NULL)
	{
		fflush(inst->handle);
		fclose(inst->handle);
	}
	
	open_log_file(inst,new_index);
	
	return 0;
}

int
mylogger_init(struct mylogger * inst, struct skynet_context *ctx, const char * parm) {
	if (parm == NULL)
		return 1;
	char service_name[128];
	// 输入服务名、日志文件目录、日志文件名前缀
	int n = sscanf(parm, "%s %s %s",(char*)&service_name,(char*)&inst->log_dir,(char*)&inst->log_prefix);
	if (n<3) return 1;
		
	if (-1==_update_log_name(inst)) {
		inst->close = 1;
	}
	if (inst->handle) {
		skynet_callback(ctx, inst, _logger);
		// 注册服务名
		skynet_command(ctx, "REG", service_name);
		return 0;
	}
	return 1;
}
