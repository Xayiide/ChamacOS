#include <stddef.h> /* size_t */
#include <stdint.h> /* uint_t */

#include "sys.h"


void *memcpy(void *dest, const void *src, size_t n)
{
    const char *sp;
    char *dp;

    sp = (const char *) src;
    dp = (char *) dest;

    for (; n != 0; n--)
        *dp++ = *sp++;

    return dest;
}

void *memset(void *dest, uint8_t byte, size_t n)
{
    uint8_t *temp;

    temp = (uint8_t *) dest;

    for (; n != 0; n--)
        *temp++ = byte;

    return dest;
}

void *memsetw(void *dest, uint16_t val, size_t n)
{
    uint16_t *temp;

    temp = (uint16_t *) dest;

    for (; n != 0; n--)
        *temp++ = val;

    return dest;
}

size_t strlen(const char *s)
{
    size_t ret;

    for (ret = 0; *s != '\0'; s++)
        ret++;

    return ret;
}

uint8_t inb(uint16_t port)
{
    uint8_t ret = 0;
    __asm__ volatile ("inb %1, %0" : : "a" (ret), "Nd" (port));

    return ret;
}

void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ("outb %0, %1" : : "a" (val), "Nd" (port));
}

void cli()
{
    __asm__ volatile("cli");
}

void sti()
{
    __asm__ volatile("sti");
}
