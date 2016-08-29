#!/bin/bash
base_path=`pwd`
log_path=${base_path}"/log/"
log_name=$1
today=`date +%Y%m%d`

cd ${log_path}
if [ -f ${log_name}"-"${today}".log" ]; then
	name=${log_name}"-"${today}"*"
	
	index=`ls ${name} | wc -l`
	index=`expr ${index} + 1`

	mv ${log_name}"-"${today}".log" ${log_name}"-"${today}"-"${index}".log"
fi
