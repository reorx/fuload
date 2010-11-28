/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        fl_slave_input.cpp
#  Description:     
#  Version:         1.0
#  LastChange:      2010-11-28 21:11:25
#  History:         
=============================================================================*/
#include "fl_slave_input.h"

int CFLSlaveInput::AddInputData(const string& strInputData)
{
    m_LastInputData = strInputData;
    return parseInputData(m_LastInputData);
}
int CFLSlaveInput::parseInputData(const string& strInputData)
{
    string tempInputData = strInputData;
    tempInputData = CFLCommFunc::replace_all(tempInputData,"\r\n","\n");

    vector<string> vecLines;
    CFLCommFunc::SplitString(tempInputData,"\n",vecLines);

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
            continue;
        }

        tempLine.erase(tempLine.find_last_not_of(" ")+1); 
        tempLine.erase(0,tempLine.find_first_not_of(" ")); 

        if (tempLine.size()<=0)
        {
            continue;
        }

        StSWInput swi;
        vector<string> vecParams;
        CFLCommFunc::SplitString(tempLine," ",vecParams);

        swi.vecParams = vecParams;
        swi.strInputLine = tempLine;
        m_SWInputRoute.add(swi,loopNum);
    }
    return 0;
}
int CFLSlaveInput::Alloc(StSWInput& node)
{
    return m_SWInputRoute.alloc(node);
}
