#include <stdint.h>

#include "drivers/vga.h" /* printk */
#include "gdt.h"


static gdt_entry_t gdt[GDT_NUM_DESC];
gdtr_t             gdtr; /* se usa en boot.asm */

extern void gdt_load(void); /* boot.asm */


static void gdt_set(uint32_t num, uint32_t base, uint32_t limit,
                    uint8_t access, uint8_t gran);


static void gdt_set(uint32_t num, uint32_t base, uint32_t limit,
                    uint8_t access, uint8_t gran)
{
    gdt[num].base_lo  = (base & 0xFFFF);
    gdt[num].base_mi  = (base >> 16) & 0xFF;
    gdt[num].base_hi  = (base >> 24) & 0xFF;

    gdt[num].limit_lo = (limit & 0xFFFF);
    gdt[num].gran     = ((limit >> 16) & 0x0F);

    gdt[num].gran    |= (gran & 0xF0);
    gdt[num].access   = access;
}

void gdt_init(void)
{
    gdtr.limit = (sizeof(gdt_entry_t) * 3) - 1;
    gdtr.base = (uint32_t) gdt;

    /* Descriptor nulo */
    gdt_set(0, 0, 0, 0, 0);

    /* Segmento de código de kernel */
    gdt_set(1, 0, 0xFFFFFFFF, GDT_R0_CODE_XR, GDT_GRAN_DEFAULT);

    /* Segmento de datos de kernel. Igual pero tipo = datos */
    gdt_set(2, 0, 0xFFFFFFFF, GDT_R0_DATA_RW, GDT_GRAN_DEFAULT);

    gdt_load();
}

void gdt_diag(void)
{
    vga_color(VGA_BACK_BLACK, VGA_FORE_RED);
    printk(" === GDT DIAGNOSIS ===\n");
    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    printk("  gdt:      0x%x\n", &gdt);
    printk("  gdtr:     0x%x\n", &gdtr);
    printk("  gdt_load: 0x%x\n", gdt_load);
}
