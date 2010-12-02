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
    printf("\t -h     host\n");
    printf("\t -p     port\n");
    printf("\t -t     timeout_ms\n");
    printf("\t -m     mmapfile\n");
    printf("\t -r     reporttime(sec)\n");
    printf("\t -s     sofile\n");
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

    while ((input = getopt (argc, argv, "m:r:s:h:p:t:")) != -1) 
    {
        if ( input == 'h' )
        {
            param.ip = optarg;
            continue;
        }
        if ( input == 'p' )
        {
            param.port = atoi(optarg);
            continue;
        }
        if ( input == 't' )
        {
            param.timeout_ms = atoi(optarg);
            continue;
        }
        if ( input == 'm' )
        {
            param.mmapFile = optarg;
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
