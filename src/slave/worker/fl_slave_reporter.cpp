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
    ResetStat();
}
CSlaveReporter::~CSlaveReporter ()
{
}
int CSlaveReporter::Init(int key, int reportTime_sec)
{
    int ret;
    ret = m_MsgWrapper.Init(key);
    if (ret)
    {
        return -1;
    }
    m_Timer.Init(reportTime_sec*1000,false);
    return 0;
}
void CSlaveReporter::AddCount(int retcode, long usec)
{
    int time_ms = usec / 1000;
    m_LocStat.AddCount(retcode,time_ms);
    m_NetStat.AddCount(retcode,time_ms);

    if (m_Timer.Check())
    {
        ReportToCtrl();
    }
}
void CSlaveReporter::ResetStat()
{
    m_LocStat.ResetStat();
    m_NetStat.ResetStat();
}
int CSlaveReporter::ReportToCtrl()
{
    CSWReport report(&m_NetStat);
    string output = report.Output();
    m_MsgWrapper.send(output);
    m_NetStat.ResetStat();
    return 0;
}
