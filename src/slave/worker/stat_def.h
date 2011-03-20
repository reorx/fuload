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
    STAT_SUC,
    STAT_ERR,

    STAT_5MS_SUC,
    STAT_10MS_SUC,
    STAT_50MS_SUC,
    STAT_100MS_SUC,
    STAT_200MS_SUC,
    STAT_500MS_SUC,
    STAT_1000MS_SUC,
    STAT_MORE_SUC,

    STAT_5MS_ERR,
    STAT_10MS_ERR,
    STAT_50MS_ERR,
    STAT_100MS_ERR,
    STAT_200MS_ERR,
    STAT_500MS_ERR,
    STAT_1000MS_ERR,
    STAT_MORE_ERR,

    STAT_OVER
}STAT_ID;

const char * const stat_desc[] = 
{
    "STAT_REQ",
    "STAT_SUC",
    "STAT_ERR",

    "STAT_5MS_SUC",
    "STAT_10MS_SUC",
    "STAT_50MS_SUC",
    "STAT_100MS_SUC",
    "STAT_200MS_SUC",
    "STAT_500MS_SUC",
    "STAT_1000MS_SUC",
    "STAT_MORE_SUC",

    "STAT_5MS_ERR",
    "STAT_10MS_ERR",
    "STAT_50MS_ERR",
    "STAT_100MS_ERR",
    "STAT_200MS_ERR",
    "STAT_500MS_ERR",
    "STAT_1000MS_ERR",
    "STAT_MORE_ERR",
};
#endif
