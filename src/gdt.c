#include <stdint.h>

#include "gdt.h"
#include "system.h"


static gdt_entry_t gdt[3];
gdtr_t             gdtr; /* se usa en boot.asm */

extern void gdt_load(); /* boot.asm */


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

void gdt_init()
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
    gdt_set(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* Segmento de datos. Igual pero tipo = datos */
    gdt_set(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_load();
}
		