#!/usr/bin/python
# -*- coding: utf-8 -*-
import urllib
try:
    import json
except ImportError:
    import simplejson as json

from django.http import HttpResponse
from django.shortcuts import render_to_response

from models import StatDetail
from report_upload_handler import ReportUploadHandler
from forms import SearchReportForm

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
    form = SearchReportForm(request.GET)
    if not form.is_valid():
        return HttpResponse('error input')

    cd = form.cleaned_data

    objs = StatDetail.objects.filter(reportId=cd['reportid'])

    if 'begintime' in cd and cd['begintime'] is not None:
        objs = objs.filter(firstTime__gte=cd['begintime'])

    if 'endtime' in cd and cd['endtime'] is not None:
        objs = objs.filter(secondTime__lte=request.GET['endtime'])

    objs.order_by('firstTime')

    return render_to_response('show/line_data.xml',{'objs':objs})

def HttpReportShow(request):
    form = SearchReportForm(request.GET)
    if not form.is_valid():
        return HttpResponse('error input')

    data_url = '/report/data?'
    for k,v in request.GET.items():
        if v is not None:
            data_url += (k+'='+v+'&')
    data_url = urllib.quote(data_url)
    return render_to_response('show/show.html',{'data_url':data_url})
