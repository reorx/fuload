CC = gcc
CXX = g++
CFLAGS  = -Wall -pipe -DDEBUG -D_NEW_LIC -g -D_GNU_SOURCE \
		  -shared -D_REENTRANT -fPIC -pthread \

		  #-shared -D_REENTRANT -fPIC -pthread -Xlinker -export-dynamic \
		  #-shared -D_REENTRANT -fPIC -pthread

#LIB     = -L/usr/local/lib/ -lpython2.7 -ldl -lutil -lrt -lm -Xlinker -export-dynamic 
#LIB     = -L/usr/local/lib/ -lpython2.7 -ldl -lutil -lz
#LIB     = -L/usr/local/lib/ -lpython2.7 -ldl -lutil -export-dynamic
#LIB     = /usr/local/lib/libpython2.7.a -ldl -lutil -Xlinker -export-dynamic -rdynamic
LIB     = -L/usr/local/lib/ -lpython2.7 -ldl -lutil -Xlinker -export-dynamic -Wl,-E
INC     = -I. -I/usr/local/include/python2.7/

#LD_PRELOAD=/usr/lib/libutil.so
#export LD_PRELOAD


OO  = main.o

TARGETS = libmodule.so

all: $(TARGETS)

$(TARGETS): $(OO)
	$(CXX) $(CFLAGS) $(INC) $(OO) -o $@ $(LIB)


.c.o:
	$(CC)  $(CFLAGS) -c $(INC) $<
	echo $@

.cpp.o:
	$(CXX) $(CFLAGS) -c $(INC) $<
	echo $@

%:%.c
	$(CC) $(CFLAGS) -o $@ $< $(OO)
	echo $@

clean:
	rm -f *.o
	rm -f $(TARGETS)
