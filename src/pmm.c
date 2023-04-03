
#include <stdint.h>    /* uint_t */
#include "pmm.h"
#include "multiboot.h"

#include "vga.h"       /* printk */
#include "sys.h"       /* panic  */

typedef multiboot_memory_map_t multiboot_mmap_t; /* Ahorrar espacio */

/* Definiciones estáticas */
static pmm_info_t pmm_meminfo =
    {.installed_mem = 0,
     .usable_mem    = PMM_MEM_SIZE,
     .free_mem      = 0,
     .map_addr      = (uint8_t *) PMM_BASE_ADDR,
     .map_size      = PMM_NUM_FRAMES};

static void    pmm_validate(multiboot_info_t *mbd, uint32_t magic);
static void    pmm_printinfo(multiboot_info_t *mbd);
static void    pmm_map_frame_set(uint32_t index, uint8_t status);
static uint8_t pmm_map_frame_get(uint32_t index);

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

static void pmm_printinfo(multiboot_info_t *mbd)
{
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
            pmm_meminfo.installed_mem += mmmt->len_lo;
        }

        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE &&
            mmmt->addr_lo >= 0x00100000) /* punto de entrada */
        {
            vga_color(VGA_BACK_BLACK, VGA_FORE_CYAN);
            printk("Memoria usable: ");
            printk("%d KBs a partir de la dir. 0x%x\n",
                (PMM_FRAME_SIZE * PMM_NUM_FRAMES) / 1024, mmmt->addr_lo);

            vga_color(VGA_BACK_BLACK, VGA_FORE_GREEN);
        }
    }

    printk("pmm_bitmap: 0x%x\n", (uint32_t ) pmm_meminfo.map_addr);
    printk("pmm_bitmap size: %d B (%d bits)\n", pmm_meminfo.map_size,
        pmm_meminfo.map_size * 8);

    printk("Memoria instalada: 0x%x (%d MB)\n", pmm_meminfo.installed_mem,
        pmm_meminfo.installed_mem/1024/1024);

    printk("Tamano de pagina:  %d Bytes\n", PMM_FRAME_SIZE);
    printk("Numero de paginas: %d\n", PMM_NUM_FRAMES);
    printk("Memoria fisica usable: %d KB\n", pmm_meminfo.usable_mem / 1024);

    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);

}

static void pmm_map_frame_set(uint32_t index, uint8_t status)
{
    pmm_meminfo.map_addr[index] = status;
}

static uint8_t pmm_map_frame_get(uint32_t index)
{
    uint8_t frame;

    frame = pmm_meminfo.map_addr[index];

    return frame;
}






void pmm_init(multiboot_info_t *mbd, uint32_t magic)
{
    pmm_validate(mbd, magic);
    pmm_printinfo(mbd);

    memset(pmm_meminfo.map_addr, PMM_FRAME_FREE, pmm_meminfo.map_size);

    
}


pmm_info_t *pmm_get_meminfo(void)
{
    return &pmm_meminfo;
}