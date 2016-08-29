#/bin/base

pid=`pgrep redis-server`
echo $pid
kill -9 $pid

sleep 2

cp /home/diandian/work/dump.rdb /root/redis_db/

sleep 2

redis-server /etc/redis.conf
