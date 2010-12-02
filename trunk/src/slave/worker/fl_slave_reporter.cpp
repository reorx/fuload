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
void CSlaveReporter::AddCount(int retcode, long usec)
{
    int time_ms = usec / 1000;
    m_LocStat.AddCount(retcode,time_ms);
    m_NetStat.AddCount(retcode,time_ms);
}
void CSlaveReporter::ResetStat()
{
    m_LocStat.ResetStat();
    m_NetStat.ResetStat();
}
