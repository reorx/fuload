#!/usr/bin/python
# -*- coding: utf-8 -*-
import mmap

wtext = 'www.vimer.cn'
f = file('hello.txt','w+b')
f.truncate(len(wtext))
map = mmap.mmap(f.fileno(), len(wtext))
map.write(wtext)
map.flush()
