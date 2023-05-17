#ifndef _TSS_H_
#define _TSS_H_

/* Intel x86 SDM Vol3: Ch 8.
 * Para el tema de protección de memoria entre tareas: 8.1.3 */

#include <stdint.h>

typedef struct
{
    uint32_t prev; /* previous task link */

    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;

    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;

    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;

    uint32_t ldtsel;
    uint16_t trap;
    uint16_t iomapbase;
} tss_entry_t; /* Intel x86 SDM Vol 3 Fig 8-2 */

void tss_init(void);

#endif
