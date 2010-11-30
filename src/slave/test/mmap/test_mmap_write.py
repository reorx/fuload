#!/usr/bin/python
# -*- coding: utf-8 -*-
import mmap

wtext = 'just soso haha'
f = file('hello.txt','r+b')
f.truncate(len(wtext))
map = mmap.mmap(f.fileno(), len(wtext))
map.write(wtext)
map.flush()
