#include <stdint.h> /* uint_t */

#include "irq.h"
#include "io.h"  /* inb outb  */
#include "pic.h" /* pic_remap */
#include "sys.h" /* regs_t    */
#include "idt.h" /* idt_set   */
#include "gdt.h" /* macros    */


extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();


void *irq_routines[16] = {0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0};

void irq_install_handler(uint8_t irq, void (*handler)(regs_t *r))
{
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(uint8_t irq)
{
    irq_routines[irq] = 0;
}

void irq_init()
{
    pic_remap();

    idt_set(32, (uint32_t) irq0,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(33, (uint32_t) irq1,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(34, (uint32_t) irq2,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(35, (uint32_t) irq3,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(36, (uint32_t) irq4,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(37, (uint32_t) irq5,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(38, (uint32_t) irq6,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(39, (uint32_t) irq7,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(40, (uint32_t) irq8,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(41, (uint32_t) irq9,  GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(42, (uint32_t) irq10, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(43, (uint32_t) irq11, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(44, (uint32_t) irq12, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(45, (uint32_t) irq13, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(46, (uint32_t) irq14, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    idt_set(47, (uint32_t) irq15, GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
}

void irq_fault_handler(regs_t *r)
{
    void (*handler)(regs_t *r); /* handler: puntero vacío a función */

    handler = irq_routines[r->int_no - PIC1_OFFSET];
    if (handler != 0)
    {
        handler(r);
    }

    /* El índice de IDT invocado es superior a 40: enviamos un EOI al PIC
     * esclavo */
    if (r->int_no >= PIC2_OFFSET)
    {
        outb(IO_PIC2_CMD, PIC_EOI);
    }
    outb(IO_PIC1_CMD, PIC_EOI); /* Y siempre se lo tenemos que enviar al master */
}