CXX = g++

TARGET = fl_slave_worker fl_toolstat

C_FLAGS += -march=i486 -g -Wall -pthread

INC += -I./comm/ -I./jsoncpp/include
LIB += -L./jsoncpp/lib -ljsoncpp -ldl

dirs = jsoncpp

.PHONY: all clean $(dirs)

all: $(dirs) $(TARGET)

$(dirs):
	$(MAKE) -C $@ $(MAKECMDGOALS)

fl_slave_worker:  main.o fl_slave_worker.o fl_slave_input.o stat.o fl_slave_reporter.o fl_log.o
	$(CXX) -o $@ $^  $(LIB) $(C_FLAGS)
	cp $@ ../ctrl

fl_toolstat:  stat.o fl_toolstat.o
	$(CXX) -o $@ $^  $(LIB_FLAGS) $(LIB) $(C_FLAGS)
	cp $@ ../ctrl

.cpp.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cpp

clean: $(dirs)
	-rm -f *.o $(TARGET)
