#!/bin/bash

CUR_DIR=$(dirname $(readlink -f $0))
sh $CUR_DIR/stop_game.sh
sleep 1
sh $CUR_DIR/start_game.sh
