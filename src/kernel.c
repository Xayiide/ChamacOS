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

void kmain(multiboot_info_t *mbd, uint32_t magic)
{
    void *dir1, *dir2, *dir3;

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

    pmm_init(mbd, magic);

    sti();

    pit_install_handler();
    printk("Timer instalado\n");

    kb_install_handler();
    printk("Teclado instalado\n");

    dir1 = pmm_alloc_frame();
    dir2 = pmm_alloc_frame();
    dir3 = pmm_alloc_frames(2);

    pmm_free_frame(dir1);
    pmm_free_frame(dir2);
    pmm_free_frame(dir3);

    for (;;);

}
