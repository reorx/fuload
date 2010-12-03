#!/usr/bin/python
# -*- coding: utf-8 -*-
import datetime

try:
    import json
except ImportError:
    import simplejson as json

from models import StatMain,StatDetail
from comm_func import detail_report_keys,get_border_time, get_pre_time,calc_values,copy_report_detail

class ReportHandler(object):
    '''
    这是worker进程中的定义
    root["allTimeMsStat"] = m_PtrNetStat->allTimeMsStat;
    root["sucTimeMsStat"] = m_PtrNetStat->sucTimeMsStat;
    root["errTimeMsStat"] = m_PtrNetStat->errTimeMsStat;

    root["allReqNum"] = m_PtrNetStat->allReqNum;
    root["sucReqNum"] = m_PtrNetStat->sucReqNum;
    root["errReqNum"] = m_PtrNetStat->errReqNum;
    '''

    reportId = None
    slaveReport = None

    def __init__(self):
        pass

    def set_report(self, reportId, slaveReport):
        self.reportId = reportId
        self.slaveReport = slaveReport

        ret = self.handle_report()
        if ret is False:
            return ret
        return True

    def handle_report(self):
        slaveReportObj = json.loads(self.slaveReport)

        now_time = datetime.datetime.now()
        pre_time = get_pre_time(now_time)

        pre_border_time = get_border_time(pre_time)
        now_border_time = get_border_time(now_time)

        pre_report_info = {}
        now_report_info = {}

        pre_distance = pre_border_time['second_distance']
        now_distance = now_border_time['first_distance']

        for key in detail_report_keys:
            value = int(slaveReportObj[key])
            pre_report_info[key] = value*pre_distance/(pre_distance+now_distance)
            now_report_info[key] = value*now_distance/(pre_distance+now_distance)
        
        self.save_to_detail(self.reportId,pre_border_time['first_time'],pre_border_time['second_time'],pre_report_info)
        self.save_to_detail(self.reportId,now_border_time['first_time'],now_border_time['second_time'],now_report_info)

        return True

    def save_to_detail(self, report_id, first_time, second_time, report_info):
        new_report_info = {}
        try:
            stat_detail = StatDetail.objects.get(reportId=report_id, firstTime=first_time, secondTime=second_time)
            try:
                old_report_info = eval(stat_detail.reportInfo)
            except:
                old_report_info = {}
            for key in report_info:
                new_report_info[key] = old_report_info[key] + report_info[key]
        except StatDetail.DoesNotExist:
            stat_detail = StatDetail(reportId=report_id, firstTime=first_time, secondTime=second_time)
            new_report_info = copy_report_detail(report_info)

        result = calc_values(new_report_info)
        for k,v in result.items():
            setattr(stat_detail,k,v)

        stat_detail.reportInfo = repr(new_report_info)

        stat_detail.save()

        return True
