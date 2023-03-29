#include <stdint.h> /* uint_t              */

#include "pit.h"
#include "io.h"     /* inb outb */
#include "sys.h"    /* cli, sti            */
#include "vga.h"    /* print               */
#include "irq.h"    /* irq_install_handler */

static uint32_t timer_ticks = 0;

void pit_set_phase(uint32_t count)
{
    /* cli() ? */
    uint32_t divisor;
    uint8_t  cmd;

    cmd = (PIT_SEL_CH0 | PIT_AM_LOHI | PIT_OPM_3 | PIT_BIN);

    divisor = PIT_OSC_FQ / count;
    outb(IO_PIT_CMD, cmd);           /* 0x36 */
    outb(IO_PIT_CH0_DAT, divisor & 0xFF); /* Low byte of divisor  */
    outb(IO_PIT_CH0_DAT, divisor >> 8);   /* High byte of divisor */
}


void pit_handler(regs_t *r)
{
    timer_ticks++;

    if (timer_ticks % 18 == 0)
    {
        sys_uptime_add_sec();
    }
}

void pit_install_handler()
{
    irq_install_handler(0, pit_handler); /* TODO: Cambiar el 0. En io.h cuando esté */
}
