#
# Makefile
#
# python ipc module by Walter de Jong <walter@heiho.net>
#

CC=gcc
COPT=-g #-O2 -Winline
CFLAGS=$(COPT) -Wall
CFLAGSWRAP = $(COPT) -fpic
CFLAGSSO = $(COPT) -shared

SWIG=swig
PYTHON_INCLUDE=/usr/local/include/python2.7
PYTHON_LIB=/usr/lib/python2.6

OBJS=ipc_wrap.o

all: _ipc.so

_ipc.so: $(OBJS)
	$(CC) $(CFLAGSSO) $(OBJS) -o _ipc.so

ipc_wrap.o: ipc_wrap.c
	$(CC) $(CFLAGSWRAP) -c ipc_wrap.c -Dbool=char -I. -I$(PYTHON_INCLUDE)

ipc_wrap.c: ipc.i
	$(SWIG) -python ipc.i

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) _ipc.so ipc.py ipc.pyc ipc_wrap.c ipc_wrap.doc
	
install:
	cp ipc.py _ipc.so $(PYTHON_LIB)

uninstall:
	rm $(PYTHON_LIB)/{ipc.py,_ipc.so}

