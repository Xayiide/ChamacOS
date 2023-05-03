export PREFIX=$(HOME)/opt/cross
export target=i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

BUILD_DIR = ../../build/lib

CC = i686-elf-gcc

CFLAG = -g -ffreestanding -falign-jumps -falign-functions -falign-labels      \
		-falign-loops -fstrength-reduce -fomit-frame-pointer                  \
		-finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-cpp \
		-Wno-unused-label -Wno-unused-parameter -nostdlib -nostartfiles -Wall \
		-Wextra -nodefaultlibs -O0

IFLAG = -I../

LIB_SRCS := $(wildcard *.c)
LIB_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(LIB_SRCS))

.PHONY: all clean re

all: $(LIB_OBJS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAG) $(UFLAG) $(IFLAG) -c $< -o $@

clean:
	rm -f $(LIB_OBJS)

re: clean all
