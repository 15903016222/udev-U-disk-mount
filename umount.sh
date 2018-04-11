#!/bin/sh

MOUNT="/bin/mount"
UMOUNT="/bin/umount"
RM="/bin/rm"

if [ "remove" == $ACTION ]
then
	DEV_NAME=`cat /proc/mounts | grep "$DEVNAME" | awk '{print $1}'`
	DIR=`cat /proc/mounts | grep "$DEVNAME" | awk '{print $2}'`

	$UMOUNT $DEV_NAME
	$RM $DIR
fi

echo "remove" >> /tmp/test.txt
echo $DEVNAME >> /tmp/test.txt
echo $DEV_NAME >> /tmp/test.txt
echo $DIR >> /tmp/test.txt
