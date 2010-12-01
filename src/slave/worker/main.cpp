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

    int ret;
    CFLSlaveWorker sworker;
    ret = sworker.SetModuleFile("../../module/libmodule.so");
    printf("set modulefile:%d\n",ret);
    sworker.Run();

    return 0;
}
