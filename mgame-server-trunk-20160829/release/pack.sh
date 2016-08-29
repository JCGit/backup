#!/bin/bash

# 参数控制 full包 update包 -- 默认生成update包 full包需要输入 full
default="update"
full="full"

case "$1" in
    update|full)
        ;;
    *)
        echo $"Usage: $0 {update|full}"
        exit 2
esac

mode="$1" #这里只用传一个参数 update or full or 不传

# cp需要的文件到release/fgame
#只传so文件+可执行文件+需要用到的脚本+部分配置(服务器配置不用传)
#根据mode来判断 update包只传逻辑脚本 full则传so文件+可执行文件+脚本+全部配置

#当前路径
CUR_DIR=$(cd "$(dirname "$0")"; pwd)
echo $CUR_DIR
#原路径(源文件所在路径)
SRC_DIR=$(dirname ${CUR_DIR})
echo $SRC_DIR
#目标路径
DEST_DIR=${CUR_DIR}"/fgame"
echo $DEST_DIR

BUILD_DIR=$CUR_DIR/build

#编译skynet源码
SKYNET_DIR=${SRC_DIR}/skynet
cd ${SKYNET_DIR}
make linux

GAME_DIR=${SRC_DIR}/fgamesvr
cd ${GAME_DIR}
make
sh gen_pb.sh
make

cd ${SRC_DIR}/release

#skynet所在路径
SKYNET_DEST_DIR=${DEST_DIR}/skynet
echo "skynet dir is $SKYNET_DIR"
echo "skynet des dir is $SKYNET_DEST_DIR"
#逻辑脚本所在路径
FGAME_DEST_DIR=${DEST_DIR}/fgamesvr
echo "game dir is $GAME_DIR"
echo "game des dir is $FGAME_DEST_DIR"

#删除
if [ -d ${DEST_DIR} ]; then
    rm -rf ${DEST_DIR}
fi

#存在与否创建目录，父目录不存在也会创建
mkdir -p ${SKYNET_DEST_DIR}

#创建日志&oss目录
LOG_DIR=${FGAME_DEST_DIR}/log
OSS_DIR=${FGAME_DEST_DIR}/oss
echo "will create $LOG_DIR"
mkdir -p ${LOG_DIR}
echo "will create $OSS_DIR"
mkdir -p ${OSS_DIR}


#TODO:只传需要的文件
cp -rf ${GAME_DIR}/*.sh         ${FGAME_DEST_DIR}
cp -rf ${GAME_DIR}/conf       ${FGAME_DEST_DIR}
cp -rf ${GAME_DIR}/cservice   ${FGAME_DEST_DIR}
cp -rf ${GAME_DIR}/data       ${FGAME_DEST_DIR}
cp -rf ${GAME_DIR}/luaclib    ${FGAME_DEST_DIR}
cp -rf ${GAME_DIR}/lualib     ${FGAME_DEST_DIR}
cp -rf ${GAME_DIR}/proto      ${FGAME_DEST_DIR}
cp -rf ${GAME_DIR}/proto-bin  ${FGAME_DEST_DIR}
cp -rf ${GAME_DIR}/python     ${FGAME_DEST_DIR}
cp -rf ${GAME_DIR}/script     ${FGAME_DEST_DIR}
cp -rf ${GAME_DIR}/setting    ${FGAME_DEST_DIR}
cp -rf ${GAME_DIR}/sql        ${FGAME_DEST_DIR}
cp -rf ${GAME_DIR}/sh         ${FGAME_DEST_DIR}


cp -f ${SKYNET_DIR}/skynet    ${SKYNET_DEST_DIR}
cp -rf ${SKYNET_DIR}/cservice ${SKYNET_DEST_DIR}
cp -rf ${SKYNET_DIR}/luaclib  ${SKYNET_DEST_DIR}
cp -rf ${SKYNET_DIR}/lualib   ${SKYNET_DEST_DIR}
cp -rf ${SKYNET_DIR}/service  ${SKYNET_DEST_DIR}

cp -r ${CUR_DIR}/../start     ${DEST_DIR}/start

cp -rf ${CUR_DIR}/../version.md     ${DEST_DIR}/version.md
#打包时间输入到version.md中
echo -n `date` >> ${DEST_DIR}/version.md

rm ${DEST_DIR}/start/server_dependency.sh

#包名 fgame+时间+包类型
DATE=$(date '+%Y-%m-%d_%H_%M_%S')

cd $CUR_DIR/fgame
if [ -d "$BUILD_DIR" ]; then
    rm -rf $BUILD_DIR/*
else
    mkdir -p $BUILD_DIR
fi

find -name .svn | xargs rm -rf
TAR="$BUILD_DIR/fgame-""$DATE""_$mode"".tar.gz"
echo "out put : $TAR"
tar -czf $TAR *
#tar -czf $BUILD_DIR/fgame-"$DATE"_"$mode".tar.gz *

rm -rf ${DEST_DIR}


