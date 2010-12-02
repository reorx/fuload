SRC_DIR= ./
OBJ_DIR= ./
LIB_DIR= ./

OBJ_EXT= .o
CXXSRC_EXT= .cpp
CSRC_EXT= .c
LIB_EXT= .a
H_EXT= .h

OBJECTS = $(OBJ_DIR)tcp_client$(OBJ_EXT)

LIB_TARGET = $(LIB_DIR)libtcpclient$(LIB_EXT)

$(OBJ_DIR)%$(OBJ_EXT): $(SRC_DIR)%$(CXXSRC_EXT)
	@echo
	@echo "Compiling $< ==> $@..."
	$(CXX) $(INC) $(C_FLAGS) -c $< -o $@

$(OBJ_DIR)%$(OBJ_EXT): $(SRC_DIR)%$(CSRC_EXT)
	@echo
	@echo "Compiling $< ==> $@..."
	$(CC)  $(INC) $(C_FLAGS) -c $< -o $@

all:$(LIB_TARGET)

$(LIB_TARGET): $(OBJECTS)
	@echo
	$(AR) rc $(LIB_TARGET) $(OBJECTS)
	@echo "ok"

clean:
	rm -f $(LIB_TARGET) $(OBJECTS)
