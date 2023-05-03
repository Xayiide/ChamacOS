CC = i686-elf-gcc
LD = i686-elf-ld

CFLAG = -g -ffreestanding -falign-jumps -falign-functions -falign-labels      \ 
        -falign-loops -fstrength-reduce -fomit-frame-pointer                  \ 
        -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-cpp \ 
        -Wno-unused-label -Wno-unused-parameter -nostdlib -nostartfiles -Wall \ 
        -nodefaultlibs -O0

IFLAG = -I./inc
LFLAG = -T

OBJS = io.o kb.o kernel.o sys.o vga.o

all: $(OBJS)
