#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <getopt.h>

#include "fl_log.h"
#include "fl_slave_worker.h"

using namespace std;

void useage()
{
    printf("USEAGE:\n");
    printf("\t -i     inputFile\n");
    printf("\t -r     reportTime(sec)\n");
    printf("\t -s     soFile\n");
    printf("\t -m     msgQueueKey\n");
    printf("\t -l     limitSpeed\n");
    printf("\t -t     statFile\n");
    printf("\t -e     loglevel\n");
    printf("\t -g     logfile\n");
    printf("\t -z     logmaxsize\n");
    printf("\n");
}
int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        useage();
        return 1;
    }
    int input;

    StSWParam param;

    LogLevel iLogLevel = LM_ERROR;
    string strLogFile = "../log/worker";
    int iLogMaxSize = 104857600;//100M

    while ((input = getopt (argc, argv, "i:r:s:m:l:t:e:g:z:")) != -1) 
    {
        if ( input == 'i' )
        {
            param.inputFile = optarg;
            continue;
        }
        if ( input == 'r' )
        {
            param.reportTime_sec = atoi(optarg);
            continue;
        }
        if ( input == 's' )
        {
            param.moduleFile = optarg;
            continue;
        }
        if ( input == 'm' )
        {
            param.msgQKey= atoi(optarg);
            continue;
        }
        if ( input == 'l' )
        {
            param.limitSpeed= atoi(optarg);
            continue;
        }
        if ( input == 't' )
        {
            param.statFile= optarg;
            continue;
        }
        if ( input == 'e' )
        {
            iLogLevel = (LogLevel)atoi(optarg);
            continue;
        }
        if ( input == 'g' )
        {
            strLogFile = optarg;
            continue;
        }
        if ( input == 'z' )
        {
            iLogMaxSize = atoi(optarg);
            continue;
        }
    }
    string::size_type pos = strLogFile.rfind("/");
    if (pos == string::npos)
    {
        log_init(iLogLevel,"./",strLogFile.c_str(),iLogMaxSize);
    }
    else
    {
        log_init(iLogLevel,strLogFile.substr(0,pos).c_str(),strLogFile.substr(pos+1).c_str(),iLogMaxSize);
    }

    int ret;
    CFLSlaveWorker sworker;
    ret = sworker.Init(param);
    if (ret)
    {
        printf("slave worker init error:%d\n",ret);
        return 0;
    }
    error_log("worker start run...");
    sworker.Run();

    return 0;
}
