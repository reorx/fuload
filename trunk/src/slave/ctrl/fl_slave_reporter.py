#!/usr/bin/python
# -*- coding: utf-8 -*-
import urllib
import urllib2
import logging
import traceback

try:
    import json
except ImportError:
    import simplejson as json

class SlaveReporter(object):
    _reportUrl= ""

    def __init__(self, reportUrl):
        self._reportUrl = reportUrl

    def report(self,data):
        #logging.error(data)
        req = urllib2.Request(self._reportUrl)
        params = {"reportinfo":data}
        en_params = urllib.urlencode(params)
        try:
            readdata = urllib2.urlopen(req,en_params).read()
            obj = json.loads(readdata)
            if obj['ret'] != 0:
                logging.error("report error,ret:%d,msg:%s",obj['ret'],obj['msg'])
                return False
        except Exception, ex:
            logging.error("urllib2 exception:"+ repr(ex) + traceback.format_exc() + ",data:" +data)
            return False
        else:
            return True
