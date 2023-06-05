export PREFIX=$(HOME)/opt/cross
export target=i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

BUILD_DIR = ../../build/mm

CC = i686-elf-gcc

CFLAG = -std=gnu99 -ffreestanding -Wall -Wextra -Werror -g
IFLAG = -I../

MEM_SRCS := $(wildcard *.c)
MEM_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(MEM_SRCS))

.PHONY: all clean re

all: $(MEM_OBJS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAG) $(UFLAG) $(IFLAG) -c $< -o $@

clean:
	rm -f $(MEM_OBJS)

re: clean all
