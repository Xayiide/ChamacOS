#include <stdint.h>    /* uint_t              */
#include "vga.h"       /* vga_puts, printk    */
#include "gdt.h"       /* gdt_init            */
#include "idt.h"       /* idt_init            */
#include "isr.h"       /* isr_init            */
#include "irq.h"       /* irq_init            */
#include "pit.h"       /* pit_install_handler */
#include "kb.h"        /* kb_install_handler  */
#include "sys.h"       /* sti                 */
#include "pmm.h"       /* pmm_init            */
#include "task.h"      /* task_init           */
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
    task_init();

#ifdef DIAG
    vga_diag();
    gdt_diag();
    idt_diag();
    isr_diag();
    pmm_diag();
    pit_diag();
#endif


    for (;;);

}

void k_diag(void)
{
    sys_diag();
    printk("\tkmain:  [0x%x]\n", kmain);
    printk(" === === \n");
}
