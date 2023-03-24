#include "vga.h"   /* vga_puts              */
#include "gdt.h"   /* gdt_init              */
#include "idt.h"   /* idt_init              */
#include "isr.h"   /* isr_init              */
#include "irq.h"   /* irq_init              */
#include "timer.h" /* timer_install_handler */

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

    idt_en_ints();

    timer_install_handler();
    vga_puts("Timer instalado\n");

    for (;;);

}
