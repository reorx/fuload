#!/usr/bin/python
# -*- coding: utf-8 -*-

from ipc import ipc

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
