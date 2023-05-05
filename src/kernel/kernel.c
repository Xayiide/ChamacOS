#include <stdint.h>      /* uint_t              */
#include "drivers/vga.h" /* vga_puts, printk    */
#include "cpu/gdt.h"     /* gdt_init            */
#include "cpu/idt.h"     /* idt_init            */
#include "cpu/isr.h"     /* isr_init            */
#include "cpu/irq.h"     /* irq_init            */
#include "cpu/pit.h"     /* pit_install_handler */
#include "drivers/kb.h"  /* kb_install_handler  */
#include "lib/sys.h"     /* sti                 */
#include "mm/pmm.h"      /* pmm_init            */
#include "task/sched.h"  /* sched_init          */
#include "multiboot.h"

static void k_diag(void);

void kmain(multiboot_info_t *mbd, uint32_t magic)
{
    sys_fillinfo();

    vga_init();
    printk("Bienvenido a ChamacOS!\n");

    gdt_init();
    vga_puts("GDT inicializado\n");

    idt_init();
    vga_puts("IDT inicializado\n");

    isr_init();
    vga_puts("ISRs inicializados\n");

    irq_init();
    vga_puts("IRQs inicializados\n");

    pmm_init(mbd, magic);

    sti();

    pit_install_handler();
    printk("Timer instalado\n");

    kb_install_handler();
    printk("Teclado instalado\n");

    k_diag();
    sched_init();

#ifdef DIAG
    vga_diag();
    gdt_diag();
    idt_diag();
    isr_diag();
    pmm_diag();
    pit_diag();
    sched_diag();
#endif


    for (;;);

}

void k_diag(void)
{
    sys_diag();
    printk("\tkmain:  [0x%x]\n", kmain);
    printk(" === === \n");
}
