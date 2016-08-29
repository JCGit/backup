#include "skynet.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

struct logger {
    FILE * handle;
    int close;
    char log_path[64];
    unsigned short day;
};

int _update_logname(struct logger * inst, time_t now);

struct logger *
logger_create(void) {
    struct logger * inst = skynet_malloc(sizeof(*inst));
    inst->handle = NULL;
    inst->close = 0;
    return inst;
}

void
logger_release(struct logger * inst) {
    if (inst->close) {
        fclose(inst->handle);
    }
    skynet_free(inst);
}

static int
_logger(struct skynet_context * context, void *ud, int type, int session, uint32_t source, const void * msg, size_t sz) {
    
    time_t now = time(NULL);
    struct logger * inst = ud;

    if (inst->log_path[0] != 0)
    {
       _update_logname(inst, now);
    }
    fprintf(inst->handle, "[:%08x] ", source);
    fwrite(msg, sz , 1, inst->handle);
    fprintf(inst->handle, "\n");
    fflush(inst->handle);   

    return 0;
}

int open_log(struct logger *inst, time_t now){
    struct tm tm;
    localtime_r (&now, &tm);

    char filename[512];
    snprintf (filename, 512 -1, "%s-%04d%02d%02d.log",
					inst->log_path,
					tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday
    );

    fprintf(stderr, "filename:  %s \n", filename);

    inst->handle = fopen(filename, "w");
    inst->close = 1;
    if (inst->handle == NULL)
    {
        int saved_errno = errno;
        fprintf(stderr, "open file error: %d\n", saved_errno);
        fprintf(stderr, filename);
        inst->handle = stdout;
        inst->close = 0;
        return 1;   
    }

   inst->day = tm.tm_yday;
   return 0;
}

int _update_logname(struct logger * inst, time_t now)
{
    struct tm tm;	
    localtime_r (&now, &tm);

    if (inst->day != tm.tm_yday)
    {
      if (open_log(inst, now) == 1)
      {
         return 1;
      }
    }
    return 0;
}

int
logger_init(struct logger * inst, struct skynet_context *ctx, const char * parm) {
    memset(inst->log_path, 0, sizeof(inst->log_path));
    if (parm){
        time_t now = time(NULL);
        strcpy(inst->log_path, parm);
        if (_update_logname(inst, now) == 1) {
        	 return 1;
        }
    }
    else
    {
        inst->handle = stdout;
    }
    if (inst->handle) {
        skynet_callback(ctx, inst, _logger);
        skynet_command(ctx, "REG", ".logger");
        return 0;
    }
    return 1;
}
