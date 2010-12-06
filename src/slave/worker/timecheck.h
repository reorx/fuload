/*=============================================================================
#  Author:          DanteZhu - http://www.vimer.cn
#  Email:           dantezhu@vip.qq.com
#  FileName:        timecheck.h
#  Version:         1.0
#  LastChange:      2010-12-02 17:13:00
#  Description:     每个多长时间或者多少次执行一次的时间类
#  History:         
=============================================================================*/
#ifndef _TIMECHECK_H_
#define _TIMECHECK_H_
#include <sys/time.h>
class CTimeCheck
{
    public:
        CTimeCheck()
        {
            Clear();
        }
        /** 
         * @brief   初始化
         * 
         * @param   maxFreshTime_ms     多少毫秒执行一次
         * @param   firstTimeShouldRun  第一次check的时候，是否必须返回true
         * @param   maxCheckFreq        每多少个请求检查一次(包括这个值),0则每次都查(默认为否)
         * 
         * @return  true                可以执行
         *          false               不可以执行
         */
        int Init(unsigned int maxFreshTime_ms,bool firstTimeShouldRun=false, int maxCheckFreq = 0)
        {
            m_MaxFreshTime_ms = maxFreshTime_ms;
            m_FirstTimeShouldRun = firstTimeShouldRun;
            m_MaxCheckFreq = maxCheckFreq;
            return 0;
        }
        bool Check()
        {
            if(m_MaxFreshTime_ms == 0)
            {
                return true;
            }

            if(!m_Run)
            {
                gettimeofday(&m_Start_TV, NULL);
                m_Run = true;
                if (m_FirstTimeShouldRun)
                {
                    return true;
                }
            }

            if (m_CurCheckTimes >= m_MaxCheckFreq)
            {
                m_CurCheckTimes = 0;
            }
            else
            {
                ++m_CurCheckTimes;
                return false;
            }

            int lefttime  = 0;
            static struct timeval now_tv;
            gettimeofday(&now_tv, NULL);
            lefttime =  (now_tv.tv_sec  - m_Start_TV.tv_sec ) * 1000 + (now_tv.tv_usec - m_Start_TV.tv_usec) / 1000;
            if (lefttime < 0)
            {
                lefttime = 0;
            }
            if( (unsigned)lefttime >= m_MaxFreshTime_ms)
            {
                gettimeofday(&m_Start_TV, NULL);
                return true;
            }
            return false;
        }

        bool Stat()
        {
            return m_Run;
        }

        void Clear()
        {
            m_MaxFreshTime_ms = 0;//微秒,没有限制
            m_MaxCheckFreq = 0;//每次都检查
            m_CurCheckTimes = 0;
            m_Run = false;
        }
    private:
        unsigned int m_MaxFreshTime_ms;
        int m_MaxCheckFreq;

        int m_CurCheckTimes;//当前未检查的次数

        struct timeval m_Start_TV;

        bool m_Run;

        bool m_FirstTimeShouldRun;
};
#endif
