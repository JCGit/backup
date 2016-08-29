#!/bin/sh

db_name="fgame"

if [[ ! -n $1 ]]
then
    db_name="fgame"
elif [[ $1 == "qq" ]]
then
    db_name="qq_fgame"
elif [[ $1 == "weixin" ]]
then
    db_name="weixin_fgame"
fi

des="./sql/"$db_name.sql
sed -e 's/fgame/'$db_name'/' ./sql/database.sql > $des

mysql -uroot -p < $des
