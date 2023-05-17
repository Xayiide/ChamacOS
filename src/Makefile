export PREFIX=$(HOME)/opt/cross
export TARGET=i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

BUILD_DIR = ../build/
KERNEL_FULL = kernelfull.o
SRC_FILES = $(shell find -type f -name "*.c") \
			$(shell find -type f -name "*.asm")
DIR_FILES = $(patsubst ./%, $(BUILD_DIR)%, $(SRC_FILES))
FILES_1   = $(patsubst %.c, %.o, $(DIR_FILES))
FILES     = $(patsubst %.asm, %.o, $(FILES_1))

CC    = i686-elf-gcc
LD    = i686-elf-ld

CFLAG = -g -ffreestanding -falign-jumps -falign-functions -falign-labels      \
		-falign-loops -fstrength-reduce -fomit-frame-pointer                  \
		-finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-Cpp \
		-Wno-unused-label -Wno-unused-parameter -nostdlib -nostartfiles -Wall \
		-Wextra -nodefaultlibs -O0

IFLAG = -I./inc

LFLAG = -Wl,--verbose # No lo uso porque añade output que sólo es útil a veces

QEMU = qemu-system-i386 -kernel
QOPT = -gdb tcp::26000 -S

.PHONY: all clean re qemu fqemu


# NOTE: UFLAG se usa al llamar al comando make desde fuera:
# make fqemu UFLAG="-DDIAG" por ejemplo

all: $(KERNEL_FULL)
	$(CC) $(UFLAG) $(CFLAG) -T linker.ld kernelfull.o -o kernel.bin -lgcc
#$(LD) -T linker.ld $(FILES) -o kernel.bin


$(KERNEL_FULL):
	make -C boot
	make -C cpu
	make -C drivers
	make -C lib
	make -C mm
	make -C task
	make -C kernel
	@echo $(FILES)
	$(LD) -g -relocatable $(FILES) -o kernelfull.o


clean:
	@echo find $(BUILD_DIR) -type f -name \"*.o\" -exec rm {} \\;
	$(shell find $(BUILD_DIR) -type f -name "*.o" -exec rm {} \;)
	rm -f kernelfull.o kernel.bin

re: clean all

qemu: re
	$(QEMU) kernel.bin $(QOPT)

fqemu: re
	$(QEMU) kernel.bin -m 128M
