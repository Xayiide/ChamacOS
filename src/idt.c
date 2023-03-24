#include <stdint.h> /* uint_t */

#include "idt.h"
#include "system.h" /* memset */

static idt_entry_t idt[IDT_MAX_DESCRIPTORS];
idtr_t             idtr;


extern void idt_load(); /* boot.asm */


void idt_set(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].ssel    = sel;
    idt[num].zero    = 0;
    idt[num].flags   = flags;
}

void idt_init()
{
    /* Inicializa el IDT entero poniéndolo a 0 */
    memset(&idt, 0, sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS);

    idtr.limit = (sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS) - 1;
    idtr.base  = (uint32_t) idt;

    idt_load();
}

void idt_en_ints()
{
    __asm__ volatile("sti");
}

void idt_dis_ints()
{
    __asm__ volatile("cli");
}
