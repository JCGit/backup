#!/bin/sh

. /etc/init.d/functions

#参数1： 命令 start 启动 stop 停止
CMD=$1
#参数2： 类型 release[默认] 后台启动  debug 前台启动
MODE=$2

CUR_PATH=$(dirname $(readlink -f $0))
PID_FILE=$CUR_PATH/gamed.pid

if [[ $CMD == "start" ]] || [[ ! -n  $CMD ]]; then
    if [ -z "$3" ]; then
        CONFIG=$CUR_PATH/conf/config
    else
        CONFIG=$CUR_PATH/$3
    fi

    ulimit -c unlimited
    sh sh/log_name.sh gamed

    if [[ ! -n $MODE  ]] || [[ $MODE == "release" ]]; then
        echo -n $"Starting gameserver: "
        $CUR_PATH/../skynet/skynet $CONFIG
        if [ $? -eq 0 ]; then
            success && echo
        else
            failure && echo
        fi
    elif [[ $MODE == "debug" ]]
    then
        sed -e 's/logger/--logger/' -e 's/daemon/--daemon/' $CONFIG > $CUR_PATH/conf/debug_config
        $CUR_PATH/../skynet/skynet $CUR_PATH/conf/debug_config
    fi
elif [[ $CMD == 'stop' ]]; then
    ## Python exit port
    EXIT_PORT=$3

    if [ ! -f "$PID_FILE" ]; then
        echo "have no gameserver"
        exit 1
    fi

    pid=`cat $PID_FILE`
    exist_pid=`pgrep skynet | grep $pid`
    if [ -z "$exist_pid" ] ;then
      echo "have no gameserver"
      exit 1
    fi

    {
        flock -n 3
        [ $? -eq 1 ] && { echo fail; exit; }
        echo $$
        python python/exit_client.py $EXIT_PORT

    } 3<>mylockfile 

    exist_pid=`pgrep skynet | grep $pid`
    if [ -z "$exist_pid" ] ; then
      echo "have no gameserver"
      exit 1
    else
      echo -n $"$pid gameserver will be killed: "
      killproc -p $PID_FILE
      echo
    fi
fi
