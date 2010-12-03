/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        fl_slave_msg.h
#  Description:     消息队列操作
#  Version:         1.0
#  LastChange:      2010-12-03 14:40:19
#  History:         
=============================================================================*/
#ifndef _FL_SLAVE_MSG_H_
#define _FL_SLAVE_MSG_H_

#define MSGQUEUE_BUFSIZE 4096
#define MSGQUEUE_MTYPE 1
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

typedef struct _MsgInfo
{
    long mtype;
    char mtext[MSGQUEUE_BUFSIZE];
} MsgInfo;

class CSlaveMsg 
{
    public:
        CSlaveMsg ()
        {
            m_msg_id = -1;
        }
        virtual ~CSlaveMsg ()
        {}

        int Init(int key)
        {
            int msg_id = create_msg_queue(key);
            if (msg_id < 0)
            {
                return -1;
            }
            m_msg_id = msg_id;
            return 0;
        }
        int send(const string& message)
        {
            if (m_msg_id < 0)
            {
                return -2;
            }
            int ret;
            MsgInfo msginfo;

            memset(&msginfo,0,sizeof(MsgInfo));
            msginfo.mtype = MSGQUEUE_MTYPE;
            snprintf(msginfo.mtext,sizeof(msginfo.mtext),"%s",message.c_str());

            //ret = msgsnd(m_msg_id,&msginfo,message.size()+1,0);
            ret = msgsnd(m_msg_id,&msginfo,message.size()+1,IPC_NOWAIT);
            if(ret == -1)
            {
                return -1;
            }

            return ret;
        }

    private:
        int create_msg_queue(int key)
        {
            int msg_id;

            msg_id = msgget(key, IPC_CREAT|0660);

            if(msg_id == -1)
            {
                return -1;
            }

            return msg_id;
        }
    private:
        int m_msg_id;
};
#endif
