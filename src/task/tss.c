#include <stdint.h>

#include "drivers/vga.h" /* printk  */
#include "lib/sys.h"     /* memset  */
#include "tss.h"

extern void tss_load(void); /* boot.asm */

//static tss_entry_t ktss;

void tss_init(void)
{
    //ktss.cr3 = 0x00;
    //memset(&ktss, 0x00, sizeof(ktss));
    //printk("size of entry: \n");
    //printk("entry size: bytes\n");
    //tss_load();
}
