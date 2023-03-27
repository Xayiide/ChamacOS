#include <stdint.h> /* uint_t */

#include "kb.h"
#include "sys.h"    /* regs_t              */
#include "vga.h"    /* pritnk              */
#include "irq.h"    /* irq_install_handler */

void kb_handler(regs_t *r)
{
    uint8_t scancode;

    scancode = inb(0x64);

    if (scancode & 0x0E /*KB_KEY_RELEASED*/)
    {
        printk("Soltada la tecla %x\n", (int8_t) scancode);
        /* Se ha soltado el shift, ctrl, alt */
    }
    else
    {
        printk("Apretada la tecla %x\n", (int8_t) scancode);
        /* Si se mantiene la tecla pulsada seguiremos recibiendo ints. */
        /* 1. Traducir el scancode a ASCII
         * 2. Imprimir el ASCII que sea */
    }
}

void kb_install_handler()
{
    irq_install_handler(1, kb_handler);
}