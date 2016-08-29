#!/bin/bash
base_path=`pwd`
#删除以前的数据
rm -rf *.pid
rm -rf name.txt
#重新生成监测pid数据 服务名必须含有config
pid=`ps aux |grep -i skynet |grep -v grep |awk '{print $2}'`
for i in ${pid};do
	name=`ps aux |grep -i ${i} |grep -v grep |awk '{print $12}'`
	filename=${name#*/}
	echo ${i} >> ${filename}"."${i}".pid"
	echo ${filename}"."${i}".pid" >> name.txt
done