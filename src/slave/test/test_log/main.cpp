#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "fl_log.h"
using namespace std;
int main(int argc, const char *argv[])
{
    log_init(LM_ERROR,"./","log",200);
    for (int i = 0; i < 100; i++)
    {
        error_log("error : %d",i);
    }
    return 0;
}
