#!/usr/bin/python
# -*- coding: utf-8 -*-

#启动的worker进程数目
WORKER_NUM = 2

#worker进程的输入文件
INPUT_FILE = "./fl_input.txt"

#worker进程需要加载的so
SO_FILE = "../module/libmodule.so"

#worker进程的上报间隔(sec)
REPORT_TIMESEC = 10

#上报的URL
REPORT_URL = 'http://dante.qq.com/report/upload/1/'
