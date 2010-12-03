#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<stdio.h>

#include<iostream>

using namespace std;

#define BUF_SIZE 1024

typedef struct {
    long mtype;
    char mtext[BUF_SIZE];
} msg_info;

int create_msg_queue(int key)
{
    int msg_id;

    msg_id = msgget(key, IPC_CREAT|0660);

    if(msg_id == -1)
    {
        perror("msgget error");
        exit(-1);
    }

    return msg_id;
}

int send_message(int msg_id,int mtype,char* message)
{
    int result;
    msg_info MsgInfo;

    memset(&MsgInfo,0,sizeof(msg_info));
    MsgInfo.mtype = mtype;
    strcpy(MsgInfo.mtext, message);

    result = msgsnd(msg_id,&MsgInfo,strlen(message)+1,0);
    if(result == -1)
    {
        perror("Failed to send message");
    }

    return result;
}

int main(int argc,char* argv[])
{
    if (argc < 3)
    {
        printf("input key type\n");
        return 0;
    }
    int key = atoi(argv[1]);
    int mtype = atoi(argv[2]);
    
    int msg_id = create_msg_queue(key);
    send_message(msg_id,mtype,"Hello message queue");

    return 0;
}


