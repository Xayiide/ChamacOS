set architecture i8086
add-symbol-file kernelfull.o 0x00100000
set disassembly-flavor intel
target remote localhost:26000
layout asm
layout regs
