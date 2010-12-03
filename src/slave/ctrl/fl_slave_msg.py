#!/usr/bin/python
# -*- coding: utf-8 -*-

from ipc import ipc

MSGQUEUE_MTYPE = 1
MSGQUEUE_BUFSIZE= 4096

class SlaveMsg(object):
    _msg_id = -1

    def __init__(self,msgQKey):
        self._clear()

        self._msg_id = ipc.msgget(msgQKey,0666|ipc.IPC_CREAT)
        if self._msg_id < 0:
            raise TypeError 
        
    def __del__(self):
        self._clear()

    def _clear(self):
        if self._msg_id < 0:
            return
        id_dsp = ipc.msqid_ds()
        if 0 > ipc.msgctl(self._msg_id,ipc.IPC_RMID,id_dsp):
            return

    def recv(self):
        mbuf = ipc.msgbuf()
        msg_len=ipc.msgrcv(self._msg_id,mbuf,MSGQUEUE_BUFSIZE,MSGQUEUE_MTYPE,ipc.IPC_NOWAIT)
        if 0 > msg_len:
            return ""
        else:
            return mbuf.mtext
