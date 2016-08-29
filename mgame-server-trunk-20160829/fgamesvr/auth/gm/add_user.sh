#!bin/bash

DIR=`dirname $0`;
BIN='/data/fgame/php5.6/bin/php'

if [ $# -lt 2 ]; then
    echo "usage : $0 [user] [pass]"
    exit
fi

if [ -x "$BIN" ]; then
    cmd="$BIN $DIR/register.php $1 $2"
    echo "the cmd is $cmd"
    $cmd
else
    echo "$BIN not binary file"
fi

