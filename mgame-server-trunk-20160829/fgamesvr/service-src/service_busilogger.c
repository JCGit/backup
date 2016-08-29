#include "skynet.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

struct busilogger {
	FILE * handle;
	int close;
	time_t log_create_time;
	char log_dir[256];
	char log_prefix[256];
};

static int is_sameday(time_t t1, time_t  t2)
{
   return (t1 + 8 * 3600) / 86400 == (t2 + 8 * 3600) / 86400 ? 1 : 0;
}

int _update_busilog_name(struct busilogger * inst);

struct busilogger * busilogger_create(void) {
	struct busilogger * inst = skynet_malloc(sizeof(*inst));
	inst->handle = NULL;
	inst->close = 0;
	inst->log_create_time = 0;
	return inst;
}

void busilogger_release(struct busilogger * inst) {
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
	
	struct busilogger * inst = ud;
	_update_busilog_name(inst);
//	fprintf(inst->handle, "[%s-%ldms : %08x] ",timebuf,tv.tv_usec/1000,source);
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

int open_busilog_file(struct busilogger * inst)
{
	char timebuf[64];
	char filename[512];
	struct tm tm;
	time_t now = time(NULL);
	localtime_r(&now, &tm);
	strftime(timebuf, sizeof(timebuf), "%Y%m%d", &tm);

	_create_log_dir(inst->log_dir);
	snprintf(filename, sizeof(filename), "%s/%s.log.%s", inst->log_dir, inst->log_prefix, timebuf);
	inst->handle = fopen(filename, "a+");
	if (inst->handle == NULL)
	{
		int saved_errno = errno;
		fprintf(stderr, "open file error: %d\n", saved_errno);
		fprintf(stderr, filename);
		inst->handle = stdout;
	}
	return 0;
}

int _update_busilog_name(struct busilogger * inst)
{
	time_t now = time(NULL);

	if(NULL == inst->handle || is_sameday(now, inst->log_create_time) == 0)
	{
		inst->log_create_time = now;
	}
	else
	{
		return 0;
	}

	if(inst->handle != NULL)
	{
		fflush(inst->handle);
		fclose(inst->handle);
	}

	open_busilog_file(inst);
	return 0;
}

int
busilogger_init(struct busilogger * inst, struct skynet_context *ctx, const char * parm) {
	if (parm == NULL)
		return 1;
	char service_name[128];
	// 输入服务名、日志文件目录、日志文件名前缀
	int n = sscanf(parm, "%s %s %s",(char*)&service_name,(char*)&inst->log_dir,(char*)&inst->log_prefix);
	if (n < 3) return 1;

	if (-1 == _update_busilog_name(inst)) {
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
