
#include <stdint.h>    /* uint_t */
#include "pmm.h"
#include "multiboot.h"

#include "vga.h"       /* printk */
#include "sys.h"       /* panic  */

typedef multiboot_memory_map_t multiboot_mmap_t; /* Ahorrar espacio */

static void pmm_validate(multiboot_info_t *mbd, uint32_t magic);


static void pmm_validate(multiboot_info_t *mbd, uint32_t magic)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        panic("MULTIBOOT: Numero magico invalido");
    }

    if (!(mbd->flags >> 6 & 0x1))
    {
        panic("MULTIBOOT: Mapa de memoria de GRUB invalido");
    }

}

void pmm_init(multiboot_info_t *mbd, uint32_t magic)
{
    pmm_validate(mbd, magic);

    for (int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_mmap_t))
    {
        multiboot_mmap_t *mmmt = (multiboot_mmap_t *) (mbd->mmap_addr + i);
        //printk("Start addr: 0x%x_%x | ", mmmt->addr_hi, mmmt->addr_lo);
        //printk("length: 0x%x_%x | ", mmmt->len_hi, mmmt->len_lo);
        //printk("size: 0x%x | type: %d\n", mmmt->size, mmmt->type);

        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE &&
            mmmt->addr_lo >= 0x00100000) /* punto de entrada */
        {
            printk("Memoria usable: \n\t");
            printk("start addr: 0x%x_%x | ", mmmt->addr_hi, mmmt->addr_lo);
            printk("length: 0x%x_%x | ", mmmt->len_hi, mmmt->len_lo);
            printk("size: 0x%x\n", mmmt->size);
        }
    }
}
