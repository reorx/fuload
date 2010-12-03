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
    printf("\t -i     inputfile\n");
    printf("\t -r     reporttime(sec)\n");
    printf("\t -s     sofile\n");
    printf("\t -m     msgqueue\n");
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

    while ((input = getopt (argc, argv, "i:r:s:m:")) != -1) 
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
            param.msgQueueName = optarg;
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
