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
#include <fstream>

#include <dlfcn.h>
#include <sys/mman.h>
#include <signal.h>

#include <json/json.h>

#include "speedlimit.h"

#include "fl_commfunc.h"
#include "fl_slave_input.h"
#include "fl_slave_reporter.h"

using namespace std;

typedef struct _st_swparam
{
    int reportTime_sec;

    string inputFile;
    string moduleFile;
    int msgQKey;
    int limitSpeed;
    _st_swparam()
    {
        reportTime_sec = 30;
        msgQKey = -1;
        limitSpeed = -1;//全速
    }
}StSWParam;

typedef int (*FunPtrInit)();
typedef int (*FunPtrProcess)(map<string,string>& mapParams);
typedef int (*FunPtrFini)();

class CFLSlaveWorker 
{
    public:
        static void *SoObj;
    public:
        CFLSlaveWorker ();
        virtual ~CFLSlaveWorker ();

        /**
         * @brief   初始话函数
         *
         * @param   param       参数
         *
         * @return  0           succ
         *          else        fail
         */
        int Init(const StSWParam& param);

        /**
         * @brief   运行
         * 
         * @return  0               succ
         *          else            fail
         */
        int Run();

    private:
        /**
         * @brief   调用so，并统计时间
         *
         * @return  0               succ
         *          else            fail
         */
        int process(StSWInput& swi);

        /**
         * @brief   设置要加载的so的文件
         *
         * @param   moduleFile      文件
         *
         * @return  0               succ
         *          else            fail
         */
        int setModuleFile(const string& moduleFile);
        /**
         * @brief   设置输入数据
         *
         * @param   inputFile       输入文件
         *
         * @return  0               succ
         *          else            fail
         */
        int setInputData(const string& inputFile);

    private:
        CFLSlaveInput m_SlaveInput;

        FunPtrInit m_funPtrInit;
        FunPtrProcess m_funPtrProcess;
        FunPtrFini m_funPtrFini;

        string m_mmapFile;

        StSWParam m_SWParam;

        CSlaveReporter m_reporter;

        CSpeedLimit m_SpeedLimit;
};

#endif
