/*=============================================================================
#  Author:          DanteZhu - http://www.vimer.cn
#  Email:           dantezhu@vip.qq.com
#  FileName:        appwork_stat.cpp
#  Description:     
#  Version:         1.0
#  LastChange:      2010-10-09 10:27:07
#  History:         
=============================================================================*/
#include <errno.h>
#include <error.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <getopt.h>
#include "stat.h"
#include "stat_def.h"

int _clear_flag = 0;
int _loop_flag = 0;
int main(int argc,char** argv)
{
    int input;
    std::string _stat_file;
    int num=-1;
    while ((input = getopt (argc, argv, "f:d:cl")) != -1) 
    {
        if ( input == 'd' )
        {
            num = atoi(optarg);
            continue;
        }
        if ( input == 'c' )
        {
            _clear_flag = 1;
            continue;
        }
        if ( input == 'l' )
        {
            _loop_flag = 1;
            continue;
        }
        if ( input == 'f' )
        {
            _stat_file = optarg;
            continue;
        }
    }

    if ( _stat_file.length() == 0 )
    {
        printf("invalid input,please input \"-f stat_file\" \n");
        return -1;
    }
    //printf("show num:%d\n",num);

    CStatInfo _stat;
    if ( _stat.Init(_stat_file.c_str(),&stat_desc[0],STAT_OVER) < 0 )
    {
        printf("open stat error\n");
        return 0;
    }

    while(1)
    {
        _stat.ShowStatInfo(num);
        if ( _clear_flag )
        {
            _stat.ResetStat();
        }
        printf("\n");
        printf(".............................................\n");
        printf("\n");
        sleep(1);

        if ( !_loop_flag ) break;
    }

    return 0;
}
