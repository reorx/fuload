/*=============================================================================
#  Author:          DanteZhu - http://www.vimer.cn
#  Email:           dantezhu@vip.qq.com
#  FileName:        stat_def.h
#  Description:     统计码定义
#  Version:         1.0
#  LastChange:      2010-10-08 15:42:38
#  History:         
=============================================================================*/
#ifndef _STAT_DEF_H_
#define _STAT_DEF_H_
typedef enum
{
    STAT_REQ = 0,
    STAT_ERR,

    STAT_5000_REQ,
    STAT_10000_REQ,
    STAT_50000_REQ,
    STAT_100000_REQ,
    STAT_200000_REQ,
    STAT_500000_REQ,
    STAT_1000000_REQ,
    STAT_MORE_REQ,

    STAT_OVER
}STAT_ID;

const char * const stat_desc[] = 
{
    "STAT_REQ",
    "STAT_ERR",

    "STAT_5000_REQ",
    "STAT_10000_REQ",
    "STAT_50000_REQ",
    "STAT_100000_REQ",
    "STAT_200000_REQ",
    "STAT_500000_REQ",
    "STAT_1000000_REQ",
    "STAT_MORE_REQ",
};
#endif
