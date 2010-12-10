#!/usr/bin/python
# -*- coding: utf-8 -*-

#启动的worker进程数目
WORKER_NUM = 2

#每个worker每秒速度限制(-1为不限制)
LIMIT_SPEED = -1

#log文件的路径
LOG_FILE = './log.txt'

#worker进程的输入文件
#INPUT_FILE = "../module/fl_input.txt"
INPUT_FILE = "/home/dantezhu/baseplatform/application/ThirdPartApp/server/spp_openlogin/src/tools/fl_so/login_input.txt"

#worker进程需要加载的so
#SO_FILE = "../module/libmodule.so"
SO_FILE = "/home/dantezhu/baseplatform/application/ThirdPartApp/server/spp_openlogin/src/tools/fl_so/login.so"

#worker进程的上报间隔(sec)
REPORT_TIMESEC = 10

#上报的URL
REPORT_URL = 'http://fuload.qq.com/report/upload/2/'

#worker进程的路径(默认不用修改)
WORKER_FILE = '../fl_slave_worker'

#worker进程统计文件的路径(默认不用修改)
WORKER_STATFILE = './fl_statfile'
