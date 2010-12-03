try:
    import json
except ImportError:
    import simplejson as json

from django.http import HttpResponse
from django.shortcuts import render_to_response

from report_handler import ReportHandler

def HandleReportUpload(request,reportId):
    #if request.method != 'POST':
        #return HttpResponse(json.dumps({"ret":-1,"msg":"please use post method"}))

    #if 'reportinfo' not in request.POST:
        #return HttpResponse(json.dumps({"ret":-1,"msg":"no reportinfo in postdata"}))
    #slaveReport = request.POST['reportinfo'];

    slaveReport = {
            'allTimeMsStat':1000,
            'sucTimeMsStat':900,
            'errTimeMsStat':100,
            'allReqNum':10,
            'sucReqNum':8,
            'errReqNum':2,
            'timemap':{
                5:10,
                10:15,
                100:20
                },
            'retmap':{
                0:1000,
                1:10,
                1000:22
                }
            }
    slaveReport = json.dumps(slaveReport)

    #such as 127.0.0.1
    clientIp = request.META['REMOTE_ADDR']
    handler = ReportHandler(int(reportId), clientIp, slaveReport)
    ret = handler.handle_report()
    if ret is False:
        return HttpResponse(json.dumps({"ret":-1,"msg":"set_report failed"}))
    return HttpResponse(json.dumps({"ret":0}))

