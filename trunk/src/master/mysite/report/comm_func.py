#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        comm_func.py
#  Description:     共用的函数
#  Version:         1.0
#  LastChange:      2010-12-13 11:39:18
#  History:         
#=============================================================================
'''

import time
import datetime
from comm_def import split_minutes,rtype2attr,max_x_len,default_grid_width,default_persent_yx,pie_colors

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

#仅仅是把数据查找出来没有做进一步的处理
def get_report_objs(cd):
    from models import StatDetail
    objs = StatDetail.objects.filter(reportId=cd['reportid'])

    if 'clientip' in cd and cd['clientip'] is not None:
        objs = objs.filter(clientIp=cd['clientip'])

    if 'begintime' in cd and cd['begintime'] is not None:
        objs = objs.filter(firstTime__gte=cd['begintime'])

    if 'endtime' in cd and cd['endtime'] is not None:
        objs = objs.filter(firstTime__lt=cd['endtime'])

    objs.order_by('firstTime')

    return objs

def compress_data_line(data,max_x_len):
    '''
    压缩线性的数据
    '''
    while len(data) > max_x_len:
        t_data = []
        for i,item in enumerate(data):
            if i%2 == 0:
                if i+1 < len(data):
                    first_y = item['y']
                    second_y = data[i+1]['y']
                    if first_y is not None and second_y is not None:
                        t_y = (float(first_y)+float(second_y)) / 2
                    else:
                        t_y = first_y if first_y is not None else second_y

                    t_item = {
                            'x':item['x'],
                            'y':t_y
                            }
                else:
                    t_item = item
                t_data.append(t_item)
        data = t_data
    return data

def get_report_data_line(cd):
    '''
    获取线性的数据
    '''
    from models import StatDetail

    objs = get_report_objs(cd)

    if objs is None or len(objs) == 0:
        return []

    rtype = cd['rtype']

    begintime = objs[0].firstTime
    endtime = objs[len(objs)-1].firstTime

    data = []
    t = datetime.timedelta(minutes=split_minutes)
    d = begintime
    while d <= endtime:
        t_item= {}
        t_item['x']=d
        try:
             obj = objs.get(firstTime=d)
        except StatDetail.DoesNotExist:
            t_item['y'] = None
        else:
            t_item['y'] = getattr(obj,rtype2attr[rtype]['attr'])
        data.append(t_item)
        d = d+t

    if cd['adjust'] == 0:
        data = compress_data_line(data,max_x_len)

    for item in data:
        if item['x'] is not None:
            item['x'] = item['x'].strftime('%Y-%m-%d %H:%M')
        if item['y'] is not None:
            item['y'] = rtype2attr[rtype]['accuracy'] % item['y']

    return data

def get_report_data_pie(cd):
    '''
    获取饼状的数据
    '''
    objs = get_report_objs(cd)

    if objs is None or len(objs) == 0:
        return []
    rtype = cd['rtype']

    data_map = {}
    for obj in objs:
        report_info = eval(obj.reportInfo)
        t_map = report_info[rtype]
        if t_map is None:
            continue
        for k,v in t_map.items():
            if k in data_map:
                data_map[k] += v
            else:
                data_map[k] = v

    orig_data = []
    for k,v in data_map.items():
        t_item = {
                'name':k,
                'value':v
                }
        orig_data.append(t_item)

    orig_data.sort(lambda x,y: cmp(y['value'], x['value']))   

    max_pie_typecount = len(pie_colors) - 1

    #截断出最大类别
    cut_data = orig_data[:max_pie_typecount]

    if len(orig_data) > max_pie_typecount:
        sum_d = 0
        for i in range(max_pie_typecount,len(orig_data)):
            sum_d += orig_data[i]['value']
        cut_data.append({'name':'else','value':sum_d})

    sum_value = 0

    for v in cut_data:
        sum_value+=v['value']

    #加上颜色
    accuracy = rtype2attr[rtype]['accuracy']
    for i,item in enumerate(cut_data):
        item['color'] = pie_colors[i]
        if float(sum_value) != 0:
            item['value'] = accuracy % (float(item['value']) * float(100) / float(sum_value))
        else:
            item['value'] = 0

    return cut_data

def cal_grid_width(len_data=0):
    if len_data <= max_x_len:
        return default_grid_width
    return default_grid_width * len_data / max_x_len

def cal_persent_yx(len_data=0):
    if len_data <= max_x_len:
        return default_persent_yx
    return default_persent_yx * len_data / max_x_len

if __name__ == '__main__':
    print get_border_time(datetime.datetime.now())
    #print get_pre_time(datetime.datetime.now())
