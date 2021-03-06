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

# Installation paths
USR_BIN = /usr/bin
USR_LIB = /usr/lib

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

VPATH = $(BIN) $(LIB) $(LIB_RESISTANCE_PATH) $(LIB_POWER_PATH) $(LIB_COMPONENT_PATH) $(ELECTROTEST_PATH) $(BUILD)

.PHONY : lib 
lib : $(LIB_RESISTANCE_SO) $(LIB_POWER_SO) $(LIB_COMPONENT_SO)

.PHONY : all
all : $(LIB_RESISTANCE_SO) $(LIB_POWER_SO) $(LIB_COMPONENT_SO) $(BIN)/$(ELECTROTEST_BIN)

.PHONY : install
install : all
	cp -f $(BIN)/$(ELECTROTEST_BIN) $(USR_BIN)
	cp -f $(LIB)/$(LIB_RESISTANCE_SO) $(USR_LIB)
	cp -f $(LIB)/$(LIB_POWER_SO) $(USR_LIB)
	cp -f $(LIB)/$(LIB_COMPONENT_SO) $(USR_LIB)
	sudo make clean

.PHONY : uninstall
uninstall:
	rm -f $(USR_BIN)/$(ELECTROTEST_BIN)
	rm -f $(USR_LIB)/$(LIB_RESISTANCE_SO)
	rm -f $(USR_LIB)/$(LIB_POWER_SO)
	rm -f $(USR_LIB)/$(LIB_COMPONENT_SO)

.PHONY : clean
clean:
	rm -rf $(BUILD) $(BIN) $(LIB)

%.so : %.c
	$(MAKE_DIR) $(BUILD) $(LIB)
	$(CC) -c $(CFLAGS) -fpic -o $(BUILD)/$(*:.c=.o) $^
	$(CC) $(CFLAGS) -shared -o $(LIB)/$@ $(BUILD)/$(*:.c=.o)

$(BIN)/$(ELECTROTEST_BIN) : $(ELECTROTEST_OBJ)
	$(MAKE_DIR) $(BIN)
	$(CC) $(CFLAGS) -L$(LIB) -Wl,-rpath,../$(LIB) -o $@ $(BUILD)/$(ELECTROTEST_OBJ) -lresistance -lcomponent -lpower -lm

$(ELECTROTEST_OBJ) : $(ELECTROTEST_SRC)
	$(MAKE_DIR) $(BUILD)
	$(CC) -c $(CFLAGS) -fpic $^ -o $(BUILD)/$@
