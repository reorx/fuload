#!/usr/bin/python
# -*- coding: utf-8 -*-
import subprocess
from subprocess import Popen
import signal

childs = []

def handler(signo, frame):
    global childs
    for child in childs:
        try:
            child.send_signal(signal.SIGINT)
        except:
            pass

def main():
    global childs

    for i in range(0,10):
        p = Popen(["./fl_slave_worker"])
        childs.append(p)

    signal.signal(signal.SIGINT, handler)

    #for child in childs:
        #child.send_signal(signal.SIGUSR1)

    for child in childs:
        child.wait()

if __name__ == "__main__":
    main()
