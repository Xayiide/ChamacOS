#include <stdint.h> /* uint_t              */

#include "pit.h"
#include "io.h"     /* inb outb */
#include "sys.h"    /* cli, sti            */
#include "vga.h"    /* print               */
#include "irq.h"    /* irq_install_handler */

static void pit_handler(regs_t *r);

static uint32_t timer_ticks = 0;


static void pit_handler(regs_t *r)
{
    timer_ticks++;

    if (timer_ticks % 18 == 0)
    {
        sys_uptime_add_sec();
    }
}



uint32_t pit_get_ticks(void)
{
    return timer_ticks;
}

void pit_set_phase(uint16_t count)
{
    /* cli() ? */
    cli();

    uint32_t divisor = 0;
    uint8_t  cmd = 0;

    if (count == 0)
        return;

    if (divisor | cmd) {}; /* compilador */

    cmd = (PIT_SEL_CH0 | PIT_AM_LOHI | PIT_OPM_3 | PIT_BIN); /* 0x36 */

    divisor = PIT_OSC_FQ / count;
    //outb(IO_PIT_CMD, cmd);
    outb(IO_PIT_CH0_DAT, count & 0xFF);            /* Low byte of divisor  */
    outb(IO_PIT_CH0_DAT, (count & 0xFF00) >> 8);   /* High byte of divisor */
}

void pit_install_handler(void)
{
    irq_install_handler(0, pit_handler); /* TODO: Cambiar el 0. En io.h cuando esté */
}

uint16_t pit_get_count(void)
{
    uint16_t ret = 0;

    cli();
    outb(IO_PIT_CMD, PIT_SEL_CH0);
    ret  = inb(IO_PIT_CH0_DAT);      /* Byte menos sign */
    ret |= inb(IO_PIT_CH0_DAT) << 8; /* Byte más sign   */

    return ret;
}
