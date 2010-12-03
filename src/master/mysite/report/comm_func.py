#!/usr/bin/python
# -*- coding: utf-8 -*-

import time
import datetime

#=============================================================================
#五分钟
split_minutes = 5
detail_report_keys = ('allTimeMsStat', 'sucTimeMsStat', 'errTimeMsStat', 'allReqNum', 'sucReqNum', 'errReqNum')
#=============================================================================
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


def get_pre_time(now_time):
    '''
    通过给定时间，获取split_minutes之前的是时间
    '''
    t = datetime.timedelta(minutes=split_minutes)
    pre_time = now_time - t
    return pre_time

def calc_values(report_info):
    result = {}
    result['allReqNum'] = report_info['allReqNum']
    result['sucReqNum'] = report_info['sucReqNum']
    result['errReqNum'] = report_info['errReqNum']

    result['allAvgTime'] = report_info['allTimeMsStat'] / result['allReqNum']
    result['sucAvgTime'] = report_info['sucTimeMsStat'] / result['sucReqNum']
    result['errAvgTime'] = report_info['errTimeMsStat'] / result['errReqNum']

    result['sucRate'] = float(result['sucReqNum']) / float(result['allReqNum'])
    result['errRate'] = float(result['errReqNum']) / float(result['allReqNum'])

    return result

def copy_report_detail(report_info):
    new_report_info = {}
    for key in detail_report_keys:
        new_report_info[key] = report_info[key]
    return new_report_info

if __name__ == '__main__':
    print get_border_time(datetime.datetime.now())
    #print get_pre_time(datetime.datetime.now())
