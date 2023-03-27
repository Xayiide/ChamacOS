#include <stddef.h> /* size_t */
#include <stdint.h> /* uint_t */

#include "sys.h"

static uint32_t uptime_seconds = 0;


/* TODO: mover a string.h */
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

/* TODO: mover a string.h */
void *memset(void *dest, uint8_t byte, size_t n)
{
    uint8_t *temp;

    temp = (uint8_t *) dest;

    for (; n != 0; n--)
        *temp++ = byte;

    return dest;
}

/* TODO: mover a string.h */
void *memsetw(void *dest, uint16_t val, size_t n)
{
    uint16_t *temp;

    temp = (uint16_t *) dest;

    for (; n != 0; n--)
        *temp++ = val;

    return dest;
}

/* TODO: mover a string.h */
size_t strlen(const char *s)
{
    size_t ret;

    for (ret = 0; *s != '\0'; s++)
        ret++;

    return ret;
}

void sys_uptime_add_sec()
{
    uptime_seconds++;
}

uint32_t sys_uptime()
{
    return uptime_seconds;
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

char *changebase(int32_t num, base_t base)
{
    static char repr[] = "0123456789ABCDEF";
    static char buff[50];

    char *ptr;

    ptr  = &buff[49];
    *ptr = '\0';

    do
    {
        *--ptr = repr[num % base];
        num /= base;
    } while (num != 0);

    return ptr;
}