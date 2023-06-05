#include <stdint.h>      /* uint_t              */

#include "pit.h"
#include "io.h"          /* inb outb */
#include "irq.h"         /* irq_install_handler */
#include "lib/sys.h"     /* cli, sti            */
#include "drivers/vga.h" /* printk              */

static void pit_handler(regs_t *r);

static pit_info_t pit_info = {
        .num_ints = 0,
        .ints_sec = 0
};

static void pit_handler(regs_t *r)
{
    (void) r;
    pit_info.num_ints++; /* FIXME: cuando se desborda, el mód es incorrecto */

    /* TODO: para mayor exactitud, cuando se cambia ints_sec (pit_set_phase)
     * también habría que cambiar num_ints en proporción. Por ejemplo si
     * estábamos con 18 ticks por segundo y cuando ya hemos contado 17 el
     * ints_sec cambia a 36 ticks por segundo, num_ints debería pasar a
     * ser 34 para no perder proporcionalidad */
    if (pit_info.num_ints % pit_info.ints_sec == 0)
    {
        sys_uptime_add_sec();
    }
}



uint32_t pit_get_num_ints(void)
{
    return pit_info.num_ints;
}

uint32_t pit_get_freq(void)
{
    return pit_info.ints_sec;
}

void pit_set_phase(uint16_t count)
{
    cli();

    outb(IO_PIT_CH0_DAT, count & 0xFF);            /* Low byte of divisor  */
    outb(IO_PIT_CH0_DAT, (count & 0xFF00) >> 8);   /* High byte of divisor */

    sti();

    pit_info.ints_sec = (uint32_t) PIT_OSC_FQ / (count == 0 ? 65536 : count);
    printk("ints_sec: %d\n", pit_info.ints_sec);
}

void pit_install_handler(void)
{
    pit_set_phase(62799); /* fosc/62799 = 19.000015924 Hz */
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

void pit_diag(void)
{
    vga_color(VGA_BACK_BLACK, VGA_FORE_RED);
    printk(" === PIT DIAGNOSIS ===\n");
    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    printk("  pit_handler: 0x%x\n", &pit_handler);
    printk("  pit_info:    0x%x\n", &pit_info);
}
