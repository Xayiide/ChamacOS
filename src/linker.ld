ENTRY(_start)

SECTIONS
{
    . = 1M;

    _kernel_start = .;
    .text BLOCK(4K) : ALIGN(4K)
    {
        _text_start = .;

        code = .; /* multiboot_header */
        
        _mb_start = .;
        *(.multiboot)
        _mb_end   = .;
        *(.text)
        _text_end   = .;
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        _rodata_start = .;
        *(.rodata)
        _rodata_end   = .;
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        _data_start = .;
        *(.data)
        _data_end   = .;
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        _bss_start = .;
        bss = .; /* multiboot_header */
        *(COMMON)
        *(.bss)
        _bss_end   = .;
    }
    end = .; /* multiboot_header */
    _kernel_end   = .;

    /* Otras secciones producidas por el compilador */
}

