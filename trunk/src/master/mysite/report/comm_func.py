#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        comm_func.py
#  Description:     共用的函数
#  Version:         1.0
#  LastChange:      2011-01-14 21:59:35
#  History:         
#=============================================================================
'''
from comm_def import split_minutes,max_x_len,default_grid_width,default_persent_yx

def get_border_time(now_time):
    '''
    now_time = datetime.datetime.now()
    通过给定时间，获取所处的区段，及距离两端的比例
    '''
    #分割时间为5分钟

    now_tup = now_time.timetuple()

    #5分钟之内的分割
    first_min = now_tup.tm_min - int(now_tup.tm_min / split_minutes) * split_minutes
    second_min = split_minutes - first_min

    t = datetime.timedelta(minutes=first_min)
    first_time = now_time - t
    first_time = datetime.datetime(*first_time.timetuple()[:5])

    t = datetime.timedelta(minutes=second_min)
    second_time = now_time + t
    second_time = datetime.datetime(*second_time.timetuple()[:5])

    now_stamp = time.mktime(now_time.timetuple())
    first_stamp = time.mktime(first_time.timetuple())
    second_stamp = time.mktime(second_time.timetuple())

    first_distance = now_stamp - first_stamp
    second_distance = second_stamp - now_stamp

    return {
            'first_time':first_time,
            'second_time':second_time,
            'first_distance':first_distance,
            'second_distance':second_distance,
            }

def calc_values(report_info):
    result = {}

    #平均到每秒
    try:
        result['allAvgNum'] = float(report_info['allReqNum'])/float(split_minutes * 60)
    except:
        result['allAvgNum'] = 0

    try:
        result['sucAvgNum'] = float(report_info['sucReqNum'])/float(split_minutes * 60)
    except:
        result['sucAvgNum'] = 0

    try:
        result['errAvgNum'] = float(report_info['errReqNum'])/float(split_minutes * 60)
    except:
        result['errAvgNum'] = 0

    try:
        result['allAvgTime'] = float(report_info['allTimeMsStat']) / float(report_info['allReqNum'])
    except:
        result['allAvgTime'] = 0

    try:
        result['sucAvgTime'] = float(report_info['sucTimeMsStat']) / float(report_info['sucReqNum'])
    except:
        result['sucAvgTime'] = 0

    try:
        result['errAvgTime'] = float(report_info['errTimeMsStat']) / float(report_info['errReqNum'])
    except:
        result['errAvgTime'] = 0

    try:
        result['sucRate'] = 100 * float(report_info['sucReqNum']) / float(report_info['allReqNum'])
    except:
        result['sucRate'] = 100

    try:
        result['errRate'] = 100 * float(report_info['errReqNum']) / float(report_info['allReqNum'])
    except:
        result['errRate'] = 0

    return result

def cal_grid_width(len_data=0):
    if len_data <= max_x_len:
        return default_grid_width
    return default_grid_width * len_data / max_x_len

def cal_persent_yx(len_data=0):
    if len_data <= max_x_len:
        return default_persent_yx
    return default_persent_yx * len_data / max_x_len
