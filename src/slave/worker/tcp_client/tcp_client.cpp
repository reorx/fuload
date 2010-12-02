/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        tcp_client.cpp
#  Description:     封装的linux tcpclient
#  Version:         1.0
#  LastChange:      2010-11-08 13:37:36
#  History:
=============================================================================*/
#include "tcp_client.h"
CTcpClient::CTcpClient()
{
    m_Ip[0]='\0';
    m_Port = 0;
    m_TimeLeftMs = m_TimeoutMs = 0;
    m_KeepCnt = true;
    m_SockFd = -1;
    m_ErrMsg[0]='\0';

    m_RecvBufSize = 1024;
    m_RecvBuf = (char*)malloc(m_RecvBufSize);;
    if(!m_RecvBuf)
    {
        m_RecvBufSize = 0;
    }
}
CTcpClient::~CTcpClient()
{
    if (m_RecvBuf)
    {
        free(m_RecvBuf);
        m_RecvBuf = NULL;
    }
    Close();
}
int CTcpClient::Open(const char* ip, unsigned short port, int timeoutMs, bool keepCnt)
{
    if (!ip)
    {
        logErrMsg("open err,ip is NULL");
        return -1;
    }
    snprintf(m_Ip,sizeof(m_Ip),"%s",ip);
    m_Port = port;
    if (timeoutMs >= 0)
    {
        m_TimeLeftMs = m_TimeoutMs = timeoutMs;
    }
    else
    {
        logErrMsg("open err,timeoutMs should >=0,timeoutMs:%d",timeoutMs);
        return -2;
    }
    m_KeepCnt = keepCnt;
    return 0;
}
int CTcpClient::Send(char* pBuf, int bufLen)
{
    int ret;
    m_TimeLeftMs = m_TimeoutMs;//重新赋总的超时时间
    ret = asyncConnect();
    if (ret)
    {
        return -1;
    }
    int nsend =0;
    while(1)
    {
        ret = send(m_SockFd,pBuf+nsend,(bufLen-nsend),0);
        if (ret <= 0)
        {
            if ( errno == EINTR || errno == EAGAIN ) //信号中断，或者缓冲区满
                continue;
            else
                break;
        }
        nsend += ret;
        if ( nsend >= bufLen )
            break;
    }
    if (nsend != bufLen)
    {
        logErrMsg("send needlen:%d sendlen:%d", bufLen,nsend);
        Close();
        return -2;
    }
    return 0;
}
int CTcpClient::Recv(char*& pBuf, int& bufLen)
{
    int ret = detectSingleRead(m_SockFd,m_TimeLeftMs);
    if (ret <= 0)
    {
        logErrMsg("detectSingleRead ret=%d",ret);
        return -1;
    }
    ret = recv(m_SockFd, m_RecvBuf, m_RecvBufSize, 0);
    if (ret <= 0)
    {
        logErrMsg("recv ret:%d",ret);
        return -2;
    }
    int recvLen = ret;

    ret = handleInput(m_RecvBuf,recvLen);
    if (ret <= 0)
    {
        logErrMsg("handleInput ret:%d", ret);
        return -3;
    }

    int allLen = ret;
    if(allLen > m_RecvBufSize)
    {
        ret = SetRecvBufSize(allLen);
        if(ret)
        {
            return -4;
        }
    }
    ret = recvLeft(m_RecvBuf+recvLen,allLen-recvLen);
    if (ret<0)
    {
        return -5;
    }
    if (ret != (allLen-recvLen))
    {
        logErrMsg("recvLeft needLen:%d,recvLen:%d", allLen-recvLen,ret);
        return -6;
    }

    bufLen = allLen;
    pBuf = m_RecvBuf;

    if (!m_KeepCnt)
    {
        Close();
    }

    return 0;
}
int CTcpClient::Recv(char* pBuf, int bufSize, int& bufLen)
{
    int ret = detectSingleRead(m_SockFd,m_TimeLeftMs);
    if (ret <= 0)
    {
        logErrMsg("detectSingleRead ret=%d",ret);
        return -1;
    }
    ret = recv(m_SockFd, pBuf, bufSize, 0);
    if (ret <= 0)
    {
        logErrMsg("recv ret:%d",ret);
        return -2;
    }
    int recvLen = ret;

    ret = handleInput(pBuf,recvLen);
    if (ret <= 0)
    {
        logErrMsg("handleInput ret:%d", ret);
        return -3;
    }

    int allLen = ret;
    if(allLen > bufSize)
    {
        logErrMsg("allLen:%d,cursize:%d\n", allLen,bufSize);
        return -4;
    }

    ret = recvLeft(pBuf+recvLen,allLen-recvLen);
    if (ret<0)
    {
        return -5;
    }
    if (ret != (allLen-recvLen))
    {
        logErrMsg("recvLeft needLen:%d,recvLen:%d", allLen-recvLen,ret);
        return -6;
    }

    bufLen = recvLen;

    if (!m_KeepCnt)
    {
        Close();
    }

    return 0;
}
int CTcpClient::Close()
{
    if (m_SockFd > 0)
    {
        close(m_SockFd);
        m_SockFd = -1;
    }
    return 0;
}
char* CTcpClient::GetErrMsg()
{
    return m_ErrMsg;
}
int CTcpClient::SetRecvBufSize(int bufSize)
{
    if (bufSize <= m_RecvBufSize)
    {
        return 0;
    }
    m_RecvBuf = (char*)realloc(m_RecvBuf,bufSize);
    if(!m_RecvBuf)
    {
        logErrMsg("realloc,newsize:%d,oldsize:%d", bufSize,m_RecvBufSize);
        return -1;
    }
    m_RecvBufSize = bufSize;

    return 0;
}
//=============================================================================

