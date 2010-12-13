CXX = g++

TARGET = test_py

C_FLAGS += -g -Wall -pthread

INC = -I/usr/local/include/python2.7/
LIB = -L/usr/local/lib/ -lpython2.7 -ldl -lutil

all: $(TARGET)

test_py:  test_py.o
	$(CXX) -o $@ $^  $(LIB) $(C_FLAGS)

.cpp.o:
	$(CXX) -c -o $*.o $(INC) $(C_FLAGS) $*.cpp

clean:
	-rm -f *.o $(TARGET)
