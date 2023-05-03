BUILD_DIR = ../../build/boot

AS  = nasm
ASF = -f elf

BOOT_SRCS := $(wildcard *.asm)
BOOT_OBJS := $(patsubst %.asm, $(BUILD_DIR)/%.o, $(BOOT_SRCS))

all: $(BOOT_OBJS)

$(BUILD_DIR)/%.o: %.asm
	$(AS) $(ASF) $< -o $@

clean:
	rm -f $(BOOT_OBJS)

re: clean all
