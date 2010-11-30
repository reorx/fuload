#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
import os
import subprocess
from subprocess import Popen
import signal
from os.path import abspath, dirname, join

from daemon import Daemon

childs = []

mpath = abspath(dirname(__file__))

def handler(signo, frame):
    global childs
    for child in childs:
        try:
            child.send_signal(signo)
        except:
            pass

def fork_wait():
    os.chdir(mpath) 
    global childs

    for i in range(0,10):
        p = Popen(["./fl_slave_worker"])
        childs.append(p)

    signal.signal(signal.SIGINT, handler)
    signal.signal(signal.SIGTERM, handler)

    #for child in childs:
        #child.send_signal(signal.SIGUSR1)

    for child in childs:
        child.wait()

class SlaveDaemon(Daemon):
    def _run(self):
        fork_wait()

if __name__ == "__main__":
    daemon = SlaveDaemon('/tmp/daemon-slave-ctrl.pid',stdin='/tmp/fl_in',stdout='/tmp/fl_out',stderr='/tmp/fl_err')
    if len(sys.argv) == 2:
        if 'start' == sys.argv[1]:
            daemon.start()
        elif 'stop' == sys.argv[1]:
            daemon.stop()
        elif 'restart' == sys.argv[1]:
            daemon.restart()
        else:
            print "Unknown command"
            sys.exit(2)
        sys.exit(0)
    else:
        print "usage: %s start|stop|restart" % sys.argv[0]
        sys.exit(2)
