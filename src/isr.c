#include "isr.h"
#include "idt.h"    /* idt_set    */
#include "system.h" /* regs_t     */
#include "vga.h"    /* vga_puts   */
#include "gdt.h"    /* gdt macros */

extern void isr0(); /* boot.asm */
extern void isr1(); /* ...      */
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30(); /* ...      */
extern void isr31(); /* boot.asm */

char *isr_exception_name[] =
{
    "Divide error",
    "Debug",
    "Non Maskable Interrupt (NMI)",
    "Breakpoint",
    "Overflow",
    "BOUND range exceeded",
    "Invalid opcode",
    "Device not available (no math coprocessor)",
    "Double fault",
    "ISR 9: Reserved (Coprocessor segment overrun)",
    "Invalid TSS",
    "Segment not present",
    "Stack-segment fault",
    "General protection fault",
    "Page fault",
    "ISR 15: Reserved",
    "x87 FPU error (math)",
    "Alignment check",
    "Machine check",
    "SIMD FP error",
    "Virtualization error",
    "Control protection exception",
    "ISR 22: Reserved",
    "ISR 23: Reserved",
    "ISR 24: Reserved",
    "ISR 25: Reserved",
    "ISR 26: Reserved",
    "ISR 27: Reserved",
    "ISR 28: Reserved",
    "ISR 29: Reserved",
    "ISR 30: Reserved",
    "ISR 31: Reserved"
};

void isr_init()
{
    idt_set(0,  (uint32_t) isr0,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(1,  (uint32_t) isr1,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(2,  (uint32_t) isr2,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(3,  (uint32_t) isr3,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(4,  (uint32_t) isr4,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(5,  (uint32_t) isr5,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(6,  (uint32_t) isr6,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(7,  (uint32_t) isr7,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(8,  (uint32_t) isr8,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(9,  (uint32_t) isr9,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(10, (uint32_t) isr10, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(11, (uint32_t) isr11, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(12, (uint32_t) isr12, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(13, (uint32_t) isr13, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(14, (uint32_t) isr14, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(15, (uint32_t) isr15, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(16, (uint32_t) isr16, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(17, (uint32_t) isr17, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(18, (uint32_t) isr18, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(19, (uint32_t) isr19, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(20, (uint32_t) isr20, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(21, (uint32_t) isr21, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(22, (uint32_t) isr22, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(23, (uint32_t) isr23, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(24, (uint32_t) isr24, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(25, (uint32_t) isr25, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(26, (uint32_t) isr26, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(27, (uint32_t) isr27, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(28, (uint32_t) isr28, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(29, (uint32_t) isr29, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(30, (uint32_t) isr30, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(31, (uint32_t) isr31, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
}

void isr_fault_handler(regs_t *r)
{
    if (r->int_no < 32)
    {
        vga_color(VGA_BACK_BLACK, VGA_FORE_RED);
        vga_puts(isr_exception_name[r->int_no]);
        vga_puts("\nParando el sistema!\n");
        vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
        while (1);
    }
}
