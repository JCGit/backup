#!/bin/bash

CUR_DIR=$(dirname $(readlink -f $0))

if [ ! -f "$CUR_DIR/server_dependency.sh" ]; then
  echo "Lack of file $CUR_DIR/server_dependency.sh" && exit -1
fi

. $CUR_DIR/server_dependency.sh

FGAMESVR=$CUR_DIR"/../fgamesvr"

cd $FGAMESVR
bash sh/gen_pb.sh

$INIT_DB && bash sh/initdb.sh $PLATFORM

$GSLBSERVER_ON && bash $FGAMESVR/gslbserver.sh start release $GSLBSERVER_CONFIG

sleep 2 # wait 2 second

$CENTERSERVER_ON && bash $FGAMESVR/centerserver.sh start release $CENTERSERVER_CONFIG
$LOGINSERVER_ON && bash $FGAMESVR/loginserver.sh start release $LOGINSERVER_CONFIG
$RANKSERVER_ON && bash $FGAMESVR/rankserver.sh start release $RANKSERVER_CONFIG
$COMRANKSERVER_ON && bash $FGAMESVR/comrankserver.sh start release $COMRANKSERVER_CONFIG
$LOGSERVER_ON && bash $FGAMESVR/logserver.sh start release $LOGSERVER_CONFIG
$GAMESERVER_ON && bash $FGAMESVR/gameserver.sh start release $GAMESERVER_CONFIG
