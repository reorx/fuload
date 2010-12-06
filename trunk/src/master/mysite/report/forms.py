#!/usr/bin/python
# -*- coding: utf-8 -*-

from django import forms
from comm_def import rtype_original_keys,rtype2attr
from django.contrib.admin import widgets

class SearchReportShowForm(forms.Form):
    reportid = forms.IntegerField(required=True,label='上报ID')
    begintime = forms.DateTimeField(required=True,label='时间',widget=widgets.AdminDateWidget())
    #endtime = forms.DateTimeField(required=False,label='终止时间',widget=widgets.AdminDateWidget())
    rtype = forms.CharField(required=True,
            label='报表类型',
            widget=forms.Select(
                choices = [(v,rtype2attr[v]['name']) for v in rtype_original_keys]
                )
            )
class SearchReportDataForm(SearchReportShowForm):
    clientip = forms.CharField(required=True)
