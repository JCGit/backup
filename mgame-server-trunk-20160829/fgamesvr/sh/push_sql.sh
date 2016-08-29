#!/bin/bash
cd /home/fgame/fgame/fgamesvr/oss
#cd /root/server/fgamesvr/oss
bak_path="/home/fgame/dbbak/"
if [ ! -d "${bak_path}" ]; then
	mkdir /home/fgame/dbbak/
	echo "mkdir /data/dbbak/"
fi
echo "start time: "`date "+%Y-%m-%d %H:%M:%S"`
pre_time=`date -d "-1 hour" +"%Y%m%d%H"`
filename=${pre_time}".sql"
echo "file ： ${filename}"
if [ -f "${filename}" ]; then
#	mysql -uroot -proot -f --default-character-set=utf8 fgame < ${filename}
	mysql -h 119.29.26.122 -P3307 -uweb -p61Statistical@Games_ -f --default-character-set=utf8 oss_fgame_data < ${filename}
	echo "mysql run end...."
	mv ${filename} /home/fgame/dbbak/
	echo "end time: "`date "+%Y-%m-%d %H:%M:%S"`
	echo "mv ${filename} success"
fi


