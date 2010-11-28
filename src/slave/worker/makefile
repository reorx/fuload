CXX = g++

TARGET = slave_worker

C_FLAGS += -g -Wall -pthread

all: $(TARGET)

slave_worker:  main.o slave_worker.o
	$(CXX) -o $@ $^  $(LIB) $(C_FLAGS)

.cpp.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cpp
.cc.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cc


clean:
	-rm -f *.o $(TARGET)
