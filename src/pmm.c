
#include <stdint.h>    /* uint_t */
#include "pmm.h"
#include "multiboot.h"

#include "vga.h"       /* printk */
#include "sys.h"       /* panic  */

typedef multiboot_memory_map_t multiboot_mmap_t; /* Ahorrar espacio */

static void pmm_validate(multiboot_info_t *mbd, uint32_t magic);

uint32_t  total_memory = 0;
uint32_t  free_memory  = 0;

uint8_t  *pmm_bitmap   = NULL;
uint32_t  bitmap_size  = PMM_NUM_FRAMES * sizeof(uint8_t);


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
    
    vga_color(VGA_BACK_BLACK, VGA_FORE_GREEN);
    for (int i = 0; i < mbd->mmap_length; i += sizeof(multiboot_mmap_t))
    {
        multiboot_mmap_t *mmmt = (multiboot_mmap_t *) (mbd->mmap_addr + i);


        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE ||
            mmmt->type == MULTIBOOT_MEMORY_RESERVED  ||
            mmmt->type == MULTIBOOT_MEMORY_ACPI_RECLAIMABLE ||
            mmmt->type == MULTIBOOT_MEMORY_NVS       ||
            mmmt->type == MULTIBOOT_MEMORY_BADRAM)
        {
            total_memory += mmmt->len_lo;
        }

        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE &&
            mmmt->addr_lo >= 0x00100000) /* punto de entrada */
        {
            pmm_bitmap  = (uint8_t *) mmmt->addr_lo;
            vga_color(VGA_BACK_BLACK, VGA_FORE_CYAN);
            printk("Memoria usable: ");
            printk("%d KBs a partir de la dir. 0x%x\n",
                (PMM_FRAME_SIZE * PMM_NUM_FRAMES) / 1024, mmmt->addr_lo);

            vga_color(VGA_BACK_BLACK, VGA_FORE_GREEN);
        }
    }

    printk("pmm_bitmap: 0x%x\n", pmm_bitmap);
    printk("pmm_bitmap size: %d B\n", bitmap_size);

    printk("Memoria total: 0x%x (%d MB)\n", total_memory,
        total_memory/1024/1024);

    printk("Tamano de pagina:  %d Bytes\n", PMM_FRAME_SIZE);
    printk("Numero de paginas: %d\n", PMM_NUM_FRAMES);
    printk("Total de memoria fisica: %x\n", PMM_FRAME_SIZE * PMM_NUM_FRAMES);
    printk("                        (%d)\n", PMM_FRAME_SIZE * PMM_NUM_FRAMES);
    printk("                        (%d KB)\n", (PMM_FRAME_SIZE * PMM_NUM_FRAMES) / 1024);

    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
}
