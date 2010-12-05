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
int CFLSlaveWorker::Init(const StSWParam& param)
{
    m_SWParam = param;   
    int ret;
    ret = setModuleFile(m_SWParam.moduleFile);
    if (ret)
    {
        return -1;
    }
    ret = m_reporter.Init(param.msgQKey, param.reportTime_sec);
    if (ret)
    {
        return -2;
    }
    ret = setInputData(param.inputFile);
    if (ret)
    {
        return -3;
    }
    m_SpeedLimit.Init(param.limitSpeed);
    return 0;
}
int CFLSlaveWorker::setModuleFile(const string& moduleFile)
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
int CFLSlaveWorker::setInputData(const string& inputFile)
{
    ifstream fin;
    fin.open(inputFile.c_str());

    string strInputData;
    string strLine;
    while(getline(fin,strLine))
    {
        strInputData.append(strLine);
        strInputData.append("\n");
    }
    return m_SlaveInput.SetInputData(strInputData);
}
int CFLSlaveWorker::Run()
{
    int ret;
    struct timeval stBegin;
    struct timeval stEnd;
    long usec;
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
        gettimeofday(&stBegin, NULL);
        ret = process(swi);
        gettimeofday(&stEnd, NULL);
        usec = (stEnd.tv_sec-stBegin.tv_sec)*1000000+stEnd.tv_usec-stBegin.tv_usec;
        m_reporter.AddCount(ret, usec);
        if (ret)
        {
            printf("process error:%d\n",ret);
        }
        m_SpeedLimit.DetectAndLimit();
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
