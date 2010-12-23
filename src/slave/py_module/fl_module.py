#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        fl_module.py
#  Description:     给python用的module主文件
#  Version:         1.0
#  LastChange:      2010-12-13 18:45:10
#  History:         
#=============================================================================
'''
import urllib

def fuload_handle_init():
    print 'init'
    return 0

def fuload_handle_process(mapParams):
    #print repr(mapParams)
    try:
        from opentest import basetest,paytest
    except Exception,e:
        pass
    else:
        return paytest()

    print urllib.urlopen("http://www.vimer.cn").read()
    return 0

def fuload_handle_fini():
    print 'fini'
    return 0
if __name__ == '__main__':
    fuload_handle_process("")
