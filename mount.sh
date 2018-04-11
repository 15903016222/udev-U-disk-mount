#!/bin/sh

MOUNT="/bin/mount"
UMOUNT="/bin/umount"
RM="/bin/rm -rf"

name=$1
name=${name%_*}

DIR="/media/$name"

i=0

if [ "add" == $ACTION ]
then
	if ! cat /proc/mounts | awk '{print $1}' | grep -q "$DEVNAME"
	then
		while cat /proc/mounts | awk '{print $2}' | grep -q "$DIR" 
		do
			echo "00000000" >> /tmp/test.txt
			i=`expr $i + 1`
			DIR="/media/$name$i"
		done

		mkdir -p $DIR

		$MOUNT $DEVNAME $DIR
	else
		return
	fi
fi

