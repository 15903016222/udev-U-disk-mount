#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

#define MSG_KEY     0x680068 //定义IPC消息key

//定义IPC消息传送的内容
typedef struct _msgcontent{
	char mountAction[512];   //USB产生的动作
	char mountDevice[512];   //USB设备名称
	char mountPath[512];  //u盘挂载地址
}UMSGCONTENT;

typedef struct _umsg{
	long msgtype;
	UMSGCONTENT content;
}UMSG;

char cmd[512] = {0};
void send_message (int status, int fd, void *buf, int size);

int main(int argc, char *argv[]){
	UMSG umsg = {0};
	int msgid;
	int status;
	int fifofd;

	mkfifo ("/tmp/.mountFifo", 0777);
	fifofd = open ("/tmp/.mountFifo", O_RDWR);

	if ((msgid = msgget(MSG_KEY,IPC_CREAT|0666))==-1) //取得IPC管道
	  return 1;
	while (1) {
		memset (&umsg, 0, sizeof (umsg));
		memset (&cmd, 0, sizeof (cmd));
		int res = msgrcv(msgid, (void *)&umsg, sizeof(UMSGCONTENT), 1, 0);
		if (-1 == res) {
			printf("recive ERROR\n");
			return -1;
		}
		else {
			printf("recive success\n");
			if (!strncmp("add", umsg.content.mountAction, 3)) {
				sprintf(cmd, "ntfs-3g %s %s || mount %s %s", 
							umsg.content.mountDevice, 
							umsg.content.mountPath, 
							umsg.content.mountDevice, 
							umsg.content.mountPath);
				status = system(cmd);
				send_message (status, fifofd, &umsg.content, sizeof (umsg.content));
			}
			else if (!strncmp("remove", umsg.content.mountAction, 6)) {
				sprintf(cmd, "umount %s", umsg.content.mountPath);
				status = system(cmd);
				send_message (status, fifofd, &umsg.content, sizeof (umsg.content));

				sprintf(cmd, "rmdir %s", umsg.content.mountPath);
				system(cmd);
			}
		}
	}

	return 0;
}

void send_message (int status, int fd, void *buf, int size) {
	if (-1 == status) {
		printf ("system ERROR \n");
		sprintf(cmd, "rmdir %s", ((UMSGCONTENT *)buf)->mountPath);
		system(cmd);
		return;
	}
	else {
		if (WIFEXITED (status)) {
			if (0 == WEXITSTATUS(status)) { 
				write (fd, buf, size);
			}  
			else {
				sprintf(cmd, "rmdir %s", ((UMSGCONTENT *)buf)->mountPath);
				system(cmd);
			}
		}
		else {
			sprintf(cmd, "rmdir %s", ((UMSGCONTENT *)buf)->mountPath);
			system(cmd);
		}
	}

	return ;
}
