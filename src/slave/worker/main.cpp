#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <getopt.h>

#include "fl_slave_worker.h"

using namespace std;

void useage()
{
    printf("USEAGE:\n");
    printf("\t -i     inputFile\n");
    printf("\t -r     reportTime(sec)\n");
    printf("\t -s     soFile\n");
    printf("\t -m     msgQueueKey\n");
    printf("\t -l     limitspeed\n");
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

    while ((input = getopt (argc, argv, "i:r:s:m:l:")) != -1) 
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
    }

    int ret;
    CFLSlaveWorker sworker;
    ret = sworker.Init(param);
    if (ret)
    {
        printf("slave worker init error:%d\n",ret);
        return 0;
    }
    sworker.Run();

    return 0;
}
