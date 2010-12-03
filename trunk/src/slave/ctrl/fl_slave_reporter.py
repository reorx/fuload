#!/usr/bin/python
# -*- coding: utf-8 -*-
import urllib
import urllib2
import logging

try:
    import json
except ImportError:
    import simplejson as json

class SlaveReporter(object):
    _reportId = 0
    _reportUrl= ""

    def __init__(self, reportId, reportUrl):
        self._reportId = reportId
        self._reportUrl = reportUrl

    def report(self,data):
        logging.error(data)
        return
        jsonObj = json.loads(data)
        jsonObj['reportid'] = self._reportId
        newdata = json.dump(jsonObj,ensure_ascii=False)

        req = urllib2.Request(self._reportUrl)
        params = {"reportinfo":newdata}
        en_params = urllib.urlencode(params)
        try:
            data = urllib2.urlopen(req,en_params).read()
        except:
            return False
        else:
            return True
