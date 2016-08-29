#!/bin/sh

MASTER='115.159.188.208'
USER='fgame'

MYSQL_USER='root'
MYSQL_PASSWORD='MeiweiZZ0321'

REDIS_AUTH='crs-8vg42ila:MeiweiZZ0321'

SERVER_LIST=(\
    redis     '10.66.125.63'     'redis'         \
    mysql     '10.66.140.49'     'mysql'         \
    server    '10.105.112.32'    'QQ中心服'      \
    server    '10.105.118.6'     'QQ逻辑服1'     \
    server    '10.105.86.119'    '联想服'     \
    server    '10.105.66.183'    '排行服'        \
)

server_list_len=$((${#SERVER_LIST[@]}/3))

function choose_server() {
    printf "\n========================================\n"
    printf "             Server List\n"
    for ((i=0; i<$server_list_len; i=i+1)); do
        printf "[%d] %-10s %-20s  %-20s\n" $i ${SERVER_LIST[$(($i*3))]} \
            ${SERVER_LIST[$(($i*3+1))]} ${SERVER_LIST[$(($i*3+2))]}
    done
    printf "========================================\n"
    printf "             Server List\n"

    printf "Please Enter server index : "
    read -e server_index
}

while true; do
    choose_server

    if [[ "$server_index" =~ ^[0-9]?$ && $server_index -ge 0 && $server_index -le $server_list_len ]]; then
        break;
    fi
done

where=${SERVER_LIST[$(($server_index * 3))]}
ip=${SERVER_LIST[$(($server_index*3+1))]}

if [[ ! -f ~/.ssh/id_rsa || ! -f ~/.ssh/id_rsa.pub ]]; then
    ssh-keygen -t rsa
    ssh $USER@$MASTER 'mkdir -p .ssh && cat >> .ssh/authorized_keys' < ~/.ssh/id_rsa.pub
fi

case "$where" in
    master)
        ssh $USER@$MASTER
        ;;
    server)
        ssh -t $USER@$MASTER "ssh -t $USER@$ip"
        ;;
    redis)
        ssh -t $USER@$MASTER "redis-cli -h $ip -a $REDIS_AUTH"
        ;;
    mysql)
        ssh -t $USER@$MASTER "mysql -h $ip -u $MYSQL_USER -P 3306 -p'$MYSQL_PASSWORD'"
        ;;
esac
