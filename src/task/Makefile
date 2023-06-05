export PREFIX=$(HOME)/opt/cross
export target=i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

BUILD_DIR = ../../build/task

CC = i686-elf-gcc

CFLAG = -std=gnu99 -ffreestanding -Wall -Wextra -Werror -g
IFLAG = -I../

TASK_SRCS := $(wildcard *.c)
TASK_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(TASK_SRCS))

.PHONY: all clean re

all: $(TASK_OBJS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAG) $(UFLAG) $(IFLAG) -c $< -o $@

clean:
	rm -f $(TASK_OBJS)

re: clean all
