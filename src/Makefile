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

.PHONY: all clean re qemu fqemu

FILES = boot.o system.o vga.o gdt.o idt.o isr.o pic.o irq.o timer.o kernel.o

all: 
	nasm -f elf boot.asm -o boot.o
	$(CC) $(CFLAG) $(IFLAG) -c system.c -o system.o
	$(CC) $(CFLAG) $(IFLAG) -c vga.c    -o vga.o
	$(CC) $(CFLAG) $(IFLAG) -c gdt.c    -o gdt.o
	$(CC) $(CFLAG) $(IFLAG) -c idt.c    -o idt.o
	$(CC) $(CFLAG) $(IFLAG) -c isr.c    -o isr.o
	$(CC) $(CFLAG) $(IFLAG) -c pic.c    -o pic.o
	$(CC) $(CFLAG) $(IFLAG) -c irq.c    -o irq.o
	$(CC) $(CFLAG) $(IFLAG) -c timer.c  -o timer.o
	$(CC) $(CFLAG) $(IFLAG) -c kernel.c -o kernel.o
	$(LD) -g -relocatable $(FILES) -o kernelfull.o # nuevo
	$(CC) $(CFLAG) -T linker.ld kernelfull.o -o kernel.bin
#$(LD) -T linker.ld $(FILES) -o kernel.bin

clean:
	rm -f $(FILES) kernelfull.o
	rm -f kernel.bin

re: clean all

qemu: re
	$(QEMU) kernel.bin $(QOPT)

fqemu: re
	$(QEMU) kernel.bin
