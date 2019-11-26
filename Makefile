#
# Author: Michael Buchel
#

NAME := lab4
VERSION := "1.0.0"

BUILD := build

DIR := $(BUILD)/ $(BUILD)/lib/ $(BUILD)/exe/ bin/

LIB_SRC := $(wildcard src/lib/*.S) $(wildcard src/lib/*.c) $(wildcard src/lib/*.cpp)
LIB_OBJ := $(LIB_SRC:src/lib/%=$(BUILD)/lib/%.o)

EXE_SRC := $(wildcard src/exe/*.cpp)
BINARIES := $(EXE_SRC:src/exe/%.cpp=%)

ifndef VERBOSE
.SILENT:
endif

ASM := gcc
CC := gcc
CXX := g++
LD := g++

DEFS :=
GENFLAGS := -c -g -Og -I include -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare $(DEFS)
READLINE_INCLUDE :=
READLINE_LIBS :=

ifeq ($(HOSTNAME), )
	READLINE_INCLUDE := -I/nix/store/xdbld5razadh8ahxbxj8v93r426m4m5s-readline-7.0p5-dev/include/
	READLINE_LIBS := -L/nix/store/s7n8fndayx5k8m9212a8wh5vwj71qz2w-readline-7.0p5/lib/
endif

ASMFLAGS := $(GENFLAGS)
CFLAGS := $(GENFLAGS) `pkg-config --cflags portaudiocpp` $(READLINE_INCLUDE)
CXXFLAGS := $(CFLAGS)
LDFLAGS := `pkg-config --libs portaudiocpp` $(READLINE_LIBS) -lreadline -lhistory

all: lib
	$(MAKE) execs
	echo "Made all executables"

lib: $(DIR) $(LIB_OBJ)
	echo "Made $(NAME)-lib version: $(VERSION)"

execs: $(BINARIES)

clean:
	rm -rf $(DIR)
	echo "Made clean"

%/:
	mkdir -p $@

$(BUILD)/lib/%.S.o: src/lib/%.S
	echo "[ASM] assembling $?"
	$(ASM) $(ASMFLAGS) $? -o $@

$(BUILD)/lib/%.c.o: src/lib/%.c
	echo "[CC] compiling $?"
	$(CC) $(CFLAGS) $? -o $@

$(BUILD)/lib/%.cpp.o: src/lib/%.cpp
	echo "[CXX] compiling $?"
	$(CXX) $(CXXFLAGS) $? -o $@

$(BUILD)/exe/%.c.o: src/exe/%.c
	echo "[CC] compiling $?"
	$(CC) $(CFLAGS) $? -o $@

$(BUILD)/exe/%.cpp.o: src/exe/%.cpp
	echo "[CXX] compiling $?"
	$(CXX) $(CXXFLAGS) $? -o $@

%: $(BUILD)/exe/%.cpp.o
	echo "[LD] Linking $@"
	$(LD) $(LIB_OBJ) $^ $(LDFLAGS) -o bin/$@
