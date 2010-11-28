/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        slave_worker.h
#  Description:     用来处理slave_worker的主逻辑
#  Version:         1.0
#  LastChange:      2010-11-28 17:56:00
#  History:         
=============================================================================*/
#ifndef _SLAVE_WORKER_H_
#define _SLAVE_WORKER_H_

#include <iostream>
#include <string>
#include <vector>

#include "fuload_commfunc.h"
using namespace std;

typedef struct _stSWInput
{
    vector<string> vecParams;
    string strArgLine;
}StSWInput;
class CSlaveWorker 
{
    public:
        CSlaveWorker ();
        virtual ~CSlaveWorker ();

        int SetInputData(const string& strInputData);

    private:
        int parseInputData(const string& strInputData);

    private:
        string m_InputData;
        vector<StSWInput> m_vecSWInputs;
};

#endif
