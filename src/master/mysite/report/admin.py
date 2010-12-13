#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        admin.py
#  Description:     定制后台管理端展现
#  Version:         1.0
#  LastChange:      2010-12-13 11:38:32
#  History:         
#=============================================================================
'''
from django.contrib import admin
from mysite.report.models import *

class StatDetailAdmin(admin.ModelAdmin):
    list_display = (
            'reportId','clientIp',
            'firstTime', 'secondTime',
            'allAvgNum','sucAvgNum','errAvgNum',
            'allAvgTime','sucAvgTime','errAvgTime',
            'sucRate','errRate'
            )
    list_filter = ('reportId', 'clientIp')
    ordering = ('firstTime',)

admin.site.register(StatDetail,StatDetailAdmin)
