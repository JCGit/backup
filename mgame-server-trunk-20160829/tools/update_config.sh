#!/bin/sh
# Description: A tools for updating config files of plan staff

if [ -z "$1" ]; then
    echo "Usage $0 \$path/to/config/root"
    exit 1
fi

CONFIG_DIR=$1
CUR_PATH=$(dirname $(readlink -f $0))

DATA_PATH=$CUR_PATH/../fgamesvr/data
cp $CONFIG_DIR/res/table/*.csv $DATA_PATH/

NOT_NEED_FILES=(Badge.csv GoodsInfo.csv Music.csv \
    NameArea.csv ScoreInfo.csv dailyMission.csv \
    pinyin.csv playerInfo.csv IntelligentGuide.csv)

for file in ${NOT_NEED_FILES[@]}; do
    rm $DATA_PATH/$file
done

current_path=`pwd`
cd $CUR_PATH
$CUR_PATH/../skynet/3rd/lua/lua linux_tmx_parser.lua $CONFIG_DIR/res/map $DATA_PATH/map.lua
cd $current_path
