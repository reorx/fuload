CC = gcc
CXX = g++

MYFLAGS = -MMD -march=i486 -g -Wall -pthread

INC += -I./comm/ -I./jsoncpp/include
LIB += -L./jsoncpp/lib -ljsoncpp -ldl

TAR_DIR = ./
SRC_DIR = ./
OBJ_DIR = ./

TARGET = $(TAR_DIR)fl_slave_worker
STAT_TARGET = $(TAR_DIR)fl_toolstat

OBJS = \
	   $(OBJ_DIR)main.o \
	   $(OBJ_DIR)fl_slave_worker.o \
	   $(OBJ_DIR)fl_slave_input.o \
	   $(OBJ_DIR)stat.o \
	   $(OBJ_DIR)fl_slave_reporter.o \
	   $(OBJ_DIR)fl_log.o \


STAT_OBJS = $(OBJ_DIR)stat.o $(OBJ_DIR)fl_toolstat.o

OBJ_SUB_DIRS = $(sort $(dir $(OBJS) $(STAT_OBJS)))

DEPENDS = $(patsubst %.o, %.d, $(OBJS))

UNION_DIRS = jsoncpp

.PHONY: all clean init $(UNION_DIRS)

all: init $(UNION_DIRS) $(TARGET) $(STAT_TARGET)
	@echo -e "\033[1;32m\nSuccess!\033[0m"

$(UNION_DIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(TARGET): $(OBJS)
	@echo -e "\033[1;33m\nLinking $< ==> $@\033[0m"
	$(CXX) -o $@ $^ $(LIB) $(MYFLAGS)

$(STAT_TARGET): $(STAT_OBJS)
	@echo -e "\033[1;33m\nLinking $< ==> $@\033[0m"
	$(CXX) $(INC) $^ -o $@ $(LIB)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@echo -e "\033[1;33m\nCompiling $< ==> $@\033[0m"
	$(CC) $(MYFLAGS) -c -o $@ $(INC) $<

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@echo -e "\033[1;33m\nCompiling $< ==> $@\033[0m"
	$(CXX) $(MYFLAGS) -c -o $@ $(INC) $<

clean: $(UNION_DIRS)
	rm -f $(OBJS) $(STAT_OBJS) $(TARGET) $(STAT_TARGET) $(DEPENDS)

init:
	mkdir -p $(sort ./ $(TAR_DIR) $(SRC_DIR) $(OBJ_DIR) $(OBJ_SUB_DIRS))

-include $(DEPENDS)
