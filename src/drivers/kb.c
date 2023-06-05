#include <stdint.h>      /* uint_t */

#include "kb.h"
#include "drivers/vga.h" /* printk              */
#include "lib/sys.h"     /* regs_t              */
#include "cpu/io.h"      /* inb outb            */
#include "cpu/irq.h"     /* irq_install_handler */
#include "cpu/pit.h"     /* pit_get_ticks       */

/* Esta función se llama dos veces cada vez que pulso una tecla (una cuando
 * pulso, otra cuando suelto) */
void kb_handler(regs_t *r)
{
    (void) r;
    static uint16_t div   = 0;

    uint32_t ticks;
    uint32_t uptime;
    uint8_t  scancode;

    uint16_t count;


    scancode = inb(IO_KB_DAT);

    if (scancode & KB_KEY_RELEASED)
    {
        /* Para comprobar shift, alt, ctrl, ... */
    }
    else
    {
        ticks  = pit_get_num_ints();
        uptime = sys_uptime();

        if (scancode == 0x18) /* tecla o */
        {
            div += 2000;
            printk("[Timer ticks: %d (%d s) - 0x%x | ", ticks, uptime, scancode);
            printk("%d]\n", div);
            pit_set_phase(div);
        }
        else if (scancode == 0x19) /* tecla p */
        {
            div -= 2000;
            printk("[Timer ticks: %d (%d s) - 0x%x | ", ticks, uptime, scancode);
            printk("%d]\n", div);
            pit_set_phase(div);
        }
        else if (scancode == 0x1A) /* La tecla de la dcha de la p: ^`[ */
        {
            count = pit_get_count();
            printk("[PIT Counter: %d/%d\n", count, PIT_OSC_FQ / pit_get_freq());
        }
    }


}

void kb_install_handler()
{
    uint8_t kb_stat;

    irq_install_handler(1, kb_handler);

    kb_stat = inb(IO_KB_STA);
#ifdef DEBUG_KB
    printk("El registro de estado del teclado: 0x%x\n", kb_stat);
#else
    kb_stat += kb_stat;
#endif
}
