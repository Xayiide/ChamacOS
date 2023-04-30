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

extern uint32_t start;                    /* boot.asm */
extern uint32_t kstack_start, kstack_end; /* "        */

extern char     _kernel_start, _kernel_end; /* linker.ld */
extern char     _text_start, _text_end;     /* "         */
extern char     _bss_start, _bss_end;       /* "         */

static void k_diag(void);

void kmain(multiboot_info_t *mbd, uint32_t magic)
{
    vga_init();
    printk("Bienvenido a ChamacOS!\n");
    k_diag();

    gdt_init();
    vga_puts("GDT inicializado\n");

    //tss_init(3);
    //printk("TSS inicializado (pos %d)\n", 3);

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

void k_diag(void)
{
    printk("\tstart:  [0x%x]\n", &start);
    printk("\tKernel: [0x%x -> 0x%x]\n", &_kernel_start, &_kernel_end);
    printk("\tText:   [0x%x -> 0x%x]\n", &_text_start, &_text_end);
    printk("\tBSS:    [0x%x -> 0x%x]\n", &_bss_start, &_bss_end);
    printk("\tKstack: [0x%x <- 0x%x]\n", &kstack_end, &kstack_start);
    printk("\tkmain:  [0x%x]\n", kmain);

    printk(" === === \n");
}
