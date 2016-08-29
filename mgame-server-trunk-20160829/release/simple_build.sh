#!/bin/bash

echo 'Make skynet'
cur_dir=`pwd`
cd $cur_dir/../skynet
echo `pwd`
make linux -j8

echo 'Make fgame'
cd $cur_dir/../fgamesvr
echo `pwd`
make


#../skynet/make linux -j8
#../fgamesvr/make 



