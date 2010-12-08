#!/usr/bin/python
# -*- coding: utf-8 -*-

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
    result['allAvgNum'] = float(report_info['allReqNum'])/float(split_minutes * 60)
    result['sucAvgNum'] = float(report_info['sucReqNum'])/float(split_minutes * 60)
    result['errAvgNum'] = float(report_info['errReqNum'])/float(split_minutes * 60)

    result['allAvgTime'] = float(report_info['allTimeMsStat']) / float(report_info['allReqNum']) if float(report_info['allReqNum']) != 0 else 0
    result['sucAvgTime'] = float(report_info['sucTimeMsStat']) / float(report_info['sucReqNum']) if float(report_info['sucReqNum']) != 0 else 0
    result['errAvgTime'] = float(report_info['errTimeMsStat']) / float(report_info['errReqNum']) if float(report_info['errReqNum']) != 0 else 0

    result['sucRate'] = 100 * float(report_info['sucReqNum']) / float(report_info['allReqNum']) if float(report_info['allReqNum']) != 0 else 100
    result['errRate'] = 100 * float(report_info['errReqNum']) / float(report_info['allReqNum']) if float(report_info['allReqNum']) != 0 else 0

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
        objs = objs.filter(secondTime__lte=cd['endtime'])

    objs.order_by('firstTime')

    return objs

def get_report_data_line(cd):
    from models import StatDetail

    objs = get_report_objs(cd)

    if objs is None or len(objs) == 0:
        return []

    rtype = cd['rtype']

    begintime = objs[0].firstTime
    endtime = objs[len(objs)-1].firstTime

    data = []
    t = datetime.timedelta(minutes=split_minutes)
    d = begintime + t
    while d < (endtime):
        dict_d = {}
        dict_d['x']=d
        try:
             obj = objs.get(firstTime=d)
        except StatDetail.DoesNotExist:
            dict_d['y'] = ''
        else:
            dict_d['y'] = getattr(obj,rtype2attr[rtype]['attr'])
            dict_d['y'] = rtype2attr[rtype]['accuracy'] % (dict_d['y'])
        data.append(dict_d)
        d = d+t

    if cd['adjust'] == 0:
        while len(data) > max_x_len:
            tmpdata = []
            for i,d in enumerate(data):
                if i%2 == 0:
                    if i+1 < len(data):
                        if len(d['y']) > 0 and len(data[i+1]['y'])>0:
                            tmp_y = (float(d['y'])+float(data[i+1]['y'])) / 2
                            tmp_y = rtype2attr[rtype]['accuracy'] % tmp_y
                        else:
                            tmp_y = d['y'] if len(d['y']) > 0 else data[i+1]['y']

                        tmp_d = {
                                'x':d['x'],
                                'y':tmp_y
                                }
                    else:
                        tmp_d = d
                    tmpdata.append(tmp_d)
            data = tmpdata

    return data

def get_report_data_pie(cd):
    objs = get_report_objs(cd)

    if objs is None or len(objs) == 0:
        return []
    rtype = cd['rtype']

    data_map = {}
    for obj in objs:
        report_info = eval(obj.reportInfo)
        single_map = report_info[rtype]
        for k,v in single_map.items():
            if k in data_map:
                data_map[k] += v
            else:
                data_map[k] = v

    tmp_data = []
    for k,v in data_map.items():
        dict_d = {
                'name':k,
                'value':v
                }
        tmp_data.append(dict_d)

    tmp_data.sort(lambda x,y: cmp(y['value'], x['value']))   

    max_pie_typecount = len(pie_colors) - 1

    res_data = tmp_data[:max_pie_typecount]

    if len(tmp_data) > max_pie_typecount:
        sum_d = 0
        for i in range(max_pie_typecount,len(tmp_data)):
            sum_d += tmp_data[i]['value']
        res_data.append({'name':'else','value':sum_d})

    sum_value = 0
    for v in res_data:
        sum_value+=v['value']

    for i in range(0,len(res_data)):
        res_data[i]['color'] = pie_colors[i]
        if float(sum_value) != 0:
            res_data[i]['value'] = rtype2attr[rtype]['accuracy'] % (float(res_data[i]['value']) * float(100) / float(sum_value))
        else:
            res_data[i]['value'] = rtype2attr[rtype]['accuracy'] % 0

    return res_data

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
