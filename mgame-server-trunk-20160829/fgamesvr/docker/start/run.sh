#!/bin/bash

CUR_DIR=$(dirname $(readlink -f $0))
. $CUR_DIR/conf.sh

COLORE_RED='\033[31m'
COLOR_GREEN='\033[32m'
COLOR_RESET='\033[0m'

width=4
server_list_len=$((${#SERVER_LIST[@]}/${width}))
server_port_len=$((${#SERVER_EXPOSE_PORT[@]}))
ipaddr=''        #the game runing on the machine
volumePath=''    #host path mount to the container 
imageId=''       #the container launch from
containerName='' #the container name

function usage(){
    printf "$0 [IMAGE ID] \n"
    exit 1;
}

if [ $# -lt 1 ]; then
    usage
else
    imageId="$1"
fi

function choose_ip() {
    printf "\n========================================\n"
    printf "             Server List\n"
    for ((i=0; i<$server_list_len; i=i+1)); do
        printf "[%d] %20s %20s  %20s %10s\n" $i ${SERVER_LIST[$(($i*$width))]} \
            ${SERVER_LIST[$(($i*$width+1))]} ${SERVER_LIST[$(($i*$width+2))]} \
            ${SERVER_LIST[$(($i*$width+3))]}
    done
    printf "========================================\n"
    printf "             Server List\n"

    printf "Please Enter server index to run this game : "
    read -e server_index
}

while true; do
    choose_ip

    if [[ "$server_index" =~ ^[0-9]?$ && $server_index -ge 0 && $server_index -le $server_list_len ]]; then
        ipaddr="${SERVER_LIST[$(($server_index*$width))]}"
        volumePath="${SERVER_LIST[$(($server_index*$width+2))]}"
        containerName="${SERVER_LIST[$(($server_index*$width+3))]}"
        break;
    else
        echo -e "please check your input[$COLORE_RED $server_index $COLOR_RESET]"
        exit 1;
    fi
done

echo -e "\nplease sure ip:$COLOR_GREEN $ipaddr $COLOR_RESET \tdata path:$COLOR_GREEN $volumePath $COLOR_RESET[Y/N]:\c"
read -e sure

if [[ "$sure" -ne 'Yes' && "$sure" -ne 'Y' && "$sure" -ne 'y' ]]; then
    echo "bye $sure"
    exit;
fi


exposedPort=''
for ((i=0; i<$server_port_len; i=i+1)); do
    port="${SERVER_EXPOSE_PORT[$(($i))]}"
    if [ "$port" == "7777" ] ; then
        exposedPort=${exposedPort}"-p "${ipaddr}:$port:22" "
    else
        exposedPort=${exposedPort}"-p "${ipaddr}:$port:$port" "
    fi
done

EXE=`which docker`
if [[ ! -x "$EXE" ]]; then
    echo -e "$COLOR_RED $CMD $COLOR_RESET is not a binary file ^_^"
    exit 1;
fi

CMD="$EXE run -itd --privileged=true $exposedPort --name $containerName -v ${volumePath}:/fgame $imageId "

echo -e "\nplease sure CMD[$COLOR_GREEN $CMD $COLOR_RESET] [Y/N]:\c"
read -e sure
if [[ "$sure" -ne 'Yes' && "$sure" -ne 'Y' && "$sure" -ne 'y' ]]; then
    echo "bye $sure"
    exit;
fi

$CMD