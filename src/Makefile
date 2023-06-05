export PREFIX=$(HOME)/opt/cross
export TARGET=i686-elf
export PATH := $(PREFIX)/bin:$(PATH)

BUILD_DIR = ../build/
KERNEL_FULL = chamacos.elf

SRC_FILES = $(shell find -type f -name "*.c") \
			$(shell find -type f -name "*.asm")

DIR_FILES = $(patsubst ./%, $(BUILD_DIR)%, $(SRC_FILES))
FILES_1   = $(patsubst %.c, %.o, $(DIR_FILES))
FILES     = $(patsubst %.asm, %.o, $(FILES_1))

CC = i686-elf-gcc
LD = i686-elf-ld

CFLAG = -std=gnu99 -ffreestanding -Wall -Wextra -Werror
LFLAG = -Wl, --verbose

QEMU = qemu-system-i386
QOPT = -gdb tcp::26000 -S



.PHONY: all clean re fqemu cdqemu qemu

# NOTE: UFLAG se usa para llamar al comando make desde fuera:
# make fqemu UFLAG="-DDIAG"

all: $(KERNEL_FULL)
	objcopy -Obinary $(KERNEL_FULL) chamacos.bin
#$(CC) $(UFLAG) -T linker.ld -o chamacos.bin -ffreestanding -nostdlib $(FILES) -lgcc -g
	@./scripts/is_multiboot.sh

$(KERNEL_FULL):
	make -C boot
	make -C cpu
	make -C lib
	make -C drivers
	make -C mm
	make -C task
	make -C kernel
	$(LD) -Tlinker.ld $(FILES) -o $(KERNEL_FULL)
	#$(LD) -g -relocatable $(FILES) -o ../build/kernelfull.o

clean:
	@echo find $(BUILD_DIR) -type f -name \"*.o\" -exec rm {} \\;
	$(shell find $(BUILD_DIR) -type f -name "*.o" -exec rm {} \;)
	rm -f chamacos.bin chamacos.iso chamacos.elf

re: clean all

fqemu: re
	$(QEMU) -kernel chamacos.elf -m 128M

cdqemu: re
	mv chamacos.elf ../isodir/boot/chamacos.elf
	grub-mkrescue -o chamacos.iso ../isodir
	$(QEMU) -cdrom chamacos.iso
	
qemu: re
	$(QEMU) -kernel chamacos.elf $(QOPT)
