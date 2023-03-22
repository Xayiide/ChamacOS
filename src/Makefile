export PREFIX=$(HOME)/opt/cross
export TARGET=i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

CC    = i686-elf-gcc
LD    = i686-elf-ld

#CFLAG = -g -Wall -O0 -fstrength-reduce -fomit-frame-pointer  \
#		-finline-functions -nostdinc -fno-builtin -nostdlib \
#		-nodefaultlibs -Wall -ffreestanding -nostartfiles

CFLAG = -g -ffreestanding -falign-jumps -falign-functions -falign-labels      \
		-falign-loops -fstrength-reduce -fomit-frame-pointer                  \
		-finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-cpp \
		-Wno-unused-label -Wno-unused-parameter -nostdlib -nostartfiles -Wall \
		-nodefaultlibs -O0

IFLAG = -I./inc

LFLAG = -T

QEMU = qemu-system-i386 -kernel
QOPT = -gdb tcp::26000 -S

.PHONY: all clean re qemu

all: 
	nasm -f elf boot.asm -o boot.o
	$(CC) $(CFLAG) $(IFLAG) -c system.c -o system.o
	$(CC) $(CFLAG) $(IFLAG) -c vga.c    -o vga.o
	$(CC) $(CFLAG) $(IFLAG) -c kernel.c -o kernel.o
	$(LD) -T linker.ld boot.o system.o vga.o kernel.o -o kernel.bin

clean:
	rm -f boot.o system.o vga.o kernel.o
	rm -f kernel.bin

re: clean all

qemu: re
	$(QEMU) kernel.bin $(QOPT)
