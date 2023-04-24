#include <stdint.h>    /* uint_t      */
#include <stddef.h>    /* size_t      */
#include "pmm.h"
#include "multiboot.h"

#include "vga.h"       /* printk      */
#include "sys.h"       /* panic, NULL */

static pmm_info_t meminfo =
{
        .installed_mem = 0,
        .free_mem      = 0,
        .pmm_map       = (uint8_t *) PMM_BASE_ADDR,
        .pmm_map_size  = 0,
        .num_frames    = 0
};

static void    pmm_check_magic(multiboot_info_t *mbd, uint32_t magic);
static void    pmm_fill_meminfo(multiboot_info_t *mbd);
static void    pmm_printinfo(void);

static void    pmm_map_init(multiboot_info_t *mbd);
static void    pmm_set_frame(void *frame, frame_status_t s);
static void    pmm_set_frames(void *frame, size_t num, frame_status_t s);

static void    pmm_map_entry_set(uint32_t index, frame_status_t s);
static uint8_t pmm_map_entry_get(uint32_t index, frame_status_t s);

static void    pmm_test(void);

void pmm_init(multiboot_info_t *mbd, uint32_t magic)
{
    pmm_check_magic(mbd, magic);

    pmm_fill_meminfo(mbd);


    pmm_map_init(mbd);
}

static void pmm_check_magic(multiboot_info_t *mbd, uint32_t magic)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        panic("MULTIBOOT: Numero magico invalido");
    }

    if (!(mbd->flags >> 6 & 0x01))
    {
        panic("MULTIBOOT: Mapa de memoria invalido");
    }
}

static void pmm_fill_meminfo(multiboot_info_t *mbd)
{
    uint32_t i;
    multiboot_memory_map_t *mmmt;

    for (i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t))
    {
        mmmt = (multiboot_memory_map_t *) (mbd->mmap_addr + i);
        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE ||
            mmmt->type == MULTIBOOT_MEMORY_RESERVED  ||
            mmmt->type == MULTIBOOT_MEMORY_ACPI_RECLAIMABLE ||
            mmmt->type == MULTIBOOT_MEMORY_NVS       ||
            mmmt->type == MULTIBOOT_MEMORY_BADRAM)
        {
            meminfo.installed_mem += mmmt->len_lo;
        }

        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            vga_color(VGA_BACK_BLACK, VGA_FORE_CYAN);
            printk("Memoria usable: ");
            printk(" [0x%x -> 0x%x] (%d) KBs\n", mmmt->addr_lo,
                    (mmmt->addr_lo + mmmt->len_lo), mmmt->len_lo/1024);
        }
    }

    meminfo.num_frames   = meminfo.installed_mem / PMM_FRAME_SIZE;
    meminfo.pmm_map_size = meminfo.num_frames / 8;

    pmm_printinfo();
    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
}

static void pmm_printinfo(void)
{
    printk("Memoria instalada: %d bytes\n", meminfo.installed_mem);
    printk("Numero de frames:  %d\n", meminfo.num_frames);
    printk("Tamanio del mapa:  %d bytes ", meminfo.pmm_map_size);
    printk("en la dir 0x%x\n", meminfo.pmm_map);
}

static void pmm_map_init(multiboot_info_t *mbd)
{
    uint32_t i;
    multiboot_memory_map_t *mmmt;

    /* Ponemos a unos el mapa de memoria. Esto nos conviene para luego marcar
     * como disponibles sólo los rangos que nos interesan (y conocemos bien).
     * El resto mejor que queden como inusables, a riesgo de romper algo.  */
    memset(meminfo.pmm_map, 0xFF, meminfo.pmm_map_size);

    /* Iteramos sobre todas las secciones. Si están disponibles para su uso,
     * las marcamos como libres. El resto fueron marcadas como usadas. */
    for (i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t))
    {
        mmmt = (multiboot_memory_map_t *) (mbd->mmap_addr + i);
        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            pmm_set_frames((void *) mmmt->addr_lo,
                           mmmt->len_lo / PMM_FRAME_SIZE,
                           FRAME_FREE);
        }
    }

    /* Marcamos el primer MB de memoria como usado (boot, memoria VGA, el
     * mismo mapa de memoria (0x90000), etc. */
    pmm_set_frames((void *) 0x0, 0x00100000 / PMM_FRAME_SIZE, FRAME_USED);

    /* Marcamos el frame correspondiente al mapa de memoria (0x90000) como
     * usado. Esto no sería necesario porque lo acabamos de hacer, pero en
     * caso de que cambiemos la dirección del mapa pues viene bien */
    pmm_set_frames(meminfo.pmm_map, meminfo.pmm_map_size / PMM_FRAME_SIZE,
                   FRAME_USED);

}

static void pmm_set_frame(void *frame, frame_status_t s)
{
    uint32_t addr;
    uint32_t indx;

    addr = (uint32_t) frame;

    if (addr > meminfo.installed_mem)
        panic("pmm_set_frame: frame out of bounds");

    indx = addr / PMM_FRAME_SIZE;
    pmm_map_entry_set(indx, s);
    if (s == FRAME_FREE)
        meminfo.free_mem += PMM_FRAME_SIZE;
    else if (s == FRAME_USED)
        meminfo.free_mem -= PMM_FRAME_SIZE;
}

static void pmm_set_frames(void *frame, size_t num, frame_status_t s)
{
    uint32_t i;

    for (i = 0; i < num; i++)
        pmm_set_frame(frame + (i * PMM_FRAME_SIZE), s);

}


static void pmm_map_entry_set(uint32_t index, frame_status_t s)
{
    uint8_t  bit;
    uint32_t slot;

    if (index > meminfo.num_frames)
        panic("pmm_map_entry_set: index out of range");

    bit  = index % 8;
    /* index - bit: para redondear y que la div entera sea correcta */
    slot = (index - bit) / 8;
    meminfo.pmm_map[slot] |= (s << bit);

    return;
}

static uint8_t pmm_map_entry_get(uint32_t index, frame_status_t status)
{
    uint8_t  bit;
    uint32_t slot;
    uint8_t  val;
    uint8_t  ret = 0;

    if (index > meminfo.num_frames)
        panic("pmm_map_entry_get: index out of range");

    bit  = index % 8;
    /* index - bit: para redondear y que la div entera sea correcta */
    slot = (index - bit) / 8;
    val  = meminfo.pmm_map[slot];

    ret = val & (0b10000000 >> bit);

    return ret;
}
//static void    pmm_test(void);
