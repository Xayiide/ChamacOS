[BITS 32]

global start
start:
    mov esp, _sys_stack
    jmp stublet

ALIGN 4
mboot:
    ; Multiboot macros to make a few lines later more readable
    MULTIBOOT_PAGE_ALIGN	equ 1<<0
    MULTIBOOT_MEMORY_INFO	equ 1<<1
    MULTIBOOT_AOUT_KLUDGE	equ 1<<16
    MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
    EXTERN code, bss, end

    ; This is the GRUB Multiboot header. A boot signature
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM

    dd mboot
    dd code
    dd bss
    dd end
    dd start

stublet:
    extern kmain
    call kmain
    jmp $

; GDT below
global gdt_load
extern gdtr         ; gdt.c
gdt_load:
    lgdt[gdtr]      ; Carga el GDT con la variable gdtr de gdt.c
    mov ax, 0x10    ; 0x10 es el despl. de nuestro segmento de datos en el GDT

    mov ds, ax      ; Inicializa los registros de segmento
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    jmp 0x08: flush2 ; 0x08 es el despl. del segmento de codigo
flush2:
    ret             ; Retorna al codigo de C


; ISRs below
global idt_load
extern idtr         ; idt.c
idt_load:
    lidt[idtr]
    ret




SECTION .bss
    resb 8192 ; Reserva 8192 bytes de memoria
_sys_stack:
