#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
#=============================================================================
#
#     FileName: uws_app.py
#         Desc: 这个就是用来启动uwsgi，放到mysite目录下就行
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

#这里一定要到上一级目录，因为setting.py里面都是用的mysite.*之类的
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__),'../')))
os.environ['DJANGO_SETTINGS_MODULE'] = 'settings'

import django.core.handlers.wsgi

app = django.core.handlers.wsgi.WSGIHandler()
