#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        views.py
#  Description:     视图
#  Version:         1.0
#  LastChange:      2010-12-13 11:39:52
#  History:         
#=============================================================================
'''
import urllib
import random
import copy
import datetime
import time
try:
    import json
except ImportError:
    import simplejson as json

from django.http import HttpResponse
from django.shortcuts import render_to_response
from django.template.loader import get_template
from django.template import Context

from models import StatDetail
from report_upload_handler import ReportUploadHandler
from forms import SearchReportShowForm,SearchReportDataForm
from comm_def import rtype2attr
from report_show_handler import get_show_handler

def HandleReportUpload(request,reportId):
    #if request.method != 'POST':
        #return HttpResponse(json.dumps({"ret":-1,"msg":"please use post method"}))

    if 'reportinfo' not in request.POST:
        return HttpResponse(json.dumps({"ret":-1,"msg":"no reportinfo in postdata"}))
    slaveReport = request.POST['reportinfo'];

    #slaveReport = {
            #'allTimeMsStat':1000,
            #'sucTimeMsStat':900,
            #'errTimeMsStat':100,
            #'allReqNum':10,
            #'sucReqNum':8,
            #'errReqNum':2,
            #'alltimemap':{
                #5:10,
                #10:15,
                #100:20
                #},
            #'suctimemap':{
                #5:10,
                #10:1,
                #100:2
                #},
            #'errtimemap':{
                #5:10,
                #10:2,
                #100:10
                #},
            #'retmap':{
                #0:1000,
                #1:10,
                #1000:22
                #},
            #'rettimemap':{
                #0:10000,
                #1:200,
                #1000:2200
                #}
            #}
    #slaveReport = json.dumps(slaveReport)

    #such as 127.0.0.1
    clientIp = request.META['REMOTE_ADDR']
    handler = ReportUploadHandler(int(reportId), clientIp, slaveReport)
    ret = handler.handle_report()
    if ret is False:
        return HttpResponse(json.dumps({"ret":-1,"msg":"set_report failed"}))
    return HttpResponse(json.dumps({"ret":0}))

def HttpReportData(request):
    form = SearchReportDataForm(request.GET)
    if not form.is_valid():
        return HttpResponse('error input')

    cd = form.cleaned_data
    clientip = cd['clientip']
    rtype = request.GET['rtype']

    begintime = cd['begintime']

    if 'endtime' not in cd or cd['endtime'] is None:
        t = datetime.timedelta(hours=24)
        endtime = begintime + t
        cd['endtime'] = endtime
    else:
        endtime = cd['endtime']

    handler_obj = get_show_handler(rtype2attr[rtype]['swftype'], cd)
    render_data = handler_obj.get_render_data()
    t = get_template(handler_obj.get_data_file())

    html = t.render(Context(
        {
            'render_data':render_data,
            'begintime':begintime.strftime('%Y-%m-%d %H:%M'),
            'endtime':endtime.strftime('%Y-%m-%d %H:%M'),
            'clientip':clientip,
            }
        ))
    return HttpResponse(html,mimetype='application/xml')

def HttpReportShow(request):
    date_today = datetime.date.today()

    form_data = {}
    for k,v in request.GET.items():
        if v is not None:
            form_data[k] = v

    #这里主要是自动填上初始化的数据
    if 'begintime' not in form_data or form_data['begintime'] is None or len(form_data['begintime']) <= 0:
        form_data['begintime'] = datetime.datetime(*(date_today.timetuple()[:6]))
        form_data['begintime'] = form_data['begintime'].strftime("%Y-%m-%d %H:%M")

    #不要把这个时间生成更方便一些，默认就是后面的一整天的数据
    #if 'endtime' not in form_data or form_data['endtime'] is None or len(form_data['endtime']) <= 0:
        #t_begintime = datetime.datetime.strptime(form_data['begintime'],"%Y-%m-%d %H:%M")
        #form_data['endtime'] = datetime.datetime(*(t_begintime + datetime.timedelta(hours=24).timetuple()[0:6]))
        #form_data['endtime'] = form_data['endtime'].strftime("%Y-%m-%d %H:%M")

    form = SearchReportShowForm(form_data)

    if not form.is_valid():
        return render_to_response('show/show.html',{'form':form})
    cd = form.cleaned_data

    begintime = cd['begintime']

    if 'endtime' not in cd or cd['endtime'] is None:
        t = datetime.timedelta(hours=24)
        endtime = begintime + t
        cd['endtime'] = endtime
    else:
        endtime = cd['endtime']

    #生成获取数据的url
    base_data_url = '/report/data/?r='+str(random.random())
    for k,v in request.GET.items():
        if k in ('begintime','endtime'):
            continue
        if v is not None:
            base_data_url += ('&'+k+'='+v)
    #这里用timestamp来获取data，主要是因为前段swf要求xml_file的url里面不能有空格而2020-1-1 2:2精确到分就会出现空格
    base_data_url += ('&begintime='+str(int(time.mktime(begintime.timetuple()))))
    base_data_url += ('&endtime='+str(int(time.mktime(endtime.timetuple()))))


    rtype = request.GET['rtype']
    swftype = rtype2attr[rtype]['swftype']
    handler_obj = get_show_handler(swftype, cd)
    swffile = handler_obj.get_swf_file()

    clientIps = StatDetail.objects.values("clientIp").distinct()

    listData = []

    #swf类型映射最短长度(<)
    swftype_len_dict = {
            'line':2,
            'pie':1,
    }
    for ip_dict in clientIps:
        ip = ip_dict['clientIp']
        if ip is None or len(ip) == 0:
            continue
        tmpcd = copy.deepcopy(cd)
        tmpcd['clientip'] = ip

        handler_obj = get_show_handler(swftype, tmpcd)
        tmpdata = handler_obj.get_data()
        if swftype in swftype_len_dict:
            if len(tmpdata) < swftype_len_dict[swftype]:
                continue
        flash_width = handler_obj.get_swf_width()

        data_url = base_data_url + '&clientip=' + ip
        quote_data_url = urllib.quote(data_url)

        listData.append(
                {'ip':ip, 'data_url':data_url,'quote_data_url':quote_data_url,'flash_width':flash_width}
                )

    return render_to_response('show/show.html',{'listData':listData,'swffile':swffile,'form':form})
