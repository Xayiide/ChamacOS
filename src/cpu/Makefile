export PREFIX=$(HOME)/opt/cross
export target=i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

BUILD_DIR = ../../build/cpu

CC = i686-elf-gcc

CFLAG = -std=gnu99 -ffreestanding -Wall -Wextra -Werror -g
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
