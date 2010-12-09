#!/usr/bin/python
# -*- coding: utf-8 -*-

import datetime

from django import forms
from comm_def import rtype_original_keys,rtype2attr
from django.contrib.admin import widgets
from django.core import validators

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
                choices = [(v,rtype2attr[v]['name']) for v in rtype_original_keys]
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
                choices = [(v,rtype2attr[v]['name']) for v in rtype_original_keys]
                )
            )
    adjust = forms.IntegerField(required=True,
            label='展示调整',
            widget=forms.Select(
                choices = [(0,'自动调整精度'),(1,'自动调整宽度')]
                )
            )
    clientip = forms.CharField(required=True)
