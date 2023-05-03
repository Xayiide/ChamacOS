#include <stdint.h>      /* uint_t */

#include "idt.h"
#include "drivers/vga.h" /* printk */
#include "lib/sys.h"     /* memset */

static idt_entry_t idt[IDT_MAX_DESCRIPTORS];
idtr_t             idtr;


extern void idt_load(void); /* boot.asm */


void idt_set(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].ssel    = sel;
    idt[num].zero    = 0;
    idt[num].flags   = flags;
}

void idt_init(void)
{
    /* Inicializa el IDT entero poniéndolo a 0 */
    memset(&idt, 0, sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS);

    idtr.limit = (sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS) - 1;
    idtr.base  = (uint32_t) idt;

    idt_load();
}

void idt_diag(void)
{
    vga_color(VGA_BACK_BLACK, VGA_FORE_RED);
    printk(" === IDT DIAGNOSIS ===\n");
    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    printk("  idt:  0x%x\n", &idt);
    printk("  idtr: 0x%x\n", &idtr);
}
