#!/usr/bin/env python
#coding:utf-8

import SocketServer
from time import ctime
import logging
import socket
import mmandeal

HOST = ''
PORT = 21567
ADDR = (HOST, PORT)
MAXBUFSIZE = 1024
logging.basicConfig(level=logging.DEBUG,
        format='%(asctime)s %(levelname)s %(message)s',
        filename='../log/log.txt',
        filemode='a+')

class MyRequestHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        #print '...connected from:', self.client_address
        while True:
            alldata = ''
            try:
                headdata = self.request.recv(MAXBUFSIZE)
                if not headdata:
                    break
                recvlen = int(headdata[0:11])
                alldata += headdata

                while True:
                    if len(alldata) == recvlen:
                        break
                    recvdata = self.request.recv(MAXBUFSIZE)
                    if(not recvdata):
                        break
                    alldata += recvdata

                if len(alldata) != recvlen:
                    self.LogErr("recvdata'len not match,should:%u,real:%u",recvlen,len(alldata))
                    continue

            except socket.timeout:
                self.LogErr("caught socket.timeout exception")

            self.request.sendall(mmandeal.DealRequest(alldata))
    def LogTemplate(self, s):
        return '[id.' + str(id(self.request)) + ']:  ' + str(s)
    def Log(self, s):
        ss =  self.LogTemplate(s)
        logging.info(ss)
    def LogErr(self, s):
        ss =  self.LogTemplate(s)
        logging.error(ss)

    def setup(self):
        self.Log('into thread')
        self.request.settimeout(60)
    def finish(self):
        self.request.close()
        self.Log("exit thread")

if __name__ == "__main__":
    tcpServ = SocketServer.ThreadingTCPServer(ADDR, MyRequestHandler)
    print 'waiting for connection...'
    tcpServ.serve_forever()
