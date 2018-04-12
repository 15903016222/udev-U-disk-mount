#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>


#define MSG_KEY     0x680068 //定义IPC消息key

//定义IPC消息传送的内容
typedef struct _msgcontent{
	char szAction[512];   //USB产生的动作
	char szDevice[512];   //USB设备名称
	char mountPath[512];  //u盘挂载地址
}UMSGCONTENT;

typedef struct _umsg{
	long msgtype;
	UMSGCONTENT content;
}UMSG;

void send_message (int status, void *buf, int size);

int main(int argc, char *argv[]){
	char cmd[512] = {0};
	UMSG umsg = {0};
	int msgid;
	int status;

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
			if (!strncmp("add", umsg.content.szAction, 3)) {
				sprintf(cmd, "mount %s %s", 
							umsg.content.szDevice, umsg.content.mountPath);
				status = system(cmd);
				send_message (status, &umsg.content, sizeof (umsg));
			}
			else if (!strncmp("remove", umsg.content.szAction, 6)) {
				sprintf(cmd, "umount %s", umsg.content.mountPath);
				status = system(cmd);
				send_message (status, &umsg.content, sizeof (umsg));

				sprintf(cmd, "rmdir %s", umsg.content.mountPath);
				system(cmd);
			}
		}
	}

	return 0;
}

void send_message (int status, void *buf, int size) {
	if (-1 == status) {
		printf ("system ERROR \n");
		return;
	}
	else {
		printf ("status[%d] \n", status);
		if (WIFEXITED (status)) {
			if (0 == WEXITSTATUS(status)) { 
				printf("run shell script successfully.\n");  
			}  
			else {
				printf("run shell script fail, script exit code: %d\n",
							WEXITSTATUS(status));  
			}  
		}
		else {
			printf ("exit status = [%d]\n", WEXITSTATUS(status));
		}
	}

	return ;
}
