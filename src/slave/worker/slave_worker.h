/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        slave_worker.h
#  Description:     用来处理slave_worker的主逻辑
#  Version:         1.0
#  LastChange:      2010-11-28 18:41:51
#  History:         
=============================================================================*/
#ifndef _SLAVE_WORKER_H_
#define _SLAVE_WORKER_H_

#include <iostream>
#include <string>
#include <vector>

#include "fuload_commfunc.h"
#include "weight_route.h"
using namespace std;

typedef struct _stSWInput
{
    vector<string> vecParams;
    string strInputLine;
}StSWInput;
class CSlaveWorker 
{
    public:
        CSlaveWorker ();
        virtual ~CSlaveWorker ();

        /**
         * @brief   设置输入数据
         *
         * @param   strInputData    数据
         *
         * @return  0               succ
         *          else            fail
         */
        int SetInputData(const string& strInputData);

        /**
         * @brief   运行
         * 
         * @return  0               succ
         *          else            fail
         */
        int Run();

    private:
        /**
         * @brief   解析输入数据
         *
         * @param   strInputData    数据
         *
         * @return  0               succ
         *          else            fail
         */
        int parseInputData(const string& strInputData);
        /**
         * @brief   调用so，并统计时间
         *
         * @return  0               succ
         *          else            fail
         */
        int process(const StSWInput& swi);

    private:
        string m_InputData;
        CWeightRoute<StSWInput> m_SWInputRoute;
};

#endif
