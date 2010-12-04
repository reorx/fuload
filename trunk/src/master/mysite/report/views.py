#!/usr/bin/python
# -*- coding: utf-8 -*-
import urllib
import random
try:
    import json
except ImportError:
    import simplejson as json

from django.http import HttpResponse
from django.shortcuts import render_to_response

from models import StatDetail
from report_upload_handler import ReportUploadHandler
from forms import SearchReportShowForm,SearchReportDataForm
from comm_func import get_report_data

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

    begintime = ''
    if 'begintime' in cd and cd['begintime'] is not None:
        begintime = cd['begintime']

    endtime = ''
    if 'endtime' in cd and cd['endtime'] is not None:
        endtime = cd['endtime']

    objs = get_report_data(cd)

    return render_to_response('show/line_data.xml',
            {'objs':objs,'begintime':begintime,'endtime':endtime,'clientip':clientip,'header':u'总每秒请求量'}
            )

def HttpReportShow(request):
    form = SearchReportShowForm(request.GET)
    if not form.is_valid():
        return render_to_response('show/show.html',{'form':form})

    cd = form.cleaned_data
    objs = get_report_data(cd)
    if len(objs) <= 0:
        return render_to_response('show/show.html',{'form':form})

    swffile = ''
    rtype = request.GET['rtype']
    if rtype in ('alltimepie','suctimepie','errtimepie','retpie'):
        swffile = ''
    else:
        swffile = 'fcp-line-chart.swf'

    clientIps = StatDetail.objects.values("clientIp").distinct()

    base_data_url = '/report/data?r='+str(random.randint(0,10000))
    for k,v in request.GET.items():
        if v is not None:
            base_data_url += ('&'+k+'='+v)

    listData = []
    for ip_dict in clientIps:
        ip = ip_dict['clientIp']
        if ip is None or len(ip) == 0:
            continue
        data_url = base_data_url + '&clientip=' + ip
        data_url = urllib.quote(data_url)
        listData.append(
                {'ip':ip, 'data_url':data_url}
                )

    return render_to_response('show/show.html',{'listData':listData,'swffile':swffile,'form':form})
