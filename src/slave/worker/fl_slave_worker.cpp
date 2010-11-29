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
void* CFLSlaveWorker::SoObj=NULL;

CFLSlaveWorker::CFLSlaveWorker()
{
    m_funPtrInit = NULL;
    m_funPtrProcess = NULL;
    m_funPtrFini = NULL;
}

CFLSlaveWorker::~CFLSlaveWorker()
{
}
int CFLSlaveWorker::SetInputData(const string& strInputData)
{
    return m_SlaveInput.SetInputData(strInputData);
}
int CFLSlaveWorker::SetModuleFile(const string& moduleFile)
{
    if(CFLSlaveWorker::SoObj == NULL)
    {
        SoObj=dlopen((char*)moduleFile.c_str(),RTLD_LAZY);
        if(SoObj==NULL)
        {
            return -1;
        }
    }
    m_funPtrInit = (FunPtrInit)dlsym(SoObj, "fuload_handle_init");
    m_funPtrProcess= (FunPtrProcess)dlsym(SoObj, "fuload_handle_process");
    m_funPtrFini= (FunPtrFini)dlsym(SoObj, "fuload_handle_fini");
    return 0;
}
int CFLSlaveWorker::Run()
{
    int ret;
    if (m_funPtrInit)
    {
        ret = (*m_funPtrInit)();
        if (ret)
        {
            return -1;
        }
    }
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
        }
    }
    if (m_funPtrFini)
    {
        ret = (*m_funPtrFini)();
        if (ret)
        {
            return -2;
        }
    }
    return 0;
}
int CFLSlaveWorker::process(const StSWInput& swi)
{
    const map<string,string> &mapParams = swi.mapParams;
    if (m_funPtrProcess == NULL)
    {
        return -1;
    }
    return (*m_funPtrProcess)(mapParams);
}
