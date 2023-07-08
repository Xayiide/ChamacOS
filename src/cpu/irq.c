#include <stdint.h>  /* uint_t */

#include "irq.h"
#include "io.h"      /* inb outb  */
#include "pic.h"     /* pic_remap */
#include "idt.h"     /* idt_set   */
#include "gdt.h"     /* macros    */

#include "lib/sys.h" /* regs_t    */

#include "drivers/vga.h" /* printk */

extern void *irq_vector[];

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


void irq_diag(void)
{
    vga_color(VGA_BACK_BLACK, VGA_FORE_RED);
    printk(" === IRQ DIAGNOSIS ===\n");
    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    printk("  irq_routines:   0x%x\n", &irq_routines);
    printk("  irq_vector:     0x%x\n", &irq_vector);
    printk("  irq_vector[0]:  0x%x\n", &irq_vector[0]);
    printk("  irq_vector[31]: 0x%x\n", &irq_vector[16]);
}
