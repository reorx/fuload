#!/usr/bin/python
# -*- coding: utf-8 -*-
'''
#=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        fl_slave_ctrl.py
#  Description:     主程序文件，程序的骨架逻辑
#  Version:         1.0
#  LastChange:      2010-12-13 11:36:31
#  History:         
#=============================================================================
'''
from os.path import abspath, dirname, join
import logging
import random
import time
import signal

from ipc import ipc

from fl_slave_conf import REPORT_URL,CTRL_LOG_FILE,CTRL_LOG_LEVEL
from fl_slave_conf import WORKER_NUM,INPUT_FILE,SO_FILE,REPORT_TIMESEC,LIMIT_SPEED,WORKER_FILE,WORKER_STATFILE,WORKER_LOG_LEVEL,WORKER_LOG_FILE,WORKER_LOG_MAXSIZE
from fl_slave_wkmng import WorkerManager
from fl_slave_msg import SlaveMsg
from fl_slave_reporter import SlaveReporter

logging.basicConfig(level=CTRL_LOG_LEVEL,
        format='%(asctime)s %(levelname)s %(message)s',
        filename=CTRL_LOG_FILE,
        filemode='a+')

def handler_signal(signo, frame):
    if signo in (signal.SIGINT,signal.SIGTERM):
        SlaveCtrl.bRun  = False
    WorkerManager.send_signal(signo)

class SlaveCtrl(object):
    _slaveMsg = None
    _slaveReporter = None

    #类变量
    bRun = True

    def __init__(self):
        self._slaveReporter = SlaveReporter(REPORT_URL)
        self.signalhandler_reg()

    def signalhandler_reg(self):
        signal.signal(signal.SIGINT, handler_signal)
        signal.signal(signal.SIGTERM, handler_signal)

    def createMsgQKey(self):
        '''
        找一个没有用过的msgkey
        '''
        MaxKey = 0x0000FFFF

        getOne = False
        while not getOne:
            ipc_key = random.randint(0x01,MaxKey)
            msg_id = ipc.msgget(ipc_key,0666)
            if 0 > msg_id:
                #find it!
                return ipc_key

    def start(self):
        msgQKey = self.createMsgQKey()
        if msgQKey < 0:
            return -1;
        self._slaveMsg = SlaveMsg(msgQKey)
        WorkerManager.fork(
                [ 
                    WORKER_FILE,
                    "-i"+INPUT_FILE,
                    "-r"+str(REPORT_TIMESEC),
                    "-s"+SO_FILE,
                    "-m"+str(msgQKey),
                    '-l'+str(LIMIT_SPEED),
                    "-t"+WORKER_STATFILE,
                    "-e"+str(WORKER_LOG_LEVEL),
                    "-g"+WORKER_LOG_FILE,
                    "-z"+str(WORKER_LOG_MAXSIZE),
                    ],
                WORKER_NUM
                )
        while SlaveCtrl.bRun:
            msg = self._slaveMsg.recv()
            while msg is not None and len(msg)>0:
                self._slaveReporter.report(msg)
                msg = self._slaveMsg.recv()
            time.sleep(1)
        WorkerManager.wait();

if __name__ == "__main__":
    srv = SlaveCtrl()
    srv.start()
