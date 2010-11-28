/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        slave_worker.cpp
#  Description:     用来处理slave_worker的主逻辑
#  Version:         1.0
#  LastChange:      2010-11-28 17:57:25
#  History:         
=============================================================================*/
#include "fl_slave_worker.h"

CFLSlaveWorker::CFLSlaveWorker()
{
}

CFLSlaveWorker::~CFLSlaveWorker()
{
}
int CFLSlaveWorker::AddInputData(const string& strInputData)
{
    return m_SlaveInput.AddInputData(strInputData);
}
int CFLSlaveWorker::Run()
{
    int ret;
    while(1)
    {
        StSWInput swi;
        ret = m_SlaveInput.Alloc(swi);
        if (ret)
        {
            printf("error alloc:%d\n",ret);
            break;
        }
        ret = process(swi);
        if (ret)
        {
            printf("process error:%d\n",ret);
            continue;
        }
    }
    return 0;
}
int CFLSlaveWorker::process(const StSWInput& swi)
{
    const map<string,string> &mapParams = swi.mapParams;
    const string& strInputLine = swi.strInputLine;
    foreach(mapParams,it)
    {
        printf("%s=%s&",it->first.c_str(),it->second.c_str());
    }
    printf("\n");
    //printf("line:%s\n",strInputLine.c_str());
    return 0;
}
