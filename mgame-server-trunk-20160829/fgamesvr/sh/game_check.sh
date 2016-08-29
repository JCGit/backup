#!/bin/bash
#此脚本需要放置到生成pid文件同级目录下,pid文件在pid服务结束后需要删除重新生成,已保证pid文件与重启的服务pid匹配,
#监测服务pid挂了后,只发送一次报警邮件,当挂掉pid重新恢复后,错误日志将被删除
#路径
base=`pwd`
#保存pid文件名的文件
filename="name.txt"
#当前问下下的所有pid检测是否全部通过
all=true
day=`date`
echo "run start: ${day}"
for j in `cat ${filename}`;do
    for i in `cat ${j}`;do
        #是否发送过邮件了 默认没有
        send=false
        pid=`ps aux |grep -i ${i} |grep -v grep |awk '{print $2}'`
        if [ "" == "${pid}" ]; then
            #有错误日志的情况下，检测该pid是否发送过邮件
            name=${base}"/error_"${j}".log"
            if [ -f "${name}" ]; then
                for k in `cat error_${j}.log`;do
                      if [ "${k}" == "${i}" ]; then
                           send=true
                      fi
                done
            fi
            if [ "${send}" == "true" ]; then
                all=false
                continue
            fi
            #发送邮件
            echo "pid(${j},${i})找不到，请检查服务器启动是不是失败了!!" |mutt -s "服务器监听失败" liuyonghua@61games.hk
            all=false
            echo "${i}" >> error_${j}.log
            echo "${i}"
        fi
    done
done
#全部通过将上次错误文件删除
if [ "true" == "${all}" ]; then
    rm -rf error_*.log
fi
