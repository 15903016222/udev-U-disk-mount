#!/bin/sh

MOUNT="/bin/mount"
UMOUNT="/bin/umount"
SEND="/usr/bin/send"

name=$1
name=${name%_*}

if cat /proc/mounts | awk '{print $1}' | grep -q "$DEVNAME"
then
	DIR=`cat /proc/mounts | grep "$DEVNAME" | awk '{print $2}'`
	# 发送卸载信号
	$SEND $DIR
else
	return
fi

