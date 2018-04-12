#!/bin/sh

MOUNT="/bin/mount"
UMOUNT="/bin/umount"
SEND="/usr/bin/send"
RM="/bin/rm -rf"

name=$1
name=${name%_*}

DIR="/media/$name"

if cat /proc/mounts | awk '{print $1}' | grep -q "$DEVNAME"
then
	echo "umount" >> /tmp/test.txt
	# 发送卸载信号
	$SEND $DIR
else
	return
fi

