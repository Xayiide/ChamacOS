#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <stddef.h> /* size_t */
#include <stdint.h> /* uint_t */

typedef struct
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, old_esp, ss;
} __attribute__((packed)) regs_t;

/* main.c (futuro mem) */
void   *memcpy (void *dest, const void *src, size_t n);
void   *memset (void *dest, uint8_t byte, size_t n);
void   *memsetw(void *dest, uint16_t val, size_t n);
size_t  strlen (const char *s);

/* io (futuro) */
uint8_t inb(uint16_t port);
void    outb(uint16_t port, uint8_t val);

void    cli();
void    sti();

#endif
