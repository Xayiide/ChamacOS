#include <stdint.h>    /* uint_t              */
#include "vga.h"       /* vga_puts, printk    */
#include "gdt.h"       /* gdt_init            */
#include "idt.h"       /* idt_init            */
#include "isr.h"       /* isr_init            */
#include "irq.h"       /* irq_init            */
#include "pit.h"       /* pit_install_handler */
#include "kb.h"        /* kb_install_handler  */
#include "sys.h"       /* sti */
#include "multiboot.h"

void kmain(multiboot_info_t *mbd, uint32_t magic)
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

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        panic("Numero magico invalido");
    }

    if (!(mbd->flags >> 6 & 0x1))
    {
        panic("Mapa de memoria dado por GRUB invalido");
    }

    for (int i = 0; i < mbd->mmap_length; i+= sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t *mmmt = (multiboot_memory_map_t *) (mbd->mmap_addr + i);
        printk("Start addr: 0x%x%x | ", mmmt->addr_hi, mmmt->addr_lo);
        printk("length: 0x%x%x | ", mmmt->len_hi, mmmt->addr_lo);
        printk("size: 0x%x | type: %d\n", mmmt->size, mmmt->type);
    
        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            /* Hacer algo con esta memoria. Tener en cuenta que estamos ocupando
            memoria de esta zona.*/
        }
    }




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
