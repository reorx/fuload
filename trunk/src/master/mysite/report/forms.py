#!/usr/bin/python
# -*- coding: utf-8 -*-

from django import forms
from comm_def import rtype_original_keys,rtype2attr

class SearchReportShowForm(forms.Form):
    reportid = forms.IntegerField(required=True,label='上报ID')
    begintime = forms.DateTimeField(required=False,label='起始时间',widget=forms.DateTimeInput(attrs={'onfocus':'HS_setDate(this)'}))
    endtime = forms.DateTimeField(required=False,label='终止时间',widget=forms.DateTimeInput(attrs={'onfocus':'HS_setDate(this)'}))
    rtype = forms.CharField(required=True,
            label='报表类型',
            widget=forms.Select(
                choices = [(v,rtype2attr[v]['name']) for v in rtype_original_keys]
                )
            )
class SearchReportDataForm(SearchReportShowForm):
    clientip = forms.CharField(required=True)