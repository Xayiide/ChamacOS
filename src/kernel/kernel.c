#include <stdint.h>
#include "lib/sys.h"
#include "drivers/vga.h"
#include "mm/pmm.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/isr.h"
#include "cpu/irq.h"
#include "cpu/pit.h"
#include "drivers/kb.h"
#include "task/task.h"
#include "multiboot.h"

extern struct multiboot_header mbheader __attribute__((section(".multiboot")));

static void k_diag(void);

void doIt(void)
{
    printk("Switching to otherTask...\n");
    yield();
    printk("Returned to mainTask\n");
}

void kmain(multiboot_info_t *mbd, uint32_t magic)
{
    /* Initialize terminal interface */
    sys_fillinfo();
    vga_init();
    printk("Bienvenido a ChamacOS!\n");

    gdt_init();
    idt_init();
    isr_init();
    irq_init();

    pmm_init(mbd, magic);

    sti();

    pit_install_handler();
    kb_install_handler();

    k_diag();

    initTasking();
    startChamacOS();

#ifdef DIAG
    //vga_diag();
    //gdt_diag();
    //idt_diag();
    //isr_diag();
    //pmm_diag();
    //pit_diag();
#endif

    for (;;);
}


void k_diag(void)
{
    sys_diag();
    printk("\tkmain:  [0x%x]\n", kmain);
    printk(" === ===\n");
    printk("\tmbh magic:         0x%x\n", mbheader.magic);
    printk("\tmbh header_addr:   0x%x\n", mbheader.header_addr);
    printk("\tmbh load_addr:     0x%x\n", mbheader.load_addr);
    printk("\tmbh load end addr: 0x%x\n", mbheader.load_end_addr);
    printk("\tmbh bss_end addr:  0x%x\n", mbheader.bss_end_addr);
}
