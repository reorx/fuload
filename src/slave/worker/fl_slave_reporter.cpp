/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        fl_slave_reporter.cpp
#  Description:     
#  Version:         1.0
#  LastChange:      2010-12-02 22:57:00
#  History:         
=============================================================================*/
#include "fl_slave_reporter.h"
CSlaveReporter::CSlaveReporter()
{
}
CSlaveReporter::~CSlaveReporter ()
{
}
int CSlaveReporter::Init(int key, int reportTime_sec,const string& statFile)
{
    int ret;
    ret = m_MsgWrapper.Init(key);
    if (ret)
    {
        return -1;
    }
    m_Timer.Init(reportTime_sec*1000,false);
    m_LocStat.Init(statFile);
    return 0;
}
void CSlaveReporter::AddCount(int retcode, int time_ms)
{
    if (time_ms < 0)
    {
        time_ms = 0;
    }
    m_LocStat.AddCount(retcode,time_ms);
    m_NetStat.AddCount(retcode,time_ms);

    if (m_Timer.Check())
    {
        ReportToCtrl();
    }
}
int CSlaveReporter::ReportToCtrl()
{
    CSWReport report(&m_NetStat);
    string output = report.Output();
    debug_log("report:%s",output.c_str());
    int ret = m_MsgWrapper.send(output);
    if (ret)
    {
        error_log("msg send error,ret:%d,msg:%s",ret,output.c_str());
    }
    m_NetStat.ResetStat();
    return 0;
}
