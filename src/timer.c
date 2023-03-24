#include <stdint.h> /* uint_t              */

#include "timer.h"
#include "system.h" /* in/outb             */
#include "vga.h"    /* print               */
#include "irq.h"    /* irq_install_handler */

static uint32_t timer_ticks = 0;

#if 0
void timer_phase(uint32_t hz)
{
    uint32_t divisor;

    divisor = 1193180 / hz;
    outb(0x43, 0x36);           /* command byte 0x36    */
    outb(0x40, divisor & 0xFF); /* Low byte of divisor  */
    outb(0x40, divisor >> 8);   /* High byte of divisor */
}
#endif


void timer_handler(regs_t *r)
{
    timer_ticks++;

    if (timer_ticks % 18 == 0)
    {
        vga_color(VGA_BACK_BLACK, VGA_FORE_GREEN);
        vga_puts("Ha pasado un segundo\n");
        vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    }
}

void timer_install_handler()
{
    irq_install_handler(0, timer_handler);   
}
