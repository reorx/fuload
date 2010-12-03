#!/usr/bin/env python
import sys,ipc
 
if len(sys.argv)==5 and sys.argv[3][0]=='s':
    ipc_key=int(sys.argv[1])
    msg_id = ipc.msgget(ipc_key,0666|ipc.IPC_CREAT)
    if 0 > msg_id:
        sys.exit(1)
    mbuf = ipc.msgbuf()
    mbuf.mtype = int(sys.argv[2])
    mbuf.mtext = sys.argv[4]
    if 0 > ipc.msgsnd(msg_id,mbuf,len(mbuf.mtext),0):
        sys.exit(3)
    print 'Send Success.'
elif len(sys.argv)==4 and sys.argv[3][0]=='r':
    ipc_key=int(sys.argv[1])
    mbuf = ipc.msgbuf()
    msg_id = ipc.msgget(ipc_key,0666)
    if 0 > msg_id:
        sys.exit(1)
    msg_len=ipc.msgrcv(msg_id,mbuf,2048,int(sys.argv[2]),ipc.IPC_NOWAIT)
    if 0 > msg_len:
        print 'No message received.'
        sys.exit(3)
    else:
        print 'Recv Success.(%d bytes):'% msg_len
        print mbuf.mtext
elif len(sys.argv)==3 and sys.argv[2][0]=='c':
    ipc_key=int(sys.argv[1])
    id_dsp = ipc.msqid_ds()
    msg_id = ipc.msgget(ipc_key,0666)
    if 0 > msg_id:
        sys.exit(1)
    if 0 > ipc.msgctl(msg_id,ipc.IPC_RMID,id_dsp):
        sys.exit(2)
else:
    print "usage: \n%s key type s message --to send message\n\
%s key type r --to receive\n\
%s key c --to clear queue"\
        %(sys.argv[0],sys.argv[0],sys.argv[0])
