#include <stdint.h> /* uint_t              */

#include "pit.h"
#include "sys.h" /* in/outb, cli, sti   */
#include "vga.h" /* print               */
#include "irq.h" /* irq_install_handler */

static uint32_t timer_ticks = 0;

void pit_set_phase(uint32_t count)
{
    /* cli() ? */
    uint32_t divisor;
    uint8_t  cmd;

    cmd = (PIT_SEL_CH0 | PIT_AM_LOHI | PIT_OPM_3 | PIT_BIN);

    divisor = PIT_OSC_FQ / count;
    outb(PIT_CMD, cmd);           /* 0x36 */
    outb(PIT_CH0_DATA, divisor & 0xFF); /* Low byte of divisor  */
    outb(PIT_CH0_DATA, divisor >> 8);   /* High byte of divisor */
}


void pit_handler(regs_t *r)
{
    static uint8_t paso = 0;
    timer_ticks++;

    if (timer_ticks % 10 == 0)
    {
        vga_color(VGA_BACK_BLACK, VGA_FORE_GREEN);
        vga_clear();
        if (paso % 2 == 0)
        {
            vga_puts("\\(o_o)\n");
            vga_puts("  ( (>\n");
            vga_puts(" /  \\\n");
        }
        else
        {
            vga_puts(" (o_o)/\n");
            vga_puts(" <) )\n");
            vga_puts(" /  \\\n");
        }

        paso++;
        vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    }
}

void pit_install_handler()
{
    irq_install_handler(0, pit_handler);   
}
