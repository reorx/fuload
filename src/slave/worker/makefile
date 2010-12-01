CXX = g++

TARGET = fl_slave_worker

C_FLAGS += -g -Wall -pthread

INC += -I./comm/
LIB += -ldl

all: $(TARGET) fl_toolstat

fl_slave_worker:  main.o fl_slave_worker.o fl_slave_input.o
	$(CXX) -o $@ $^  $(LIB) $(C_FLAGS)
	cp $@ ../ctrl

fl_toolstat:  stat.o fl_toolstat.o
	$(CXX) -o $@ $^  $(LIB_FLAGS) $(LIB) $(C_FLAGS)
	cp $@ ../ctrl

.cpp.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cpp
.cc.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cc


clean:
	-rm -f *.o $(TARGET) fl_toolstat