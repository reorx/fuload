#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        fl_slave_conf.py
#  Description:     主要配置文件
#  Version:         1.0
#  LastChange:      2010-12-16 15:53:56
#  History:         
#=============================================================================
'''
import logging

#=============================================================================
#ctrl相关

#ctrl log的等级
CTRL_LOG_LEVEL = logging.ERROR

#ctrl log文件的路径
CTRL_LOG_FILE = '../log/ctrl.log'

#上报的URL
REPORT_URL = 'http://fuload.qq.com/report/upload/3/'

#=============================================================================
#worker相关

#worker进程的路径(默认不用修改)
WORKER_FILE = './fl_slave_worker'

#启动的worker进程数目
WORKER_NUM = 2

#worker进程需要加载的so
#SO_FILE = "../module/libmodule.so"
SO_FILE = "../py_module/libmodule.so"
#SO_FILE = "/home/dantezhu/appbase/application/platform/third_party_api/server/spp_openlogin/src/tools/fl_so/login.so"

#worker进程的输入文件
INPUT_FILE = "../module/fl_input.txt"
#INPUT_FILE = "/home/dantezhu/appbase/application/platform/third_party_api/server/spp_openlogin/src/tools/fl_so/login_input.txt"

#每个worker每秒速度限制(-1为不限制)
LIMIT_SPEED = 50

#worker进程的上报间隔(sec)
REPORT_TIMESEC = 10

#worker进程统计文件的路径(默认不用修改)
WORKER_STATFILE = './fl_statfile'

#worker log的等级(ALL:0,DEBUG:2,ERROR:5)
WORKER_LOG_LEVEL = 5

#worker log文件的路径
WORKER_LOG_FILE = '../log/worker'

#worker log最大大小(100M)
WORKER_LOG_MAXSIZE = 104857600

#=============================================================================
