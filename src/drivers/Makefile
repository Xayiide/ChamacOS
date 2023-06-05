export PREFIX=$(HOME)/opt/cross
export target=i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

BUILD_DIR = ../../build/drivers

CC = i686-elf-gcc

CFLAG = -std=gnu99 -ffreestanding -Wall -Wextra -Werror -g
IFLAG = -I../

DRV_SRCS := $(wildcard *.c)
DRV_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(DRV_SRCS))

.PHONY: all clean re

all: $(DRV_OBJS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAG) $(UFLAG) $(IFLAG) -c $< -o $@

clean:
	rm -f $(DRV_OBJS)

re: clean all
