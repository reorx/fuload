#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <gtest/gtest.h>
#include "tcp_client.h"
using namespace std;

class CMyClient:public CTcpClient
{
    public:
        virtual int handleInput(char* pBuf, int bufLen)
        {
            if (bufLen < 11)
            {
                return -1;
            }
            char allLenBuf[12];
            memcpy(allLenBuf,pBuf,11);
            allLenBuf[11]='\0';

            int allLen = atoi(allLenBuf);
            return allLen;
        }

    private:
        /* data */
};
TEST(suite_name,case_name)
{
    CMyClient client;
    client.Open("127.0.0.1",21567,500,true);

    char sendBuf[1024];
    char *ptrBuf = &sendBuf[0];
    int sendLen = 11+11+11+3;

    snprintf(ptrBuf,sizeof(sendBuf)-(ptrBuf-sendBuf),"%011d",sendLen);
    ptrBuf+=11;

    snprintf(ptrBuf,sizeof(sendBuf)-(ptrBuf-sendBuf),"%011d",1);
    ptrBuf+=11;

    snprintf(ptrBuf,sizeof(sendBuf)-(ptrBuf-sendBuf),"%011d",0);
    ptrBuf+=11;

    snprintf(ptrBuf,sizeof(sendBuf)-(ptrBuf-sendBuf),"zny");
    ptrBuf+=3;

    //printf("%s,%d\n",sendBuf,strlen(sendBuf));
    ASSERT_EQ(sendLen,strlen(sendBuf));
    ASSERT_STREQ("000000000360000000000100000000000zny",sendBuf);

    int ret = client.Send(sendBuf,sendLen);
    ASSERT_EQ(0,ret)<<client.GetErrMsg();
    
#if 1
    char * recvBuf = NULL;
    int recvLen = 0;

    ret = client.Recv(recvBuf,recvLen);
#else
    char recvBuf[1024];
    int maxLen =1024;
    int recvLen = 0;

    ret = client.Recv(recvBuf,maxLen,recvLen);
#endif

    ASSERT_EQ(0,ret)<<client.GetErrMsg();

    recvBuf[recvLen] = '\0';
    //printf("recv:%s,len:%d\n",recvBuf,recvLen);
    EXPECT_EQ(43,recvLen);
    EXPECT_STREQ("000000000430000000000100000000000zny|append",recvBuf);
}
