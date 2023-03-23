#include "system.h"
#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"


void kmain()
{
    vga_init();
    vga_puts("Bienvenido a ChamacOS!\n");

    gdt_init();
    vga_puts("GDT inicializado\n");
    for (;;);
}
