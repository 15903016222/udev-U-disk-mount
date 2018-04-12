#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>


#define MSG_KEY     0x780078 //定义IPC消息key

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

int main(int argc, char *argv[]){
	char cmd[512] = {0};
    UMSG umsg = {0};
    int msgid;
    
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
       			system(cmd);
       		
				sprintf(cmd, "echo %s >> /tmp/receive.txt", umsg.content.szDevice);
       			system(cmd);
       			sprintf(cmd, "echo %s >> /tmp/receive.txt", umsg.content.szAction);
       			system(cmd);
       			sprintf(cmd, "echo %s >> /tmp/receive.txt", umsg.content.mountPath);
       			system(cmd);
       		}
       		else if (!strncmp("remove", umsg.content.szAction, 6)) {
       			sprintf(cmd, "umount %s", umsg.content.mountPath);
       			system(cmd);
       			sprintf(cmd, "rmdir %s", umsg.content.mountPath);
       			system(cmd);
       			
				sprintf(cmd, "echo %s >> /tmp/receive.txt", umsg.content.szDevice);
       			system(cmd);
       			sprintf(cmd, "echo %s >> /tmp/receive.txt", umsg.content.szAction);
       			system(cmd);
       			sprintf(cmd, "echo %s >> /tmp/receive.txt", umsg.content.mountPath);
       			system(cmd);
       		}
       	}
	}

    return 0;
}
