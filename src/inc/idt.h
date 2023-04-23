#ifndef _IDT_H_
#define _IDT_H_

#include <stdint.h> /* uint_t */

#define IDT_NUM_EXC         32
#define IDT_MAX_DESCRIPTORS 256

#define IDT_DESC_TASK    0x05
#define IDT_DESC_16_INT  0x06
#define IDT_DESC_16_TRAP 0x07
#define IDT_DESC_32_INT  0x0E
#define IDT_DESC_32_TRAP 0x0F

#define IDT_DESC_PRES 0x80 // 1000 -> PRESENT
#define IDT_DESC_R0   0x00 // 0000 -> Por defecto al usar GATE_PRESENT
#define IDT_DESC_R1   0x40 // 0100
#define IDT_DESC_R2   0x20 // 0010
#define IDT_DESC_R3   0x60 // 0110

#define IDT_R0_32_INT  (IDT_DESC_PRES | IDT_DESC_32_INT)  // 8Eh
#define IDT_R0_32_TRAP (IDT_DESC_PRES | IDT_DESC_32_TRAP) // 8Fh
#define IDT_R3_32_INT  (IDT_DESC_PRES | IDT_DESC_R3 | IDT_DESC_32_INT)  // EEh
#define IDT_R3_32_TRAP (IDT_DESC_PRES | IDT_DESC_R3 | IDT_DESC_32_TRAP) // EFh

typedef struct
{
    uint16_t base_lo;
    uint16_t ssel;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t base_hi;
} __attribute__((packed)) idt_entry_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

void idt_init(void);
void idt_set(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_diag(void);

#endif
