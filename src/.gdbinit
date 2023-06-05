set architecture i8086
add-symbol-file ./chamacos.elf 0x00100000
set disassembly-flavor intel
target remote localhost:26000
layout asm
layout regs
