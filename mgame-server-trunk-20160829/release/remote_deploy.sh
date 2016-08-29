#!/bin/bash

export CUR_DIR=$(dirname $(readlink -f $0))
export PATH=$PATH:$CUR_DIR

COLOR_GREEN='\x1b[32m'
COLOR_REST='\x1b[0m'

SERVER_RELEASE="/data/fgame/release"

if [[ -z "$1" || -z "$2" ]]; then
    echo "Usage $0 {copy|deploy} {a|0|1|2..} [file_name]"
    exit 6
fi

OPTION=$1
SERVER_INDEX=$2
FILE=$3

source conf.sh

function remote_copy() {
    local node="$1"
    local user="$2"
    local server="$3"
    local user_at_server=$user'@'$server
    local simple_file=$(basename $FILE)

    # Init environment of server deploy
    # Update scritp files and deploy them
    echo
    if [ "$node" != "master"  ]; then
        echo "Make directories for $SERVER_RELEASE on $server"
        ssh $user_at_server "mkdir -p $SERVER_RELEASE/archive; mkdir -p $SERVER_RELEASE/bak;"
        echo "Upload deploy.sh to $server"
        scp $CUR_DIR/deploy.sh $user_at_server:$SERVER_RELEASE
        echo "Upload $simple_file to $server"
        scp $FILE $user_at_server:$SERVER_RELEASE/archive
    else
        echo "Make directories for $SERVER_RELEASE on $server"
        mkdir -p $SERVER_RELEASE/archive; mkdir -p $SERVER_RELEASE/bak;
        echo "Upload deploy.sh to $server"
        cp $CUR_DIR/deploy.sh $SERVER_RELEASE
        echo "Upload $simple_file to $server"
        cp $FILE $SERVER_RELEASE/archive
    fi
    echo
}

function remote_deploy() {
    local node="$1"
    local user="$2"
    local server="$3"
    local user_at_server=$user'@'$server

    echo "Execute deploy.sh to complete deploy at $server"
    if [ "$node" != "master" ]; then
        ssh $user_at_server "bash $SERVER_RELEASE/deploy.sh '$dest'"
    else
        bash $SERVER_RELEASE/deploy.sh $dest
    fi
}

server_list_len=$((${#SERVER_LIST[@]}/6))
if [[ "$SERVER_INDEX" == 'a' ]]; then
    for ((i=0; i<$server_list_len; i=i+1)); do
        where=${SERVER_LIST[$(($i*6))]}
        if [ $where == "outer" ]; then
            if [ "${SERVER_LIST[$(($i*6+1))]}" == "$OUTER_MASTER" ]; then
                node="master"
            else
                node="slave"
            fi
            user=${SERVER_LIST[$(($i*6+4))]}
            server=${SERVER_LIST[$(($i*6+2))]}
            dest=${SERVER_LIST[$(($i*6+5))]}
            if [ $OPTION == 'copy' ]; then
                printf "Copy to $COLOR_GREEN $server($where) $COLOR_REST\n"
                remote_copy "$node" "$user" "$server"
            else
                printf "Deploy to $COLOR_GREEN $server($where) $COLOR_REST\n"
                remote_deploy "$node" "$user" "$server" "$dest"
            fi
        fi
    done
else
    where=${SERVER_LIST[$(($SERVER_INDEX * 6))]}
    if [ "${SERVER_LIST[$(($SERVER_INDEX*6+1))]}" == "$OUTER_MASTER" ]; then
        node="master"
    else
        node="slave"
    fi
    user=${SERVER_LIST[$(($SERVER_INDEX*6+4))]}
    server=${SERVER_LIST[$(($SERVER_INDEX*6+2))]}
    dest=${SERVER_LIST[$(($SERVER_INDEX*6+5))]}
    if [ $OPTION == 'copy' ]; then
        printf "Copy to $COLOR_GREEN $server($where) $COLOR_REST\n"
        remote_copy "$node" "$user" "$server"
    else
        printf "Deploy to $COLOR_GREEN $server($where) $COLOR_REST\n"
        remote_deploy "$node" "$user" "$server" "$dest"
    fi
fi
