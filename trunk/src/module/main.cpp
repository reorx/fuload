/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        main.cpp
#  Description:     fuload so编写实例代码
#  Version:         1.0
#  LastChange:      2010-11-28 17:45:34
#  History:         
=============================================================================*/
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <error.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
using namespace std;

/**
 * @brief   第一次进入so时，需要做的初始化工作，只会执行一次。
 *
 * @return  0               succ
 *          else            fail
 */
extern "C" int fuload_handle_init()
{
    printf("%s\n",__FUNCTION__);
    return 0;
}

/**
 * @brief   业务逻辑，每次进入
 *
 * @param   mapParams       将输入数据按照空格拆分之后的vector
 *
 * @return  0               succ
 *          else            返回值,会用来做统计
 */
extern "C" int fuload_handle_process(const map<string,string>& mapParams)
{
    for(typeof(mapParams.begin()) it = mapParams.begin(); it != mapParams.end(); ++it)
    {
        printf("%s=%s&",it->first.c_str(),it->second.c_str());
        usleep(100);
    }
    printf("\n");
    return 0;
}

/**
 * @brief   so结束时的收尾工作，只会调用一次。一般不用编写
 *
 * @return  0               succ
 *          else            fail
 */
extern "C" int fuload_handle_fini()
{
    printf("%s\n",__FUNCTION__);
    return 0;
}
