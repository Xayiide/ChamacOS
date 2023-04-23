#ifndef _GDT_H_
#define _GDT_H

#include <stdint.h> /* uint_t */

/* Importante: si se cambian estas macros se tienen que cambiar también en
 * el fichero boot.asm */
#define GDT_OFFSET_KERNEL_CODE 0x08 /* 8 : Primer descriptor  */
#define GDT_OFFSET_KERNEL_DATA 0x10 /* 16: Segundo descriptor */

typedef struct 
{
    uint16_t limit_lo; /* Seg Limit 15:0 */
    uint16_t base_lo;  /* Base 15:0      */
    uint8_t  base_mi;  /* Base 23:16     */
    uint8_t  access;   /* Present, Desc Priv Lvl (2), Desc Type, Seg Type (4) */
    uint8_t  gran;     /* Granu, Default opsize, L, Availbl , Seg Limit 19:16 */
    uint8_t  base_hi;  /* Base 31:24     */
} __attribute__((packed)) gdt_entry_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtr_t;

void gdt_init(void);
void gdt_diag(void);

#endif
