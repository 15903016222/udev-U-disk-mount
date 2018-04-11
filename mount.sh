#!/bin/sh

MOUNT="/bin/mount"
UMOUNT="/bin/umount"
RM="/bin/rm -rf"

name=$1
name=${name%_*}

DIR="/media/$name"

if [ "add" == $ACTION ]
then
	if ! cat /proc/mounts | awk '{print $1}' | grep -q "$DEVNAME"
	then
		! test -d $DIR && mkdir -p $DIR

		# 发送挂载信号
#		$MOUNT $DEVNAME $DIR
	else
		return
	fi
fi

