export PREFIX=$(HOME)/opt/cross
export target=i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

BUILD_DIR = ../../build/lib

CC = i686-elf-gcc

CFLAG = -std=gnu99 -ffreestanding -Wall -Wextra -Werror -g
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