int CTcpClient::asyncConnect()
{
    int ret;
    if ( m_SockFd > 0 )
    {
        ret = detectSingleRead(m_SockFd,0);
        if ( ret > 0 )
        {
            //先关闭,再重新连接
            Close();
        }
        else
        {
            return 0;
        }
    }
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(m_Port);
    if (inet_pton(AF_INET, m_Ip, &serv_addr.sin_addr) <= 0)
    {
        logErrMsg("inet_pton set ip error:%s",m_Ip);
        return -1;
    }

    m_SockFd = socket(AF_INET, SOCK_STREAM, 0);
    if ( m_SockFd < 0 )
    {
        logErrMsg("create socket error:%s", strerror(errno));
        return -2;
    }

    //设置非阻塞
    int val = fcntl(m_SockFd, F_GETFL, 0);
    if (val == -1)
    {
        Close();
        logErrMsg("fcntl get error,val:%d",val);
        return -3;
    }

    if (fcntl(m_SockFd, F_SETFL, val | O_NONBLOCK | O_NDELAY) == -1)
    {
        Close();
        logErrMsg("fcntl set non-block err");
        return -4;
    }

    ret = connect(m_SockFd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret < 0)
    {
        if(errno != EINPROGRESS)
        {
            Close();
            logErrMsg("conncet ret:%d,error:%s",ret,strerror(errno));
            return -5;
        }
        else
        {
            struct pollfd conncet_client[1];
            int nfd = 1;
            memset(&conncet_client[0],0,sizeof(pollfd));
            conncet_client[0].fd = m_SockFd;
            conncet_client[0].events = POLLIN | POLLOUT;

            struct timeval m_tpstart,m_tpend;
            gettimeofday(&m_tpstart,NULL);

            int poll_timeout = m_TimeLeftMs;
            ret = ::poll(conncet_client, nfd, poll_timeout);
            if ( ret <= 0 )
            {
                Close();
                logErrMsg("poll ret:%d error:%s",ret,strerror(errno));
                return -6;
            }
            else
            {
                if ((conncet_client[0].revents & POLLIN) || (conncet_client[0].revents & POLLOUT))
                {
                    int error;
                    int len = sizeof(error);
                    int bok = getsockopt(m_SockFd, SOL_SOCKET, SO_ERROR, &error,(socklen_t*)&len);
                    if (bok < 0)
                    {
                        Close();
                        logErrMsg("getsockopt ret:%d error:%s", bok,strerror(errno));
                        return -7;
                    }
                    else if (error)
                    {
                        Close();
                        logErrMsg("getsockopt ret:%d error:%s", bok,strerror(errno));
                        return -8;
                    }
                }
                else if ((conncet_client[0].revents & POLLERR) ||
                        (conncet_client[0].revents & POLLHUP) ||
                        (conncet_client[0].revents & POLLNVAL))
                {
                    Close();
                    logErrMsg("poll event is err,hup,nval");
                    return -9;
                }
            }

            gettimeofday(&m_tpend,NULL);
            m_TimeLeftMs -= (1000*(m_tpend.tv_sec-m_tpstart.tv_sec)+(m_tpend.tv_usec-m_tpstart.tv_usec)/1000);
            if (m_TimeLeftMs < 0)
            {
                m_TimeLeftMs = 0;
            }

            return 0;
        }
    }
    return 0;
}
int CTcpClient::detectSingleRead(int fd, int timeoutMs)
{
    struct pollfd conncet_client[1];
    int nfd = 1;
    memset(&conncet_client[0],0,sizeof(pollfd));
    conncet_client[0].fd = fd;
    conncet_client[0].events = POLLIN ;

    struct timeval m_tpstart,m_tpend;
    gettimeofday(&m_tpstart,NULL);

    int ret = poll(conncet_client, nfd, timeoutMs);

    gettimeofday(&m_tpend,NULL);
    m_TimeLeftMs -= (1000*(m_tpend.tv_sec-m_tpstart.tv_sec)+(m_tpend.tv_usec-m_tpstart.tv_usec)/1000);
    if (m_TimeLeftMs < 0)
    {
        m_TimeLeftMs = 0;
    }

    return ret;
}
int CTcpClient::recvLeft(char* pBuf, int allLen)
{
    int recvLen = 0;
    int iRemain = allLen - recvLen;
    int ret;

    while (iRemain > 0)
    {
        ret = detectSingleRead(m_SockFd,m_TimeLeftMs);
        if(ret<=0)
        {
            logErrMsg("detectSingleRead ret=%d",ret);
            return -5;
        }

        ret = recv(m_SockFd, pBuf + recvLen, iRemain, 0);
        if (ret <= 0)
        {
            if ( errno == EINTR || errno == EAGAIN ) //信号中断，或者缓冲区满
                continue;
            else
                break;
        }
        recvLen += ret;
        iRemain -= ret;
    }
    return recvLen;
}

int CTcpClient::handleInput(char* pBuf, int bufLen)
{
    return bufLen;
}
void CTcpClient::logErrMsg(const char* logFmt, ...)
{
    int headLen = snprintf(m_ErrMsg,sizeof(m_ErrMsg),
            "ERROR [%s][%u] ip:%s,port:%u,timeoutMs:%u,timeleftMs:%u,socketfd:%d ",
            __FILE__,__LINE__,m_Ip,m_Port,m_TimeoutMs,m_TimeLeftMs,m_SockFd);
    va_list ap;
    va_start(ap, logFmt);
    vsnprintf(m_ErrMsg+headLen,sizeof(m_ErrMsg)-headLen,logFmt,ap);
    va_end(ap);
}
