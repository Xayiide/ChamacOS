#include <stdint.h>

#include "vga.h" /* printk */
#include "gdt.h"


static gdt_entry_t gdt[3];
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

    /* Segmento de código.
     * Dir base: 0
     * Límite  : 4 GBytes
     * Granul  : 4 KBytes
     * opcodes : 32-bit
     * tipo    : código */
    gdt_set(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); /* TODO Cambiar 9A y CF a macros */

    /* Segmento de datos. Igual pero tipo = datos */
    gdt_set(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    /* Segmento de código de usuario */
    gdt_set(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    /* Segmento de datos de usuario */
    gdt_set(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    /* Segmento de TSS */
    /* gdt_set(5, &tss_entry, &tss_entry + sizeof(tss_entry), 0x89, 0x0); */

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
