#!/usr/bin/python
# -*- coding: utf-8 -*-
import datetime
import logging

try:
    import json
except ImportError:
    import simplejson as json

from models import StatDetail
from comm_def import report_keys,loop_keys
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

    def save_report(self, report_id, clientIp, first_time, second_time, report_info):
        #logging.error('come:'+repr(report_info['allTimeMsStat']))
        #logging.error('come:'+repr(report_info))
        new_report_info = {}
        try:
            stat_detail = StatDetail.objects.get(reportId=report_id, clientIp = clientIp, firstTime=first_time, secondTime=second_time)
            try:
                old_report_info = eval(stat_detail.reportInfo)
            except:
                old_report_info = {}
            for key in report_info:
                if key not in loop_keys:
                    new_report_info[key] = old_report_info[key] + report_info[key]
                else:
                    new_report_info[key] = {}
                    for tinfo in (old_report_info, report_info):
                        for subkey in tinfo[key]:
                            if subkey in new_report_info[key]:
                                new_report_info[key][subkey] += tinfo[key][subkey]
                            else:
                                new_report_info[key][subkey] = tinfo[key][subkey]

        except StatDetail.DoesNotExist:
            stat_detail = StatDetail(reportId=report_id, clientIp = clientIp, firstTime=first_time, secondTime=second_time)
            new_report_info = report_info

        #logging.error('new:'+repr(new_report_info['allTimeMsStat']))

        result = calc_values(new_report_info)
        for k,v in result.items():
            setattr(stat_detail,k,v)

        stat_detail.reportInfo = repr(new_report_info)

        stat_detail.save()

        return True
