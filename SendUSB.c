#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>


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

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Usage:%s <mount path>\n", argv[0]);
		return 0;
	}
    printf("USB RESPONSE!\n");
    char *pDevice,*pAction, *pMountDir;
    UMSG umsg;
    int msgid;
    
	if((msgid = msgget(MSG_KEY,IPC_CREAT|0666))==-1) //取得IPC管道
		return 1;             

    pAction = getenv("ACTION"); //取得udev产生的动作，
    pDevice = getenv("DEVNAME");//取得设备名称如/dev/sda
    pMountDir = argv[1];        //取得挂载挂载目录
   
	umsg.msgtype = 1;
	strcpy(umsg.content.mountAction,pAction);
    strcpy(umsg.content.mountDevice,pDevice);
    strcpy(umsg.content.mountPath,pMountDir);
    
	msgsnd(msgid,&umsg,sizeof(UMSGCONTENT),IPC_NOWAIT);
    
	return 0;
}
