export PREFIX=$(HOME)/opt/cross
export target=i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

BUILD_DIR = ../../build/kernel

CC = i686-elf-gcc

CFLAG = -g -ffreestanding -falign-jumps -falign-functions -falign-labels      \
		-falign-loops -fstrength-reduce -fomit-frame-pointer                  \
		-finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-cpp \
		-Wno-unused-label -Wno-unused-parameter -nostdlib -nostartfiles -Wall \
		-Wextra -nodefaultlibs -O0

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
