[BITS 32]

; GRUB nos coloca ya en modo protegido (Vol3: 10.9.1)

global start        ; Necesario en linker.ld
global kstack_end   ; Para kernel.c
global kstack_start ; Para kernel.c
start:
    mov esp, kstack_start ; Pon un stack
    jmp stublet

SECTION .multiboot
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

    ;dd mboot
    ;dd code
    ;dd bss
    ;dd end
    ;dd start

SECTION .text
stublet:
    extern kmain
    push eax        ; multiboot: magic
    push ebx        ; multiboot: info structure
    call kmain

    jmp $

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; GDT - Vol3: 3.4.5
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global gdt_load
extern gdtr         ; gdt.c - Vol3: 2.4.1
gdt_load:
    lgdt[gdtr]      ; Carga el GDT con la variable gdtr de gdt.c
    mov ax, 0x10    ; 0x10 es el despl. de nuestro segmento de datos en el GDT

    mov ds, ax      ; Inicializa los registros de segmento
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    jmp 0x08:flush2 ; 0x08 es el despl. del segmento de codigo - far jump
flush2:
    ret             ; Retorna al codigo de C

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; TSS  Vol3: 8.2.1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global tss_load ; Usado en tss.c
tss_load:
    mov ax, 0x18 ; Cuarto indice
    ltr ax
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ISRs
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global idt_load
global isr_vector

extern idtr              ; idt.c
extern isr_fault_handler ; isr.c

idt_load: ; Vol3: 6.10
    lidt[idtr]
    ret

%macro isr_err_stub 1
isr_stub_%+%1:
    cli
    push byte %1  ; Apilo no. de interrupt. El no. de error ya lo apila la CPU
    jmp isr_common_stub
%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:
    cli
    push byte 0  ; Apilo no. de error de mentira (Vol3: 6.12)
    push byte %1 ; Apilo no. de interrupt.
    jmp isr_common_stub
%endmacro

 ; Volume 3: 6.3.1 (table 6-1)
isr_no_err_stub 0  ; Divide error
isr_no_err_stub 1  ; Debug exception
isr_no_err_stub 2  ; NMI interrupt
isr_no_err_stub 3  ; Breakpoint
isr_no_err_stub 4  ; Overflow
isr_no_err_stub 5  ; BOUND range exceeded
isr_no_err_stub 6  ; Invalid opcode (undefined opcode)
isr_no_err_stub 7  ; Device not available (no math coprocessor)
isr_err_stub    8  ; Double Fault
isr_no_err_stub 9  ; Coprocessor segment overrun (reserved)
isr_err_stub    10 ; Invalid TSS
isr_err_stub    11 ; Segment not present
isr_err_stub    12 ; Stack-segment fault
isr_err_stub    13 ; General Protection
isr_err_stub    14 ; Page Fault
isr_no_err_stub 15   ; INTEL RESERVED DO NOT USE
isr_no_err_stub 16 ; x87 FPU Floating-point Error (Math fault)
isr_err_stub    17 ; Alignment check
isr_no_err_stub 18 ; Machine check
isr_no_err_stub 19 ; SIMD Floating-point Exception
isr_no_err_stub 20 ; Virtualization exception
isr_no_err_stub 21 ; Control Protection exception
isr_no_err_stub 22   ; INTEL RESERVED DO NOT USE
isr_no_err_stub 23   ; INTEL RESERVED DO NOT USE
isr_no_err_stub 24   ; INTEL RESERVED DO NOT USE
isr_no_err_stub 25   ; INTEL RESERVED DO NOT USE
isr_no_err_stub 26   ; INTEL RESERVED DO NOT USE
isr_no_err_stub 27   ; INTEL RESERVED DO NOT USE
isr_no_err_stub 28   ; INTEL RESERVED DO NOT USE
isr_no_err_stub 29   ; INTEL RESERVED DO NOT USE
isr_err_stub    30   ; INTEL RESERVED DO NOT USE
isr_no_err_stub 31   ; INTEL RESERVED DO NOT USE
                     ; 32-255 User defined


isr_common_stub:
    pushad
    push ds ; Guarda registros de segmento
    push es
    push fs
    push gs

    mov ax, 0x10 ; Segmento de datos del kernel
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp
    push eax
    mov eax, isr_fault_handler
    call eax
    pop eax

    pop gs
    pop fs
    pop es
    pop ds
    popad

    add esp, 8
    iret

isr_vector:
%assign i 0
%rep 32
    dd isr_stub_%+i
%assign i i+1
%endrep

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; IRQs (PIC)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
extern irq_fault_handler
global irq_vector

%macro irq_stub 1
irq_stub_%+%1:
    cli
    push byte 0
    push byte %1
    jmp irq_common_stub
%endmacro

irq_stub 32
irq_stub 33
irq_stub 34
irq_stub 35
irq_stub 36
irq_stub 37
irq_stub 38
irq_stub 39
irq_stub 40
irq_stub 41
irq_stub 42
irq_stub 43
irq_stub 44
irq_stub 45
irq_stub 46
irq_stub 47


irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    mov eax, esp
    push eax
    ;mov eax, irq_fault_handler
    call irq_fault_handler
    pop eax
    
    pop gs
    pop fs
    pop es
    pop ds
    popa
    
    add esp, 8
    iret

irq_vector:
%assign i 32
%rep 16
    dd irq_stub_%+i
%assign i i+1
%endrep



SECTION .bss
ALIGN 16      ; Alineado a 16 bytes de acuerdo al ABI de System V
kstack_end:
    resb 8192 ; Reserva 8192 bytes de memoria como stack para el kernel
kstack_start:
