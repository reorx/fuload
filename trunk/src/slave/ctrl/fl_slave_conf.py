#!/usr/bin/python
# -*- coding: utf-8 -*-

#启动的worker进程数目
WORKER_NUM = 10

#worker进程的输入文件
INPUT_FILE = "./fl_input.txt"

#worker进程需要加载的so
SO_FILE = "../../module/libmodule.so"

#worker进程的上报间隔(sec)
REPORT_TIMESEC = 300

#上报的ID，用来区分统计
REPORT_ID = 1

#上报的URL
REPORT_URL = 'http://10.6.207.119/report/upload'
