#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>

#include "fl_slave_worker.h"

using namespace std;
int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        printf("please input mmapfile\n");
        return 0;
    }
    int ret;
    CFLSlaveWorker sworker;
    ret = sworker.SetMMapFile(argv[1]);
    printf("set mmapfile:%d\n",ret);
    ret = sworker.SetModuleFile("../../module/libmodule.so");
    printf("set modulefile:%d\n",ret);
    sworker.Run();

    return 0;
}
