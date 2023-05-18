OUTPUT_FORMAT("binary")
ENTRY(start)
phys = 0x00100000; /* 1M */
SECTIONS
{
    . = phys;
    _kernel_start = .;
    _text_start = .;
    .text phys : AT(phys) {
        . = ALIGN(4096);
        KEEP(*(.multiboot))
        code = .;
        *(.text)
        *(.rodata)
    }
    _text_end = .;
    

    _data_start = .;
    .data BLOCK(4K) : ALIGN(4K) {
        data = .;
        *(.data)
        . = ALIGN(4096);
    }
    _data_end = .;

    _bss_start = .;
    .bss BLOCK(4K) : ALIGN(4K) {
        bss = .;
        *(.bss)
        . = ALIGN(4096);
    }
    _bss_end = .;
    _kernel_end = .;
    end = .;
}
