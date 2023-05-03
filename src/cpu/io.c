#include <stdint.h>

#include "io.h"


uint8_t inb(uint16_t port)
{
    uint8_t ret = 0;
    asm volatile("inb %1, %0" : "=a" (ret) : "Nd" (port));

    return ret;
}

void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ("outb %0, %1" : : "a" (val), "Nd" (port));
}
