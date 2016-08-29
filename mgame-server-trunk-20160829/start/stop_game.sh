#!/bin/bash

CUR_DIR=$(dirname $(readlink -f $0))

if [ ! -f "$CUR_DIR/server_dependency.sh" ]; then
  echo "Lack of file $CUR_DIR/server_dependency.sh" && exit -1
fi

. $CUR_DIR/server_dependency.sh

cd $CUR_DIR/../fgamesvr

$GSLBSERVER_ON && bash gslbserver.sh stop
$CENTERSERVER_ON && bash centerserver.sh stop
$LOGINSERVER_ON && bash loginserver.sh stop
$RANKSERVER_ON && bash rankserver.sh stop release $RANK_EXIT_PORT
$COMRANKSERVER_ON && bash comrankserver.sh stop release $COMRANK_EXIT_PORT
$LOGSERVER_ON && bash logserver.sh stop
$GAMESERVER_ON && bash gameserver.sh stop release $GAME_EXIT_PORT
