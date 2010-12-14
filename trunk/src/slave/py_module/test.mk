CXX = g++

TARGET = main

C_FLAGS += -g -Wall -pthread -export-dynamic
LIB     = -L/usr/local/lib/ -lpython2.7 -ldl -lutil
INC     = -I. -I/usr/local/include/python2.7/

all: $(TARGET)

main:  main.o
	$(CXX) -o $@ $^  $(LIB) $(C_FLAGS)

.cpp.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cpp
.cc.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cc


clean:
	-rm -f *.o $(TARGET)
