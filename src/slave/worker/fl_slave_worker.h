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
#include <sys/mman.h>
#include <signal.h>

#include "fl_commfunc.h"
#include "fl_slave_input.h"
using namespace std;

typedef int (*FunPtrInit)();
typedef int (*FunPtrProcess)(const map<string,string>& mapParams);
typedef int (*FunPtrFini)();

class CFLSlaveWorker 
{
    public:
        static void *SoObj;
        static int iRun;
    public:
        CFLSlaveWorker ();
        virtual ~CFLSlaveWorker ();

        /**
         * @brief   设置mmapfile
         *
         * @param   mmapFile
         *
         * @return  0           succ
         *          else        fail
         */
        int SetMMapFile(const string& mmapFile);
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

        /**
         * @brief   获取文件大小
         *
         * @param   filename        文件名
         *
         * @return  文件大小
         */
        unsigned long get_file_size(const char *filename);

        /**
         * @brief   读取mmap文件中的信息
         *
         * @param   filename        文件名
         *
         * @return  0               succ
         *          else            fail
         */
        int load_mmapdata(const string& filename);

    private:
        CFLSlaveInput m_SlaveInput;

        FunPtrInit m_funPtrInit;
        FunPtrProcess m_funPtrProcess;
        FunPtrFini m_funPtrFini;

        bool m_bReadInput;
        string m_mmapFile;
};

#endif
