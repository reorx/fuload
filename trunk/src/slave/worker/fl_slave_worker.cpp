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
int CFLSlaveWorker::iSignal=0;
void handle_signal_user1(int sig)
{
    CFLSlaveWorker::iSignal= 1;
}

CFLSlaveWorker::CFLSlaveWorker()
{
    m_funPtrInit = NULL;
    m_funPtrProcess = NULL;
    m_funPtrFini = NULL;

    m_bReadInput = false;
    m_stat_info.Init("fl_statfile",stat_desc,STAT_OVER);
}

CFLSlaveWorker::~CFLSlaveWorker()
{
}
int CFLSlaveWorker::SetInputData(const string& strInputData)
{
    return m_SlaveInput.SetInputData(strInputData);
}
int CFLSlaveWorker::SetMMapFile(const string& mmapFile)
{
    m_mmapFile = mmapFile;
    return 0;
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
    signal(SIGUSR1,handle_signal_user1);
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
        if (CFLSlaveWorker::iSignal == 1)
        {
            ret = load_mmapdata(m_mmapFile);
            if (ret)
            {
                break;
            }
            if (m_MMapWrapper.run == 1)
            {
                ret = handle_starttest();
                if (ret)
                {
                    break;
                }
            }
            else
            {
                ret = handle_stoptest();
                if (ret)
                {
                    break;
                }
            }
        }
        if (m_MMapWrapper.run != 1)
        {
            sleep(1);
            continue;
        }

        StSWInput swi;
        ret = m_SlaveInput.Alloc(swi);
        if (ret)
        {
            printf("error alloc:%d\n",ret);
            break;
        }
        m_stat_info.AddCount(STAT_REQ);
        gettimeofday(&stBegin, NULL);
        ret = process(swi);
        gettimeofday(&stEnd, NULL);
        usec = (stEnd.tv_sec-stBegin.tv_sec)*1000000+stEnd.tv_usec-stBegin.tv_usec;

        if (ret)
        {
            m_stat_info.AddCount(STAT_ERR);
            printf("process error:%d\n",ret);
        }
        dealTimeStat(usec);
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
unsigned long CFLSlaveWorker::get_file_size(const char *filename)
{
    struct stat buf;
    if(stat(filename, &buf)<0)
    {
        return 0;
    }
    return (unsigned long)buf.st_size;
}
int CFLSlaveWorker::load_mmapdata(const string& filename)
{
    ifstream fin;
    fin.open(filename.c_str());

    string mmapData;
    string strLine;
    while(getline(fin,strLine))
    {
        mmapData.append(strLine);
        mmapData.append("\n");
    }
    int ret = m_MMapWrapper.Input(mmapData);
    if (ret)
    {
        printf("m_MMapWrapper input error:%d\n",ret);
        return ret;
    }
    return SetInputData(m_MMapWrapper.inputdata);
}
int CFLSlaveWorker::handle_starttest()
{
    return 0;
}
int CFLSlaveWorker::handle_stoptest()
{
    m_bReadInput = false;
    return 0;
}
void CFLSlaveWorker::dealTimeStat(long usec)
{
    if(usec<5000)
        m_stat_info.AddCount(STAT_5000_REQ);
    else if(usec<10000)
        m_stat_info.AddCount(STAT_10000_REQ);
    else if(usec<50000)
        m_stat_info.AddCount(STAT_50000_REQ);
    else if(usec<100000)
        m_stat_info.AddCount(STAT_100000_REQ);
    else if(usec<200000)
        m_stat_info.AddCount(STAT_200000_REQ);
    else if(usec<500000)
        m_stat_info.AddCount(STAT_500000_REQ);
    else if(usec<1000000)
        m_stat_info.AddCount(STAT_1000000_REQ);
    else
        m_stat_info.AddCount(STAT_MORE_REQ);
}
