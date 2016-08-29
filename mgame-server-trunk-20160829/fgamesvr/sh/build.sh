#!/bin/sh

GAME_DIR='/root/netfgame/trunk/fgamesvr'

WORK_DIR=$(dirname $(readlink -f $0))
FILES=$1
MAP=$2
BUILD_NAME=''
RES=''
SVN=$(which svn)

function GameDirCheck() {
    if [ ! -d $GAME_DIR ]; then
        echo "$GAME_DIR not found..."
        exit 1
    fi
}

function SvnCheckout() {
    if [ "$FILES" != "" ]; then
        if [ -x $SVN ]; then
            echo "not found svn cmd,please check has the svn command"
            exit 1
        fi
        BUILD_NAME=${FILES##*/}
        RES=${BUILD_NAME}"/res"
        svn checkout $FILES
	    ret=$?
        if [  $ret -ne 0 ]; then
            echo "cehckout $FILES failed $ret"
            exit 1
        fi
    	SRC=$WORK_DIR"/"$RES"/table/*"
        \cp -f $SRC $GAME_DIR"/data/"
    	cd $GAME_DIR
    	rm -rf script
    	svn update
    	rm -rf ${WORK_DIR}"/"${BUILD_NAME}
    fi    
}

function Map() {
    #重新生成map文件
    if [ "$MAP" == "yes" ]; then
        LUA=$(which lua)
        if [ ! -x $LUA ]; then
            echo "not found lua command....."
            exit 1
        fi
        ret=`$LUA -v | grep "5.3"` 
	    if [ "$ret" == "" ]; then
            echo "lua version less than 5.3"
            exit 1
	    fi	

        cd $GAME_DIR"/../tools"
        $LUA linux_tmx_parser.lua $WORK_DIR"/"$RES"/map"
    fi
}

function Run() {
    SH=$GAME_DIR/../start/restart_game.sh
    $SH
}


GameDirCheck

SvnCheckout

Map

Run
