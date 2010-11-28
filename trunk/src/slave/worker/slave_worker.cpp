/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        slave_worker.cpp
#  Description:     用来处理slave_worker的主逻辑
#  Version:         1.0
#  LastChange:      2010-11-28 17:57:25
#  History:         
=============================================================================*/
#include "slave_worker.h"

CSlaveWorker::CSlaveWorker()
{
}

CSlaveWorker::~CSlaveWorker()
{
}
int CSlaveWorker::SetInputData(const string& strInputData)
{
    m_InputData = strInputData;
    return parseInputData(m_InputData);
}
int CSlaveWorker::Run()
{
    int ret;
    while(1)
    {
        StSWInput swi;
        ret = m_SWInputRoute.alloc(swi);
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
int CSlaveWorker::parseInputData(const string& strInputData)
{
    string tempInputData = strInputData;
    tempInputData = replace_all(tempInputData,"\r\n","\n");

    vector<string> vecLines;
    SplitString(tempInputData,"\n",vecLines);

    int loopNum = 1;
    foreach(vecLines,it)
    {
        string tempLine = *it;

        if (tempLine.size()<=0)
        {
            continue;
        }

        if(tempLine[0] != ' ' && tempLine[0] != '\t')//证明是suite说明
        {
            size_t begin_index = tempLine.find("[");
            if (begin_index == string::npos)
            {
                return -1;
            }
            size_t end_index = tempLine.find("]",begin_index);
            if (end_index == string::npos)
            {
                return -2;
            }
            if (begin_index == end_index)
            {
                return -3;
            }

            string substr = tempLine.substr(begin_index+1,end_index-begin_index-1);
            loopNum = atoi(substr.c_str());
        }

        tempLine.erase(tempLine.find_last_not_of(" ")+1); 
        tempLine.erase(0,tempLine.find_first_not_of(" ")); 

        StSWInput swi;
        vector<string> vecParams;
        SplitString(tempLine," ",vecParams);

        swi.vecParams = vecParams;
        swi.strInputLine = tempLine;
        m_SWInputRoute.add(swi,loopNum);
    }
    return 0;
}
int CSlaveWorker::process(const StSWInput& swi)
{
    const vector<string> &vecParams = swi.vecParams;
    const string& strInputLine = swi.strInputLine;
    foreach(vecParams,it)
    {
        printf("%s,",(*it).c_str());
    }
    printf("\nline:%s\n",strInputLine.c_str());
    return 0;
}
