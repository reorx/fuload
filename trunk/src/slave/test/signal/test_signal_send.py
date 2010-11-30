#!/usr/bin/python
# -*- coding: utf-8 -*-
import os
import signal

ps_output = os.popen("ps x | grep test_signal_recv").readline()
fd = ps_output.strip().split(' ')[0]

print fd
os.kill(int(fd),signal.SIGUSR1)
