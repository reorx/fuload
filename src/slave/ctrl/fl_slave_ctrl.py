#!/usr/bin/python
# -*- coding: utf-8 -*-
from os.path import abspath, dirname, join
import logging
import random
import time
import signal

from ipc import ipc

from fl_slave_conf import WORKER_NUM,INPUT_FILE,SO_FILE,REPORT_TIMESEC,REPORT_ID,REPORT_URL
from fl_slave_wkmng import WorkerManager
from fl_slave_msg import SlaveMsg
from fl_slave_reporter import SlaveReporter

logging.basicConfig(level=logging.DEBUG,
        format='%(asctime)s %(levelname)s %(message)s',
        filename='./log.txt',
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
        self._slaveReporter = SlaveReporter(REPORT_ID,REPORT_URL)
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
                    "./fl_slave_worker",
                    "-i"+INPUT_FILE,
                    "-r"+str(REPORT_TIMESEC),
                    "-s"+SO_FILE,
                    "-m"+str(msgQKey),
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
