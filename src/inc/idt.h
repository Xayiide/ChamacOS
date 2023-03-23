#ifndef _IDT_H_
#define _IDT_H_

#include <stdint.h> /* uint_t */

#define IDT_MAX_DESCRIPTORS 256

typedef struct
{
    uint16_t base_lo;
    uint16_t ssel;
    uint8_t  zero;
    uint8_t  flags;
    uint8_t base_hi;
} __attribute__((packed)) idt_entry_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

void idt_init();
void idt_set(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#endif
