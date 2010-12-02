#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "timecheck.h"
using namespace std;

int main(int argc, const char *argv[])
{
    CTimeCheck c;
    c.Init(1000);
    while(1)
    {
        if (c.Check())
        {
            printf("ok\n");
        }
        else
        {
            printf("no\n");
        }
        usleep(10000);
    }
    return 0;
}
