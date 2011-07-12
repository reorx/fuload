#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
#=============================================================================
#
#     FileName: uws_app.py
#         Desc: 这个就是用来启动uwsgi
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2011-07-12 00:29:09
#      Version: 0.0.1
#      History:
#               0.0.1 | dantezhu | 2011-07-12 00:29:09 | initialization
#
#=============================================================================
'''

import sys
import os

sys.path.append(os.path.abspath(os.path.dirname(__file__)))
os.environ['DJANGO_SETTINGS_MODULE'] = 'mysite.settings'

import django.core.handlers.wsgi

application = django.core.handlers.wsgi.WSGIHandler()
