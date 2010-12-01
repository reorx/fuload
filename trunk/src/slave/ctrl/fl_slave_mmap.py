#!/usr/bin/python
# -*- coding: utf-8 -*-
import mmap
try:
    import json
except ImportError:
    import simplejson as json

class MMapWrapper:
    _towriteobj = {}
    _filename = ''

    def __init__(self, filename='hello.txt'):
        self._filename = filename

    def set_host(self, ip, port):
        self._towriteobj['ip'] = ip
        self._towriteobj['port'] = port

    def set_input(self, strinput):
        self._towriteobj['input'] = strinput

    def dumps(self):
        return json.dumps(self._towriteobj, ensure_ascii=False)

    def write(self):
        wbuf = self.dumps()
        with open(self._filename, "w+b") as f:
            f.truncate(len(wbuf))
            map = mmap.mmap(f.fileno(), len(wbuf))
            map.write(wbuf)
            map.close()

if __name__ == "__main__":
    m = MMapWrapper()
    m.set_host('localhost','12233')
    m.set_input('just soso')
    m.write()
