#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        fl_slave_wkmng.py
#  Description:     多进程管理类
#  Version:         1.0
#  LastChange:      2010-12-13 11:37:35
#  History:         
#=============================================================================
'''
import sys
import os
import subprocess
from subprocess import Popen
from os.path import abspath, dirname, join

class WorkerManager:
    _childs = []

    @classmethod
    def fork(cls,exe_args,num):
        for i in range(0,num):
            p = Popen(exe_args)
            WorkerManager._childs.append(p)

    @classmethod
    def wait(cls):
        for child in WorkerManager._childs:
            child.wait()

    @classmethod
    def send_signal(cls,signo):
        for child in WorkerManager._childs:
            try:
                child.send_signal(signo)
            except:
                pass

if __name__ == "__main__":
    WorkerManager.fork(["./fl_slave_worker"],2)
    WorkerManager.wait()
