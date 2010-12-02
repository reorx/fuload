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

#include "fl_commfunc.h"
#include "fl_slave_input.h"
#include "stat.h"
#include "stat_def.h"
#include "timecheck.h"

using namespace std;

typedef struct _st_swparam
{
    //上报相关
    string ip;
    int port;
    int timeout_ms;
    int reportTime_sec;

    string mmapFile;
    string moduleFile;
}StSWParam;

typedef int (*FunPtrInit)();
typedef int (*FunPtrProcess)(const map<string,string>& mapParams);
typedef int (*FunPtrFini)();

typedef struct _st_mmapwrapper
{
    int run;
    string inputdata;

    _st_mmapwrapper()
    {
        run = 0;
    }

    int Input(const string& data)
    {
        Json::Reader reader;
        Json::Value value;
        std::string param;
        param.assign(data.c_str(), data.size());
        if (!reader.parse(param, value))
        {
            return -1;
        }
        run = value["run"].asInt();
        if (run == 1)
        {
            inputdata = value["input"].asString();
        }
        return 0;
    }
}StMMapWrapper;

class CFLSlaveWorker 
{
    public:
        static void *SoObj;
        static int iSignal;
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

        /**
         * @brief   在run设置成1之后，正式开始测试之前
         *
         * @return  0               succ
         *          else            fail
         */
        int handle_starttest();
        /**
         * @brief   在run重新设置2之后，并重置回0之前
         *
         * @return  0               succ
         *          else            fail
         */
        int handle_stoptest();

        /**
         * @brief   统计时间
         *
         * @param   retcode         返回码
         * @param   usec            消耗时间
         */
        void dealTimeStat(int retcode, long usec);

        /**
         * @brief   设置要加载的so的文件
         *
         * @param   moduleFile      文件
         *
         * @return  0               succ
         *          else            fail
         */
        int setModuleFile(const string& moduleFile);

    private:
        CFLSlaveInput m_SlaveInput;
        StMMapWrapper m_MMapWrapper;

        FunPtrInit m_funPtrInit;
        FunPtrProcess m_funPtrProcess;
        FunPtrFini m_funPtrFini;

        bool m_bReadInput;
        int m_iRun;

        string m_mmapFile;

        CStatInfo m_stat_info;
        StSWParam m_SWParam;
};

#endif
