#!/bin/sh
mkdir -p temp
mkdir -p proto-bin

VERSION_TRUNK=1
VERSION_MPLUS=2
VERSION_CHINA=3

mode="$1"
if [ -z $mode ]; then
mode="trunk"
fi

version=$VERSION_TRUNK

if [ $mode = "mplus" ]; then
version=$VERSION_MPLUS
fi

if [ $mode = "china" ]; then
version=$VERSION_CHINA
fi

echo version=$version

protos=(
    "Activity.proto"
    "Base.proto"
    "ClientSvrMsg.proto"
    "Friend.proto"
    "Glyphs.proto"
    "Message.proto"
    "Player.proto"
    "Pvp.proto"
    "PvpPersonalRoom.proto"
    "Assistant.proto"
    "Institute.proto"
    "PvpChest.proto"
)

for ((i=0;i<${#protos[@]};i++)); do
gcc -P -E -C -xc -DVERSION=$version -otemp/${protos[i]} proto/${protos[i]}
done

protoc -Itemp -o proto-bin/ClientSvrMsg.pb --include_imports temp/ClientSvrMsg.proto
#rm -rf temp
