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

    string mmapfile;
    string sofile;
    int uploadtime_sec=60*5;

    while ((input = getopt (argc, argv, "m:r:s:")) != -1) 
    {
        if ( input == 'm' )
        {
            mmapfile = optarg;
            continue;
        }
        if ( input == 'r' )
        {
            uploadtime_sec = atoi(optarg);
            continue;
        }
        if ( input == 's' )
        {
            sofile = optarg;
            continue;
        }
    }
    if (mmapfile.empty() || sofile.empty() || uploadtime_sec == 0)
    {
        useage();
        return 2;
    }

    int ret;
    CFLSlaveWorker sworker;
    ret = sworker.SetMMapFile(mmapfile);
    printf("set mmapfile:%d\n",ret);
    ret = sworker.SetModuleFile(sofile);
    printf("set modulefile:%d\n",ret);
    sworker.Run();

    return 0;
}
