#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
import os
import subprocess
from subprocess import Popen
import signal
from os.path import abspath, dirname, join
import thread,threading
import time
import logging
import SocketServer

from fl_slave_wkmng import WorkerManager
from fl_slave_srv import MyRequestHandler
from fl_slave_conf import WORKER_NUM,INPUT_FILE,SO_FILE,REPORT_TIMESEC
from fl_slave_mmap import MMapWrapper

class SlaveCtrl(object):
    _msgQueueKey = ""

    def __init__(self):
        self._msgQKey = 1000

    def start(self):
        WorkerManager.fork(
                [ 
                    "./fl_slave_worker",
                    "-i"+INPUT_FILE,
                    "-r"+str(REPORT_TIMESEC),
                    "-s"+SO_FILE,
                    "-m"+str(self._msgQKey),
                    ],
                WORKER_NUM
                )
        WorkerManager.wait();

if __name__ == "__main__":
    srv = SlaveCtrl()
    srv.start()
