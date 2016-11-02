# Compiler settings
CC = gcc
CFLAGS = -std=c99

# Commands
MAKE_DIR = @mkdir -p

# Generic paths
SRC = src
BUILD = build
LIB = lib
BIN = bin

# Lib Resistance definitions
LIB_RESISTANCE_PATH = ./libresistance
LIB_RESISTANCE_SO = libresistance.so
LIB_RESISTANCE_SRC = libresistance.c

# Lib Power definitions
LIB_POWER_PATH = ./libpower
LIB_POWER_SO = libpower.so
LIB_POWER_SRC = libpower.c

# Lib Component definitions
LIB_COMPONENT_PATH = ./libcomponent/$(SRC)
LIB_COMPONENT_SO = libcomponent.so
LIB_COMPONENT_SRC = libcomponent.c

# Electrotest definitions
ELECTROTEST_PATH = ./electrotest
ELECTROTEST_SRC = electrotest.c
ELECTROTEST_BIN = electrotest
ELECTROTEST_OBJ = electrotest.o

# Other definitions
LIB_RESISTANCE_OBJ = $(LIB_RESISTANCE_SRC:.c=.o)

VPATH = $(LIB_RESISTANCE_PATH):$(LIB_POWER_PATH):$(LIB_COMPONENT_PATH)

lib : $(LIB_RESISTANCE_SO) $(LIB_POWER_SO) $(LIB_COMPONENT_SO)

%.so : %.c
	$(MAKE_DIR) $(BUILD) $(LIB)
	$(CC) -c $(CFLAGS) -fpic -o $(BUILD)/$(*:.c=.o) $^
	$(CC) $(CFLAGS) -shared -o $(LIB)/$@ $(BUILD)/$(*:.c=.o)

all : $(LIB_RESISTANCE_SO) $(LIB_POWER_SO) $(LIB_COMPONENT_SO) $(ELECTROTEST_SRC)
	$(MAKE_DIR) $(BIN)
	$(CC) $(CFLAGS) -L$(LIB) -Wl,-rpath,../$(LIB) -o $(BIN)/$(ELECTROTEST_BIN) $(BUILD)/$(ELECTROTEST_OBJ) -lresistance -lcomponent -lpower -lm

$(ELECTROTEST_SRC) :
	$(CC) -c $(CFLAGS) -fpic $(ELECTROTEST_PATH)/$(ELECTROTEST_SRC) -o $(BUILD)/$(ELECTROTEST_OBJ)