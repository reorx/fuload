/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        tcp_client.h
#  Description:     封装的linux tcpclient
#  Version:         1.0
#  LastChange:      2010-11-08 13:37:49
#  History:         
=============================================================================*/
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_
#include <error.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdarg>

#define TCPCLIERR_MAXSIZE   1024

class CTcpClient 
{
    public:
        CTcpClient ();
        virtual ~CTcpClient ();

        /**
         * @brief   初始化必要的参数，并不会真正connect
         *
         * @param   ip          ip
         * @param   port        端口
         * @param   timeoutMs   超时时间
         * @param   keepCnt     是否是长连接
         *
         * @return  0           succ
         *          else        fail
         */
        int Open(const char* ip, unsigned short port, int timeoutMs, bool keepCnt);

        /**
         * @brief   发送数据
         *
         * @param   pBuf        数据指针
         * @param   bufLen      数据长度
         *
         * @return  0           succ
         *          else        fail
         */
        int Send(char* pBuf, int bufLen);

        /**
         * @brief   接收数据，用户不用自己分配pBuf，也不用关心数据最大长度
         *
         * @param   pBuf        数据指针
         * @param   bufLen      数据长度
         *
         * @return  0           succ
         *          else        fail
         */
        int Recv(char*& pBuf, int& bufLen);

        /**
         * @brief   接收数据，用户需要自己预分配好pBuf，并规定好最大接受长度
         *
         * @param   pBuf        调用者预分配好的数据指针
         * @param   bufSize     最大能接受的长度
         * @param   bufLen      实际接受的数据长度
         *
         * @return  0           succ
         *          else        fail
         */
        int Recv(char* pBuf, int bufSize, int& bufLen);

        /**
         * @brief   关闭socket
         *
         * @return  0           succ
         *          else        fail
         */
        int Close();

        /**
         * @brief   获取错误信息
         *
         * @return  错误信息指针
         */
        char* GetErrMsg();

        /**
         * @brief   设置内部buf的size
         *
         * @param   bufSize     最大size
         *
         * @return  0           succ
         *          else        fail
         */
        int SetRecvBufSize(int bufSize);

    protected:
        /**
         * @brief   建立tcp连接，当一次send或者重连的时候会被调用
         *
         * @return  >0          socketfd
         *          else        fail
         */
        int asyncConnect();

        /**
         * @brief   检查描述符是否可读
         *
         * @param   fd          描述符
         * @param   timeoutMs   超时时间
         *
         * @return  0           succ
         *          else        fail
         */
        int detectSingleRead(int fd, int timeoutMs);

        /**
         * @brief   继续接收总共为allLen的数据
         *
         * @param   pBuf        用来接收的buf
         * @param   allLen      要接收的长度
         *
         * @return  >=0         实际接收到的长度
         *          else        fail
         */
        int recvLeft(char* pBuf, int allLen);

    protected:
        /**
         * @brief   在Recv调用中会调用该方法来判断总共要接受的长度，子类继承后必须重写
         *
         * @param   pBuf        数据指针
         * @param   bufLen      已经接收的数据长度
         *
         * @return  >0          总共要接收的数据长度
         *          else        出错
         */
        virtual int handleInput(char* pBuf, int bufLen);

        /**
         * @brief   打印log的类，继承后可以重写为打印log
         *
         * @param   logFmt      参数
         * @param   ...
         *
         */
        virtual void logErrMsg(const char* logFmt, ...);

    protected:
        //维护的recvbuf
        char* m_RecvBuf;
        int m_RecvBufSize;

        char m_Ip[20];
        unsigned short m_Port;
        int m_TimeoutMs;
        bool m_KeepCnt;

        int m_TimeLeftMs;

        char m_ErrMsg[TCPCLIERR_MAXSIZE];

        int m_SockFd;
};
#endif
