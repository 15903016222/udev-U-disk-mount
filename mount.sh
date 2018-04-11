#!/bin/sh

MOUNT="/bin/mount"
UMOUNT="/bin/umount"
RM="/bin/rm -rf"

name=$1
name=${name%_*}

DIR="/media/$name"

echo "00000000" >> /tmp/test.txt

if [ "add" == $ACTION ]
then
	if ! cat /proc/mounts | awk '{print $1}' | grep -q "$DEVNAME"
	then
		echo "111111111" >> /tmp/test.txt
		for i in $(seq 1 3)
		do
			echo "44444444" >> /tmp/test.txt
			if [ ! -d "$DIR" ]
			then
				echo "555555555" >> /tmp/test.txt
				mkdir -p $DIR
				break
			else
				echo "666666666" >> /tmp/test.txt
				# 无匹配 返回0 执行
				if ! cat /proc/mounts | awk '{print $2}' | grep -q "$DIR"
				then
					echo "---------" >> /tmp/test.txt
					WANGSHENG=`$MOUNT | awk '{print $2}' | grep "$DIR"`
					echo $WANGSHENG >> /tmp/test.txt
					echo "777777777" >> /tmp/test.txt
					break
				# 有屁配 返回非0 执行
				else
					echo "********" >> /tmp/test.txt
					WANGSHENG=`$MOUNT | awk '{print $2}' | grep "$DIR"`
					echo $WANGSHENG >> /tmp/test.txt
					echo "888888888" >> /tmp/test.txt
					DIR="/media/$name$i"
				fi
			fi
		done

		# mount the U-disk
		echo $DEVNAME >> /tmp/test.txt
		echo $DIR >> /tmp/test.txt
		$MOUNT -t auto -o async,relatime $DEVNAME $DIR > /tmp/log 2>&1
		if $MOUNT $DEVNAME $DIR
		then
			echo "delete1" >> /tmp/test.txt
			rmdir $DIR
		else
			echo "delete2" >> /tmp/test.txt
			rmdir $DIR
		fi
	else
		return
	fi
fi

