#include <stdint.h> /* uint_t */

#include "kb.h"
#include "sys.h" /* regs_t              */
#include "io.h"  /* inb outb            */
#include "vga.h" /* pritnk              */
#include "irq.h" /* irq_install_handler */

#include "pit.h" /* pit_get_ticks       */

/* Esta función se está llamando dos veces cada vez que pulso una tecla (una
 * cuando pulso, otra cuando suelto). Para solucionarlo por ahora pongo un
 * contador estático */
void kb_handler(regs_t *r)
{
    static uint8_t  count = 0;
    static uint16_t div   = 0;

    uint32_t ticks;
    uint32_t uptime;
    uint8_t  scancode;


    count++;

    scancode = inb(IO_KB_DAT);

    if (count == 2) {
        count  = 0;
        ticks  = pit_get_ticks();
        //ticks  = (uint32_t) pit_get_count();
        uptime = sys_uptime();
        printk("[Timer ticks: %d (%d s)]\n", ticks, uptime);
        pit_set_phase(div);
        div += 2000;
    }

    if (scancode) {}; /* Para que no se queje el compilador */

}

void kb_install_handler()
{
    uint8_t kb_stat;

    irq_install_handler(1, kb_handler);

    kb_stat = inb(IO_KB_STA);
    printk("El registro de estado del teclado: 0x%x\n", kb_stat);
}
