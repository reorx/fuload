#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
import os
import subprocess
from subprocess import Popen
import signal
from os.path import abspath, dirname, join

from daemon import Daemon
from fl_slave_wkmng import WorkerManager

mpath = abspath(dirname(__file__))

class SlaveDaemon(Daemon):
    def _run(self):
        os.chdir(mpath)
        WorkerManager.fork(["./fl_slave_worker"],10)
        WorkerManager.wait()

if __name__ == "__main__":
    daemon = SlaveDaemon('/tmp/daemon-slave-ctrl.pid')
    #daemon = SlaveDaemon('/tmp/daemon-slave-ctrl.pid',stdin='/tmp/fl_in',stdout='/tmp/fl_out',stderr='/tmp/fl_err')
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
