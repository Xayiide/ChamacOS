#include "system.h"
#include "vga.h"


void kmain()
{
    vga_init();
    vga_puts("Bienvenido a ChamacOS!\n");
    for(;;);
}
