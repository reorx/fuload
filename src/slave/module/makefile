CC = gcc
CXX = g++
MYFLAGS = -MMD -Wall -pipe -g -D_GNU_SOURCE -shared -D_REENTRANT -fPIC

INC += -I.
LIB += -ldl -lrt

TAR_DIR = ./
SRC_DIR = ./
OBJ_DIR = ./obj/

RELEASE_DIR = ./

TARGET = $(TAR_DIR)libmodule.so

OBJS = \
	   $(OBJ_DIR)main.o \


OBJ_SUB_DIRS = $(sort $(dir $(OBJS)))

DEPENDS = $(patsubst %.o, %.d, $(OBJS))

.PHONY: all clean init

all: init $(TARGET)
	@echo -e "\033[1;32m\nSuccess!\033[0m"
	ldd -r $(TARGET)

$(TARGET): $(OBJS)
	@echo -e "\033[1;33m\nLinking $< ==> $@\033[0m"
	$(CXX) $(MYFLAGS) $(INC) $^ -o $@ $(LIB)
ifneq ($(TAR_DIR), $(RELEASE_DIR))
	cp $@ $(RELEASE_DIR)
endif

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@echo -e "\033[1;33m\nCompiling $< ==> $@\033[0m"
	$(CC) $(MYFLAGS) -c -o $@ $(INC) $<

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@echo -e "\033[1;33m\nCompiling $< ==> $@\033[0m"
	$(CXX) $(MYFLAGS) -c -o $@ $(INC) $<

clean:
	rm -f $(OBJS) $(TARGET) $(DEPENDS)

init:
	mkdir -p $(sort ./ $(TAR_DIR) $(SRC_DIR) $(OBJ_DIR) $(RELEASE_DIR) $(OBJ_SUB_DIRS))

-include $(DEPENDS)
