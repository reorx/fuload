#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "fl_log.h"
using namespace std;
int main(int argc, const char *argv[])
{
    log_init(LM_DEBUG,"./log/","worker",200);
    for (int i = 0; i < 100; i++)
    {
        debug_log("debug is : %d",i);
        error_log("error is : %d",i);
    }
    return 0;
}
