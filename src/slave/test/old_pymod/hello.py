#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        hello.py
#  Description:     
#  Version:         1.0
#  LastChange:      2010-12-13 23:30:42
#  History:         
#=============================================================================
'''

def hi():
    import urllib
    data = urllib.urlopen('http://www.vimer.cn').read()
    print data
