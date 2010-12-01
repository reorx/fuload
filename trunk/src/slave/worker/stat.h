/*=============================================================================
#  Author:          DanteZhu - http://www.vimer.cn
#  Email:           dantezhu@vip.qq.com
#  FileName:        stat.h
#  Description:     
#  Version:         1.0
#  LastChange:      2010-10-08 15:44:35
#  History:         
=============================================================================*/
#ifndef _STAT_INFO_H_
#define _STAT_INFO_H_
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <atomic.h>
#include <iostream>
#include <string>
using namespace std;

class CStatInfo
{
    public:
        CStatInfo();
        ~CStatInfo();

        int Init(const char* file_name,const char * const stat_desc[],int stat_num);
        void AddCount(int index);
        int  SetCount(int index,int value);
        int  GetCount(int index);
        void ResetStat();
        void ShowStatInfo(int num=-1);
    public:
        static CStatInfo *_ins;
        static CStatInfo * Ins();
    private:
        atomic_t*   _stat_buf;
        const char * const * _stat_desc;
        int         _stat_num;
};

#endif
