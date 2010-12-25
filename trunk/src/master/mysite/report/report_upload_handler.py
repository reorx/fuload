#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        report_upload_handler.py
#  Description:     处理slave上报数据的类
#  Version:         1.0
#  LastChange:      2010-12-13 11:39:37
#  History:         
#=============================================================================
'''
import datetime

try:
    import json
except ImportError:
    import simplejson as json

from models import StatDetail
from comm_def import report_keys
from comm_func import get_border_time, get_pre_time,calc_values

class ReportUploadHandler(object):
    '''
    这是worker进程中的定义
    allTimeMsStat
    sucTimeMsStat
    errTimeMsStat

    allReqNum
    sucReqNum
    errReqNum

    alltimemap = {}
    suctimemap = {}
    errtimemap = {}
    retmap = {}
    '''

    reportId = None
    slaveReport = None
    clientIp = None

    def __init__(self, reportId, clientIp, slaveReport):
        self.reportId = reportId
        self.slaveReport = slaveReport
        self.clientIp = clientIp

    def handle_report(self):
        slaveReportObj = json.loads(self.slaveReport)

        now_time = datetime.datetime.now()
        now_border_time = get_border_time(now_time)

        self.save_report(self.reportId,self.clientIp,now_border_time['first_time'],now_border_time['second_time'],slaveReportObj)

        return True

    def merge_report(self, report1, report2):
        '''
        递归合并report
        '''

        #当其中有至少一个为None的时候，直接返回那个不为None的
        if report1 is None or report2 is None:
            return report1 if report1 is not None else report2

        #在都不为None的情况下，如果两个类型不相等，要抛出异常
        if type(report1) != type(report2):
            raise TypeError

        if not isinstance(report1,dict):
            return report1 + report2

        keys = list(set(report1.keys()) | set(report2.keys()))

        new_report = {}
        for key in keys:
            try:
                value1 = report1[key]
            except:
                value1 = None

            try:
                value2 = report2[key]
            except:
                value2 = None

            new_report[key] = self.merge_report(value1,value2)
        return new_report

    def save_report(self, report_id, clientIp, first_time, second_time, report_info):
        new_report_info = {}
        try:
            stat_detail = StatDetail.objects.get(reportId=report_id, clientIp = clientIp, firstTime=first_time, secondTime=second_time)
            try:
                old_report_info = eval(stat_detail.reportInfo)
            except:
                old_report_info = {}
            #合并出最新的report
            new_report_info = self.merge_report(old_report_info,report_info)

        except StatDetail.DoesNotExist:
            stat_detail = StatDetail(reportId=report_id, clientIp = clientIp, firstTime=first_time, secondTime=second_time)
            new_report_info = report_info


        result = calc_values(new_report_info)
        for k,v in result.items():
            setattr(stat_detail,k,v)

        stat_detail.reportInfo = repr(new_report_info)

        stat_detail.save()

        return True
