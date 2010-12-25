#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        forms.py
#  Description:     表单
#  Version:         1.0
#  LastChange:      2010-12-25 23:19:37
#  History:         
#=============================================================================
'''

import datetime

from django import forms
from comm_def import rtype2attr
from django.contrib.admin import widgets
from django.core import validators

#这段代码会在模块载入的时候执行，目的就是为了生成按顺序展示的类型下拉列表
rtype_order_list= []
for k,v in rtype2attr.items():
    rtype_order_list.append((k,v['name'],v['order']))
rtype_order_list.sort(lambda x,y: cmp(x[2], y[2]))
rtype_order_list = [(v[0],v[1]) for v in rtype_order_list]

class TimeStampField(forms.IntegerField):

    def __init__(self, input_formats=None, *args, **kwargs):
        super(TimeStampField, self).__init__(*args, **kwargs)
        self.input_formats = input_formats

    def to_python(self, value):
        """
        Validates that the input can be converted to a datetime. Returns a
        Python datetime.datetime object.
        """

        value = super(TimeStampField, self).to_python(value)
        if value in validators.EMPTY_VALUES:
            return None
        return datetime.datetime.fromtimestamp(value)


class SearchReportShowForm(forms.Form):
    reportid = forms.IntegerField(required=True,label='上报ID')
    begintime = forms.DateTimeField(required=True,label='起始时间')
    endtime = forms.DateTimeField(required=False,label='终止时间')
    rtype = forms.CharField(required=True,
            label='报表类型',
            widget=forms.Select(
                choices = rtype_order_list
                )
            )
    adjust = forms.IntegerField(required=True,
            label='展示调整',
            widget=forms.Select(
                choices = [(0,'自动调整精度'),(1,'自动调整宽度')]
                )
            )
class SearchReportDataForm(forms.Form):
    reportid = forms.IntegerField(required=True,label='上报ID')
    begintime = TimeStampField(required=True,label='起始时间')
    endtime = TimeStampField(required=False,label='终止时间')
    rtype = forms.CharField(required=True,
            label='报表类型',
            widget=forms.Select(
                choices = rtype_order_list
                )
            )
    adjust = forms.IntegerField(required=True,
            label='展示调整',
            widget=forms.Select(
                choices = [(0,'自动调整精度'),(1,'自动调整宽度')]
                )
            )
    clientip = forms.CharField(required=True)
