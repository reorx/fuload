#!/usr/bin/python
# -*- coding: utf-8 -*-

#启动的worker进程数目
WORKER_NUM = 2

#每个worker每秒速度限制(-1为不限制)
LIMIT_SPEED = -1

#worker进程的输入文件
INPUT_FILE = "../module/fl_input.txt"

#worker进程需要加载的so
SO_FILE = "../module/libmodule.so"

#worker进程的上报间隔(sec)
REPORT_TIMESEC = 10

#上报的URL
REPORT_URL = 'http://fuload.qq.com/report/upload/1/'
