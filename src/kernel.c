#include "vga.h" /* vga_puts, printk    */
#include "gdt.h" /* gdt_init            */
#include "idt.h" /* idt_init            */
#include "isr.h" /* isr_init            */
#include "irq.h" /* irq_init            */
#include "pit.h" /* pit_install_handler */
#include "kb.h"  /* kb_install_handler  */
#include "sys.h" /* sti */

void kmain()
{
    vga_init();
    vga_puts("Bienvenido a ChamacOS!\n");

    gdt_init();
    vga_puts("GDT inicializado\n");

    idt_init();
    vga_puts("IDT inicializado\n");

    isr_init();
    vga_puts("ISRs inicializados\n");

    irq_init();
    vga_puts("IRQs inicializados\n");

    sti();

    int v1  = 31312;
    char c1 = 'c';

    printk("v1 vale %d\n", v1);
    printk("v1 en hex: 0x%x\n", v1);

    printk("c1 vale %c\n", c1);
    printk("c1 en hex: 0x%x\n", c1);
    printk("c1 en dec: %d\n", c1);

    pit_install_handler();
    printk("Timer instalado\n");

    kb_install_handler();
    printk("Teclado instalado\n");

    for (;;);

}
