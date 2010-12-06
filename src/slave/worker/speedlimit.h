/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        speedlimit.h
#  Description:     速度限制类
#  Version:         1.0
#  LastChange:      2010-12-05 11:51:13
#  History:         
=============================================================================*/
#ifndef _SPEEDLIMIT_H_
#define _SPEEDLIMIT_H_
#include <sys/time.h>
class CSpeedLimit
{
    public:
        CSpeedLimit()
        {/*{{{*/
            Clear();
        }/*}}}*/

        /** 
         * @brief   初始化
         * 
         * @param   iMaxCount   最大速度
         * 
         */
        void Init(int iMaxCount)
        {/*{{{*/
            m_MaxCount=iMaxCount;
            m_TrueMaxCount=iMaxCount;
        }/*}}}*/

        /** 
         * @brief   限制速度
         * 
         * @return  0,1,2,3
         */
        int DetectAndLimit()
        {/*{{{*/
            if(!m_Run)
            {
                m_Run=true;
                gettimeofday(&m_tpstart,NULL);
            }
            if (m_MaxCount<=0)
            {
                return 1;
            }
            ++m_CountPerSec;
            //printf("trueMax[%d]\n",m_TrueMaxCount);
            if(m_CountPerSec<m_TrueMaxCount)
            {
                //printf("%d\n",m_CountPerSec);
                return 0;
            }
            int ret=0;
            gettimeofday(&m_tpend,NULL);
            int timeuse=1000000*(m_tpend.tv_sec-m_tpstart.tv_sec)+m_tpend.tv_usec-m_tpstart.tv_usec;//微秒
            if(timeuse<=1000000 )
            {
                if(m_CountPerSec>=m_MaxCount)
                {
                    m_Speed=m_CountPerSec;
                    //动态调整
                    m_TrueMaxCount=m_Speed;

                    usleep(1000000-timeuse);
                    gettimeofday(&m_tpstart,NULL);
                    m_CountPerSec=0;
                    ret = 1;
                }
                ret = 2;
            }
            else
            {
                m_Speed = m_CountPerSec*1000000/timeuse;
                //动态调整
                m_TrueMaxCount=m_Speed;

                gettimeofday(&m_tpstart,NULL);
                m_CountPerSec=0;
                ret = 3;
            }
            return ret;
        }/*}}}*/

        /** 
         * @brief   获取当前速度
         * 
         * @return  速度
         */
        int Speed()
        {/*{{{*/
            return m_Speed;
        }/*}}}*/

        /** 
         * @brief   清空所有数据
         */
        void Clear()
        {/*{{{*/
            m_MaxCount=0;
            m_TrueMaxCount=0;
            m_CountPerSec=0;
            m_Speed=0;
            m_Run=false;
        }/*}}}*/

    private:
        struct timeval m_tpstart,m_tpend;
        //设置的最大速度
        int m_MaxCount;

        //通过动态调整得到的真实估计最大速度
        int m_TrueMaxCount;

        //计数器
        int m_CountPerSec;

        //当前速度
        int m_Speed;

        bool m_Run;
};
#endif

/*#include <iostream>
#include "speedlimit.h"
using namespace std;
int main()
{
    CSpeedLimit speedlimit;
    speedlimit.Init(10);
    while(1)
    {
        speedlimit.DetectAndLimit();
        printf("%d\n",speedlimit.Speed());
    }
} */
