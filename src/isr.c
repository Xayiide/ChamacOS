#include "isr.h"
#include "idt.h"    /* idt_set    */
#include "sys.h"    /* regs_t     */
#include "vga.h"    /* vga_puts   */
#include "gdt.h"    /* gdt macros */

extern void *isr_vector[];

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
    int i;
    for (i = 0; i < ISR_NUM_EXC; i++)
    {
        idt_set(i, (uint32_t) isr_vector[i],
                GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    }
}

void isr_fault_handler(regs_t *r)
{
    if (r->int_no < ISR_NUM_EXC)
    {
        vga_color(VGA_BACK_BLACK, VGA_FORE_RED);
        vga_puts(isr_exception_name[r->int_no]);
        vga_puts("\nParando el sistema!\n");
        vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
        while (1);
    }
}
