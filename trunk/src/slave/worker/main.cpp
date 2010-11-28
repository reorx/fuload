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

    ifstream fin;
    fin.open("fl_input.txt");

    string inputData;
    string strLine;
    while(getline(fin,strLine))
    {
        inputData.append(strLine);
        inputData.append("\n");
    }
    CFLSlaveWorker sworker;
    sworker.SetInputData(inputData);
    sworker.Run();

    return 0;
}
