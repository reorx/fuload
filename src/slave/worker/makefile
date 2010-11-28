CXX = g++

TARGET = fl_slave_worker

C_FLAGS += -ldl -g -Wall -pthread

all: $(TARGET)

fl_slave_worker:  main.o fl_slave_worker.o fl_slave_input.o
	$(CXX) -o $@ $^  $(LIB) $(C_FLAGS)

.cpp.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cpp
.cc.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cc


clean:
	-rm -f *.o $(TARGET)
