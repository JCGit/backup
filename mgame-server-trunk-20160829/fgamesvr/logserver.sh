#!/bin/sh

#参数1： 命令
# start[默认] 启动
# stop 停止
CMD=$1

#参数2： 类型
# release[默认] 后台启动
# debug 前台启动
MODE=$2

. /etc/init.d/functions

CUR_PATH=$(dirname $(readlink -f $0))
PID_FILE=$CUR_PATH/logserver.pid

if [[ $CMD == 'start' ]] || [[ -z  $CMD ]]; then
    if [ -z "$3" ]; then
        CONFIG=$CUR_PATH/conf/config.logserver
    else
        CONFIG=$CUR_PATH/$3
    fi

    ulimit -c unlimited
    sh sh/log_name.sh logserver

    if [[ -z $MODE  ]] || [[ $MODE == "release" ]]; then
        echo -n $"Starting logserver: "
        $CUR_PATH/../skynet/skynet $CONFIG
        if [ $? -eq 0 ]; then
            success && echo
        else
            failure && echo
        fi
    elif [ $MODE == "debug" ]
    then
        sed -e 's/logger/--logger/' -e 's/daemon/--daemon/' $CONFIG > $CUR_PATH/conf/debug_config.logserver
        $CUR_PATH/../skynet/skynet $CUR_PATH/conf/debug_config.logserver
    fi
elif [ $CMD == 'stop' ]; then
    if [ ! -f $PID_FILE ] ;then
      echo "have no logserver"
      exit 0
    fi

    pid=`cat $PID_FILE`
    exist_pid=`pgrep skynet | grep $pid`
    if [ -z "$exist_pid" ] ;then
      echo "have no logserver"
      exit 0
    else
      echo -n $"$pid logserver will killed"
      killproc -p $PID_FILE
      echo
    fi
fi




