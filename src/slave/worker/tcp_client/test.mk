CXX = g++

TARGET = test

C_FLAGS += -g -Wall -pthread
INC += -I/usr/local/include
LIB += -lgtest_main -lgtest -ldl

all: $(TARGET)

test:  tcp_client.o main.o
	$(CXX) -o $@ $^  $(LIB) $(C_FLAGS)

.cpp.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cpp
.cc.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cc


clean:
	-rm -f *.o $(TARGET)
