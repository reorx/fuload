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

#include <dlfcn.h>

#include "fl_commfunc.h"
#include "fl_slave_input.h"
using namespace std;

typedef int (*FunPtrInit)();
typedef int (*FunPtrProcess)(const map<string,string>& mapParams,const string& strInputLine);
typedef int (*FunPtrFini)();

class CFLSlaveWorker 
{
    public:
        static void *SoObj;
    public:
        CFLSlaveWorker ();
        virtual ~CFLSlaveWorker ();

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
         * @brief   设置要加载的so的文件
         *
         * @param   moduleFile      文件
         *
         * @return  0               succ
         *          else            fail
         */
        int SetModuleFile(const string& moduleFile);

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
        int process(const StSWInput& swi);

    private:
        CFLSlaveInput m_SlaveInput;

        FunPtrInit m_funPtrInit;
        FunPtrProcess m_funPtrProcess;
        FunPtrFini m_funPtrFini;
};

#endif
