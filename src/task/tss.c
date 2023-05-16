#include <stdint.h>

#include "drivers/vga.h" /* printk  */
#include "lib/sys.h"     /* memset  */
#include "cpu/gdt.h"     /* gdt_set */
#include "tss.h"

extern void tss_load(void); /* boot.asm */

static tss_entry_t tss_entry;

void tss_init(void)
{
    memset(&tss_entry, 0x00, sizeof(tss_entry_t));
    tss_load();
}

#if 0
void tss_diag(void)
{
    vga_color(VGA_BACK_BLACK, VGA_FORE_RED);
    printk(" === TSS DIAGNOSIS ===\n");
    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    printk(" tss entry at:  0x%x\n", &tss_entry);
    printk(" size of entry: %d bytes\n", sizeof(tss_entry));
}
#endif
