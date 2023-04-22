#ifndef _GDT_H_
#define _GDT_H

#include <stdint.h> /* uint_t */

/* Importante: si se cambian estas macros se tienen que cambiar también en
 * el fichero boot.asm */
#define GDT_OFFSET_KERNEL_CODE 0x08 /* 8 : Primer descriptor  */
#define GDT_OFFSET_KERNEL_DATA 0x10 /* 16: Segundo descriptor */

typedef struct 
{
    uint16_t limit_lo;
    uint16_t base_lo;
    uint8_t  base_mi;
    uint8_t  access;
    uint8_t  gran;  
    uint8_t  base_hi;
} __attribute__((packed)) gdt_entry_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtr_t;

void gdt_init();

#endif
