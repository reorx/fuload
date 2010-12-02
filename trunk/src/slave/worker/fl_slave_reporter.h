/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        fl_slave_reporter.h
#  Description:     统一上报组件
#  Version:         1.0
#  LastChange:      2010-12-02 22:56:14
#  History:         
=============================================================================*/
#ifndef _FL_SLAVE_REPORTER_H_
#define _FL_SLAVE_REPORTER_H_
#include <iostream>
#include <string>
#include <map>

#include "fl_slave_report.h"

using namespace std;

class CSlaveReporter 
{
    public:
        CSlaveReporter ();
        virtual ~CSlaveReporter ();

        void AddCount(int retcode, long usec);
        void ResetStat();

    private:
        StSWLocStat m_LocStat;
        StSWNetStat m_NetStat;
};
#endif
