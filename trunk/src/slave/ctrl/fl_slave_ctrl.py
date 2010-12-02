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
from fl_slave_conf import HOST_ADDR,WORKER_NUM,INPUT_FILE,MMAP_FILE,SO_FILE,REPORT_TIMESEC,REPORT_TIMEOUT_MS
from fl_slave_mmap import MMapWrapper

def thread_sendsignal():
    time.sleep(2)
    WorkerManager.send_signal(signal.SIGUSR1)

class SlaveCtrl(object):
    def start(self):
        WorkerManager.fork(
                [ 
                    "./fl_slave_worker",
                    "-h"+HOST_ADDR[0],
                    "-p"+str(HOST_ADDR[1]),
                    "-t"+str(REPORT_TIMEOUT_MS),
                    "-m"+MMAP_FILE,
                    "-r"+str(REPORT_TIMESEC),
                    "-s"+SO_FILE,
                    ],
                WORKER_NUM
                )

        self.write_mmap()
        self.async_sendsignal()

        tcpServ = SocketServer.ThreadingTCPServer(HOST_ADDR, MyRequestHandler)
        print 'waiting for connection...'
        tcpServ.serve_forever()

    def write_mmap(self):
        m = MMapWrapper(MMAP_FILE)
        m.set_run(1)
        m.set_input(file(INPUT_FILE,'r').read())
        m.write()

    def async_sendsignal(self):
        t = threading.Thread(target=thread_sendsignal,
                        args=[])
        t.start()

if __name__ == "__main__":
    srv = SlaveCtrl()
    srv.start()
