#include <stdint.h> /* uint_t */

#include "kb.h"
#include "sys.h" /* regs_t              */
#include "io.h"  /* inb outb            */
#include "vga.h" /* pritnk              */
#include "irq.h" /* irq_install_handler */

void kb_handler(regs_t *r)
{
    uint8_t scancode;

    scancode = inb(IO_KB_DAT);

    if (scancode & KB_KEY_RELEASED)
    {
        printk("Soltada la tecla %x\n", scancode);
        /* Se ha soltado el shift, ctrl, alt */
    }
    else
    {
        printk("Apretada la tecla %x\n", scancode);
        /* Si se mantiene la tecla pulsada seguiremos recibiendo ints. */
        /* 1. Traducir el scancode a ASCII
         * 2. Imprimir el ASCII que sea */
    }
}

void kb_install_handler()
{
    uint8_t kb_stat;

    irq_install_handler(1, kb_handler);

    kb_stat = inb(IO_KB_STA);
    printk("El registro de estado del teclado: 0x%x\n", kb_stat);
}