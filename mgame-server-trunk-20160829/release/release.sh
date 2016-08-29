#!/bin/bash

export CUR_DIR=$(dirname $(readlink -f $0))
export PATH=$PATH:$CUR_DIR
export BUILD_DIR=$CUR_DIR/build

COLOR_GREEN='\x1b[32m'
COLOR_REST='\x1b[0m'


case "$1" in
    copy|deploy)
        ;;
    *)
        echo $"Usage: $0 {copy|deploy}"
        exit 2
esac

OPTION="$1"

# Pack the server program
if [ $OPTION == 'copy' ]; then
    bash $CUR_DIR/pack.sh full
    file=$(ls -t $BUILD_DIR/*.tar.gz | head -n 1)
    printf "PACK FILE IS : ${COLOR_GREEN} $file ${COLOR_REST}\n"
fi

# Choose a server for update
source conf.sh
server_list_len=$((${#SERVER_LIST[@]}/6))

function choose_server() {
    printf "\n========================================\n"
    printf "             Server List\n"
    for ((i=0; i<$server_list_len; i=i+1)); do
        printf "[%d] %-10s %-20s  %-20s %s\n" $i ${SERVER_LIST[$(($i*6))]} \
            ${SERVER_LIST[$(($i*6+1))]} ${SERVER_LIST[$(($i*6+2))]} \
            ${SERVER_LIST[$(($i*6+3))]}
    done
    printf "========================================\n"
    printf "             Server List\n"

    printf "Please Enter server index(or $COLOR_GREEN a $COLOR_REST FOR $COLOR_GREEN outer $COLOR_REST ALL): "
    read -e server_index

}

where=inner
while true; do
    choose_server

    if [ "$server_index" == 'a' ]; then
        where=outer
        break;
    elif [[ "$server_index" =~ ^[0-9]?$ && $server_index -ge 0 && $server_index -le $server_list_len ]]; then
        where="${SERVER_LIST[$(($server_index*6))]}"
        break;
    fi
done

if [ "$where" == "inner" ]; then
    bash remote_deploy.sh $OPTION $server_index $file
else
    DATE=$(date '+%Y-%m-%d_%H_%M_%S')
    current_path=`pwd`
    release_arch=release-"$DATE".tar.gz
    release_arch_local=/tmp/$release_arch

    if [[ ! -z "$file" ]]; then
        simple_file=$(basename $file)
        file=./build/$simple_file
    fi

    cd $CUR_DIR
    if [[ $OPTION == "deploy" ]]; then
        tar -zcf $release_arch_local * --exclude=build
    else
        tar -zcf $release_arch_local *
    fi
    cd $current_path

    echo "Upload server archive file to $OUTER_MASTER"
    scp $release_arch_local $OUTER_USER@$OUTER_MASTER:/tmp/
    ssh $OUTER_USER@$OUTER_MASTER "mkdir -p /tmp/release-$DATE
        cd /tmp/release-$DATE
        tar -zxf ../$release_arch
        bash remote_deploy.sh $OPTION $server_index $file
        rm -rf /tmp/*$DATE*"

    rm $release_arch_local
fi
