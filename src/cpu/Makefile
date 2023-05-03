export PREFIX=$(HOME)/opt/cross
export target=i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

BUILD_DIR = ../../build/cpu

CC = i686-elf-gcc

CFLAG = -g -ffreestanding -falign-jumps -falign-functions -falign-labels      \
		-falign-loops -fstrength-reduce -fomit-frame-pointer                  \
		-finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-cpp \
		-Wno-unused-label -Wno-unused-parameter -nostdlib -nostartfiles -Wall \
		-Wextra -nodefaultlibs -O0

IFLAG = -I../

CPU_SRCS := $(wildcard *.c)
CPU_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(CPU_SRCS))

.PHONY: all clean re

all: $(CPU_OBJS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAG) $(UFLAG) $(IFLAG) -c $< -o $@

clean:
	rm -f $(CPU_OBJS)

re: clean all
