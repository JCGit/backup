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
PID_FILE=$CUR_PATH/rank.pid

if [[ $CMD == 'start' ]] || [[ -z  $CMD ]]; then
    if [ -z "$3" ]; then
        CONFIG=$CUR_PATH/conf/config.rank
    else
        CONFIG=$CUR_PATH/$3
    fi

    ulimit -c unlimited
    sh sh/log_name.sh rank

    if [[ -z $MODE  ]] || [[ $MODE == "release" ]]; then
        echo -n $"Starting rankserver: "
        $CUR_PATH/../skynet/skynet $CONFIG
        if [ $? -eq 0 ]; then
            success && echo
        else
            failure && echo
        fi
    elif [ $MODE == "debug" ]
    then
        sed -e 's/logger/--logger/' -e 's/daemon/--daemon/' $CONFIG > $CUR_PATH/conf/debug_config.rank
        $CUR_PATH/../skynet/skynet $CUR_PATH/conf/debug_config.rank
    fi
elif [ $CMD == 'stop' ]; then
    ## Python exit port
    EXIT_PORT=$3

    if [ ! -f $PID_FILE ] ;then
      echo "have no rankserver"
      exit 0
    fi

    pid=`cat $PID_FILE`
    exist_pid=`pgrep skynet | grep $pid`
    if [ -z "$exist_pid" ] ;then
      echo "have no rankserver"
      exit
    fi

    {
        flock -n 3
        [ $? -eq 1 ] && { echo fail; exit; }
        echo $$
        python python/exit_rank_client.py $EXIT_PORT
    } 3<>mylockfile

    exist_pid=`pgrep skynet | grep $pid`
    if [ -z "$exist_pid" ] ;then
      echo "have no rankserver"
      exit
    else
      echo -n $"$pid rankserver will killed"
      killproc -p $PID_FILE
      echo
    fi
fi
