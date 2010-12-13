#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        models.py
#  Description:     模型
#  Version:         1.0
#  LastChange:      2010-12-13 11:39:32
#  History:         
#=============================================================================
'''

from datetime import datetime

from django.db import models

class StatDetail(models.Model):
    reportId = models.IntegerField(verbose_name="上报ID")

    clientIp = models.CharField(max_length=128,verbose_name="上报IP")

    firstTime = models.DateTimeField(verbose_name="时间片起点")
    secondTime = models.DateTimeField(verbose_name="时间片终点")

    allAvgNum = models.FloatField(verbose_name="每秒总请求量")
    sucAvgNum = models.FloatField(verbose_name="每秒成功请求量")
    errAvgNum = models.FloatField(verbose_name="每秒失败请求量")

    allAvgTime = models.FloatField(verbose_name="平均响应时间")
    sucAvgTime = models.FloatField(verbose_name="成功平均响应时间")
    errAvgTime = models.FloatField(verbose_name="失败平均响应时间")

    sucRate = models.FloatField(verbose_name="成功率")
    errRate = models.FloatField(verbose_name="失败率")

    reportInfo = models.TextField(verbose_name="报告详细信息")

    class Meta:
        unique_together = (("reportId","firstTime","secondTime"))

    def __unicode__(self):
        return "[%s][%s][%u]" % (self.firstTime, self.secondTime, self.reportId)
