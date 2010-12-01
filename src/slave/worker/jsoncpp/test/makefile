CXX = g++

TARGET = test

C_FLAGS += -g -Wall -pthread
INC = -I../include
LIB = -L../lib -ljsoncpp

all: $(TARGET)

test:  test.o
	$(CXX) -o $@ $^  $(LIB) $(C_FLAGS)

.cpp.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cpp
.cc.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cc


clean:
	-rm -f *.o $(TARGET)
