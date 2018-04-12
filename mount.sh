#!/bin/sh

MOUNT="/bin/mount"
UMOUNT="/bin/umount"
SEND="/usr/bin/send"
RM="/bin/rm -rf"

name=$1
name=${name%_*}

DIR="/media/$name"
i=0
if ! cat /proc/mounts | awk '{print $1}' | grep -q "$DEVNAME"
then
	echo "mount" >> /tmp/test.txt
	while cat /proc/mounts | awk '{print $2}' | grep -q "$DIR" 
	do
		i=`expr $i + 1`
		DIR="/media/$name$i"
	done
	mkdir -p $DIR

	# 发送挂载信号
	$SEND $DIR
else
	return
fi

