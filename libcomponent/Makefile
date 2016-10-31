# Makefile to essentially perform the following step in a very complicated manner 
# > gcc -c -fpic component.c
# > gcc -shared -o libcomponent.so component.o
# > gcc -L. -Wl,-rpath=. -o test_component test_component.c -lcomponent -lm

# Compiler settings
CC = gcc
DEBUG = 0

# Commands
MAKE_DIR = @mkdir -p

# Name of binaries
TEST_BIN_NAME = test_libcomponent
SHARED_LIB_NAME = libcomponent.so
COMPONENT_LIB_LINKER_DIRECTIV = -$(patsubst lib%,l%,$(patsubst %.so,%,$(SHARED_LIB_NAME)))

# Name of source files
TEST_COMPONENT_SRC_NAME = test_component.c
COMPONENT_SRC_NAME = libcomponent.c

# Raw paths
SRC = src
BUILD = build

# Paths to artifacts
COMPONENT_SHARED_LIB = $(BUILD)/$(SHARED_LIB_NAME)
COMPONENT_SRC = $(SRC)/$(COMPONENT_SRC_NAME)
COMPONENT_OBJ = $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(COMPONENT_SRC))
TEST_COMPONENT_BIN = $(BUILD)/$(TEST_BIN_NAME)
TEST_COMPONENT_SRC = $(SRC)/$(TEST_COMPONENT_SRC_NAME)
TEST_COMPONENT_OBJ = $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(TEST_COMPONENT_SRC))

# C flags
CFLAGS = -fpic -shared
SHARED_LIB_FLAGS = -L$(BUILD) -Wl,-rpath,.
DEBUG_FLAG = -DDEBUG=$(DEBUG)

# Make shared lib
lib : $(COMPONENT_SHARED_LIB)

# Make executable test
test : $(TEST_COMPONENT_BIN)

# Make exeutable test from object file and shared library
$(TEST_COMPONENT_BIN) : $(TEST_COMPONENT_OBJ) $(COMPONENT_SHARED_LIB)
	$(CC) $(SHARED_LIB_FLAGS) -o $(TEST_COMPONENT_BIN) $< $(COMPONENT_LIB_LINKER_DIRECTIV) -lm

# Make shared lib from object file
$(COMPONENT_SHARED_LIB) : $(COMPONENT_OBJ)
	$(CC) $(CFLAGS) -o $(COMPONENT_SHARED_LIB) $^

# Compile library source files into object file
$(COMPONENT_OBJ) : $(COMPONENT_SRC)
	$(MAKE_DIR) $(BUILD)
	$(CC) -c $(CFLAGS) $(DEBUG_FLAG) $^ -o $@

# Compile test source file into object file
$(TEST_COMPONENT_OBJ): $(TEST_COMPONENT_SRC)
	$(MAKE_DIR) $(BUILD)
	$(CC) -c $^ -o $@




