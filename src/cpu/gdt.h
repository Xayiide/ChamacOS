#ifndef _GDT_H_
#define _GDT_H

/* Intel x86 SDM Vol3: 3.4.5 */

#include <stdint.h> /* uint_t */

/* Importante: si se cambian estas macros se tienen que cambiar también en
 * el fichero boot.asm */
#define GDT_NUM_DESC           1 + 2 /* NULL, Kernel */
#define GDT_OFFSET_KERNEL_CODE 0x08 /* 8 : Primer descriptor  */
#define GDT_OFFSET_KERNEL_DATA 0x10 /* 16: Segundo descriptor */
#define GDT_OFFSET_TSS         0x18 /* 24: Tercer descriptor  */

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



/* Vol3: 3.4.5.1 */
/* El campo gran de 8 bits tiene varios campos:
 * G, DB, L, AVL y 4 bits de límite de segmento */
/* G   = 1: El límite de segmento se interpreta en unidades de 4KB.
 * DB  = 1: 1 en segmentos de código/datos en modo 32 bits (Vol3: 3.4.5)
 * L   = 0: Es 0 si no estamos en modo IA-32e (Vol3: 3.4.5)
 * AVL = 0: Bit de reserva o available, da igual su valor */
#define GDT_GRAN_DEFAULT 0xCF

/* El campo access de 8 bits tiene varios campos:
 * P, 2 bits de DPL, S y 4 bits de tipo */
/* P    = 1:  Segmento presente
 * DPL  = 00: Anillo 0
 * S    = 1:  Segmento de código/datos (no de sistema).
 * Tipo = 1010: Segmento de código con permisos de ejecución/lectura */
#define GDT_R0      0x90
#define GDT_R3      0xF0
#define GDT_CODE_XR 0x0A
#define GDT_CODE_XO 0x08
#define GDT_DATA_RW 0x02

#define GDT_R0_CODE_XR (GDT_R0 | GDT_CODE_XR)
#define GDT_R0_CODE_XO (GDT_R0 | GDT_CODE_XO)
#define GDT_R0_DATA_RW (GDT_R0 | GDT_DATA_RW)
#define GDT_R3_CODE_XR (GDT_R3 | GDT_CODE_XR)
#define GDT_R3_CODE_XO (GDT_R3 | GDT_CODE_XO)
#define GDT_R3_DATA_RW (GDT_R3 | GDT_DATA_RW)

void gdt_init(void);
void gdt_diag(void);

#endif
