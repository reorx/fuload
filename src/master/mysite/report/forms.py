#!/usr/bin/python
# -*- coding: utf-8 -*-

from django import forms
from comm_def import rtype2attr

class SearchReportShowForm(forms.Form):
    reportid = forms.IntegerField(required=True,label='上报ID')
    begintime = forms.DateTimeField(required=False,label='起始时间')
    endtime = forms.DateTimeField(required=False,label='终止时间')
    rtype = forms.CharField(required=True,
            label='报表类型',
            widget=forms.Select(
                choices = [(k,v['name']) for k,v in rtype2attr.items()]
                )
            )
class SearchReportDataForm(SearchReportShowForm):
    clientip = forms.CharField(required=True)
