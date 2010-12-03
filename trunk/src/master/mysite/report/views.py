from django.http import HttpResponse
from django.shortcuts import render_to_response

def HandleReportUpload(request,masterId):
    return HttpResponse("woaini,%s" % masterId)
