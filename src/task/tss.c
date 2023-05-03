#include <stdint.h>

#include "drivers/vga.h" /* printk */
#include "tss.h"

extern void tss_load(uint32_t num); /* boot.asm */


void tss_init(uint32_t num)
{
    tss_load(num * 8);
}
