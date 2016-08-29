#!/bin/sh
#Description: Backup file that should not be changed every deploy.

## Files are in fgamesvr directory and must be started with '/'
FILES=(/setting/balance.json \
       /setting/clustername.lua \
       /setting/server_config.lua \
       /sh/initdb.sh \
       /conf)

CUR_PATH=$(dirname $(readlink -f $0))

BAK=$CUR_PATH/../bak
FGAMESVR=$CUR_PATH/../fgamesvr

mkdir -p $BAK

for file_or_dir in ${FILES[@]}; do
    if [ ! -e "$BAK$file_or_dir" ]; then
        base_dic=${file_or_dir%/*}
        mkdir -p $BAK$base_dic
        cp -r $FGAMESVR$file_or_dir $BAK$base_dic
    fi
done

cp -r $BAK/* $FGAMESVR/

## On server that should be on, assign true value to it.
export GSLBSERVER_ON=true
export CENTERSERVER_ON=true
export LOGINSERVER_ON=true
export RANKSERVER_ON=true
export COMRANKSERVER_ON=true
export LOGSERVER_ON=true
export GAMESERVER_ON=true

## The Python scripts used exit port
export RANK_EXIT_PORT=7605
export COMRANK_EXIT_PORT=7606
export GAME_EXIT_PORT=7604

## Configure files of each server.
#export GSLBSERVER_CONFIG=conf/config.gslb
#export CENTERSERVER_CONFIG=conf/config.center
#export LOGINSERVER_CONFIG=conf/config.login
#export RANKSERVER_CONFIG=conf/config.rank
#export COMRANKSERVER_CONFIG=conf/config.comrank
#export LOGSERVER_CONFIG=conf/config.logserver
#export GAMESERVER_CONFIG=conf/config

## Platform
#export PLATFORM=qq


## Tell start_game.sh to init db
export INIT_DB=false
