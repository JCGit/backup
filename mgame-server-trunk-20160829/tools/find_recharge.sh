#!/bin/bash

###
# this script use to find the recharge log
#   1.specify which uin you want to find(see uin.txt,same dir as this bash)
#   2.specify the output directory
#   3.specify the file list
#   4.example as follows
#   5.two column in uin.txt,first is the uin.the other on is uname   
#   6.the output file with uin.uname.line.log format(line to tell you how many rows in this file)
#   
#   find_recharge.sh ~/rankcheck test1.log test2.log test3.log
#   ls -lrt ~/rankcheck
###

function usage()
{
    echo "$0 [outdir] [file list(you want to find in)]"
    exit 1;
}

if [ $# -lt 2 ]; then
    usage
fi

echo "param num is $#"

#
if [ ! -d "$1" ]; then 
    echo "create dir $1"
    mkdir -p "$1" 
fi 

file_list=""

#append file
for ((i=2; i <=$#; i++)); do
    j=${!i}
    file_list="$file_list $j" 
done

echo "the file you want to find is : $file_list"

while read uin uname 
do
    echo "LINE : " $uin " name : "  $uname
    find_str="uin=$uin&changevalue=0"
    echo "find [$find_str]"
    grep "$find_str" "$file_list" > "$1/$uin.$uname.log"
    file="$1/$uin.$uname.log"
    count=`cat "$file" | wc -l`
    mv "$file" "$1/$uin.$uname.$count.log"
done < uin.txt

echo "please sea output in $1 directory!!"
