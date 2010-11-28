/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        fuload_commdef.h
#  Description:     定义
#  Version:         1.0
#  LastChange:      2010-11-28 18:37:38
#  History:         
=============================================================================*/
#ifndef _FULOAD_COMMDEF_H_
#define _FULOAD_COMMDEF_H_
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

#define foreach(container,it) \
    for(typeof((container).begin()) it = (container).begin();it!=(container).end();++it)

#endif
