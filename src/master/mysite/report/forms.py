#!/usr/bin/python
# -*- coding: utf-8 -*-

from django import forms

class SearchReportShowForm(forms.Form):
    reportid = forms.IntegerField(required=True,label='上报ID')
    begintime = forms.DateTimeField(required=False,label='起始时间')
    endtime = forms.DateTimeField(required=False,label='终止时间')
    rtype = forms.CharField(required=True,
            label='报表类型',
            widget=forms.Select(
                choices = [
                    ('allavgnum','总每秒调用量'),
                    ('sucavgnum','成功每秒调用量'),
                    ('erravgnum','失败每秒调用量'),

                    ('allavgtime','总平均响应时间'),
                    ('sucavgtime','成功平均响应时间'),
                    ('erravgtime','失败平均响应时间'),

                    ('sucrate','成功率'),
                    ('errrate','失败率'),

                    ('alltimepie','总响应时间饼图'),
                    ('suctimepie','成功响应时间饼图'),
                    ('errtimepie','失败响应时间饼图'),
                    ('retpie','返回码饼图'),
                    ]
                )
            )
class SearchReportDataForm(SearchReportShowForm):
    clientip = forms.CharField(required=True)
