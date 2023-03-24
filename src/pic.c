#include <stdint.h> /* uint_t  */

#include "system.h" /* in/outb */
#include "pic.h"

void pic_remap(void)
{
    uint8_t master_mask, slave_mask;

    /* Guarda las máscaras de cada PIC. Si no se manda CMD, DAT devuelve la
     * máscara actual: https://wiki.osdev.org/PIC#Masking */
    master_mask = inb(PIC1_DAT);
    slave_mask  = inb(PIC2_DAT);

    /* Secuencia de inicialización */
    outb(PIC1_CMD, PIC_ICW1_INIT);
    outb(PIC2_CMD, PIC_ICW1_INIT);

    /* Manda ICW2: El despl del vector */
    outb(PIC1_DAT, PIC1_OFFSET);
    outb(PIC2_DAT, PIC1_OFFSET + 0x08);

    /* Manda ICW3: El cableado de cada PIC */
    outb(PIC1_DAT, 0x04); /* PIC2 en IRQ2 (0000 0100)*/
    outb(PIC2_DAT, 0x02); /* PIC1 en IRQ9 creo, no entiendo por que el 2 */

    /* Manda ICW4: Modo 8086*/
    outb(PIC1_DAT, PIC_ICW4_8086);
    outb(PIC2_DAT, PIC_ICW4_8086);

    /* Restaura las máscaras de cada PIC */
    outb(PIC1_DAT, master_mask);
    outb(PIC2_DAT, slave_mask);
}
