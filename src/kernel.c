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
#include "multiboot.h"

extern uint32_t start;
extern uint32_t kstack_start, kstack_end;

void kmain(multiboot_info_t *mbd, uint32_t magic)
{
    vga_init();
    printk("Bienvenido a ChamacOS!\n");
    printk("\t[kmain:        0x%x]\n", kmain);
    printk("\t[start:        0x%x]\n", &start);
    printk("\t[kstack_start: 0x%x]\n", &kstack_start);
    printk("\t[kstack_end:   0x%x]\n", &kstack_end);

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
