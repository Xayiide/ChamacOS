#include <stdint.h> /* uint_t */

#include "irq.h"
#include "io.h"  /* inb outb  */
#include "pic.h" /* pic_remap */
#include "sys.h" /* regs_t    */
#include "idt.h" /* idt_set   */
#include "gdt.h" /* macros    */

extern void *irq_vector[];

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
    int i;
    pic_remap();

    for (i = IDT_NUM_EXC; i < IDT_NUM_EXC + IRQ_NUM_INT; i++)
    {
        idt_set(i, (uint32_t) irq_vector[i - IDT_NUM_EXC],
                GDT_OFFSET_KERNEL_CODE, IDT_R0_32_INT);
    }
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
