export PREFIX=$(HOME)/opt/cross
export target=i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

BUILD_DIR = ../../build/kernel

CC = i686-elf-gcc

CFLAG = -std=gnu99 -ffreestanding -Wall -Wextra -Werror -g
IFLAG = -I../

KRN_SRCS := $(wildcard *.c)
KRN_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(KRN_SRCS))

.PHONY: all clean re

all: $(KRN_OBJS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAG) $(UFLAG) $(IFLAG) -c $< -o $@

clean:
	rm -f $(KRN_OBJS)

re: clean all
