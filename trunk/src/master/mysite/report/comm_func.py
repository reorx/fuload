#!/usr/bin/python
# -*- coding: utf-8 -*-
import time
import datetime

def TimeToString(timestamp):
    t_tuple = time.localtime(timestamp)
    dt = datetime.datetime(*t_tuple[:6])
    return dt.strftime("%Y-%m-%d %H:%M")
