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
    _reportUrl= ""

    def __init__(self, reportUrl):
        self._reportUrl = reportUrl

    def report(self,data):
        logging.error(data)
        return
        req = urllib2.Request(self._reportUrl)
        params = {"reportinfo":data}
        en_params = urllib.urlencode(params)
        try:
            data = urllib2.urlopen(req,en_params).read()
        except:
            return False
        else:
            return True
