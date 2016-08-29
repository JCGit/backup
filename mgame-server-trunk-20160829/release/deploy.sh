#!/bin/bash

export CUR_DIR=$(dirname $(readlink -f $0))
export PATH=$PATH:$CUR_DIR

DESTS=$1
RELEASE_DIC="/data/fgame/release"

function deploy_to() {
    local fgame_program=$1
    local ds=$(basename $fgame_program)
    local DATE=$(date '+%Y-%m-%d_%H_%M_%S')
    local file=$(ls -t $RELEASE_DIC/archive/*.tar.gz | head -n 1)

    # Back up binary files
    local current_path=`pwd`
    cd $fgame_program
    tar -zcf $RELEASE_DIC/bak/bak-$ds-${DATE}.tar.gz * --exclude=fgamesvr/log --exclude=fgamesvr/busilog

    # Deploy new binary files
    bash ./start/stop_game.sh
    tar -zxf $file
    find -name .svn | xargs rm -rf
    bash ./start/start_game.sh
    cd $current_path
}

function del_old_files() {
    file_reg=$1
    remain_count=$2
    fcount=`ls $file_reg | wc -l`
    if [ $fcount -gt $remain_count ]; then
        delete_count=$(($fcount - $remain_count))
        ls -rt $file_reg | head -n $delete_count | xargs rm -rf
    fi
}

# Deploy archive file to destination
OIFS=$IFS
IFS=';'
for ds in $DESTS ; do
    deploy_to $ds
done
IFS=$OIFS

# Delete old files
del_old_files "$RELEASE_DIC/bak/*.tar.gz" 4
del_old_files "$RELEASE_DIC/archive/*.tar.gz" 4
