
#include <stdint.h>    /* uint_t      */
#include "pmm.h"
#include "multiboot.h"

#include "vga.h"       /* printk      */
#include "sys.h"       /* panic, NULL */

typedef multiboot_memory_map_t multiboot_mmap_t; /* Ahorrar espacio */

/* Definiciones estáticas */
static pmm_info_t pmm_meminfo =
    {.installed_mem = 0,
     .usable_mem    = PMM_MEM_SIZE,
     .free_mem      = 0,
     .map_addr      = (uint8_t *) PMM_BASE_ADDR,
     .map_size      = PMM_NUM_FRAMES};

static void    pmm_validate(multiboot_info_t *mbd, uint32_t magic);
static void    pmm_fill_meminfo(multiboot_info_t *mbd);
static void    pmm_printinfo(multiboot_info_t *mbd);
static void    pmm_map_entry_set(uint32_t index, uint8_t status);
static uint8_t pmm_map_entry_get(uint32_t index);

static void    pmm_test(void);



static void pmm_check_magic(multiboot_info_t *mbd, uint32_t magic)
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

static void pmm_fill_meminfo(multiboot_info_t *mbd) {
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

            vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
        }
    }
}

static void pmm_printinfo(multiboot_info_t *mbd)
{
    vga_color(VGA_BACK_BLACK, VGA_FORE_GREEN);

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

static void pmm_map_entry_set(uint32_t index, uint8_t status)
{
    if (index >= PMM_NUM_FRAMES)
    {
        panic("pmm_map_entry_set: index out of range");
    }

    if ((status & PMM_FRAME_USED) == PMM_FRAME_FREE)
    {
        pmm_meminfo.free_mem += PMM_FRAME_SIZE;
    }
    else
    {
        pmm_meminfo.free_mem -= PMM_FRAME_SIZE;
    }

    pmm_meminfo.map_addr[index] = status;
}

static uint8_t pmm_map_entry_get(uint32_t index)
{
    uint8_t frame;

    if (index >= PMM_NUM_FRAMES)
    {
        panic("pmm_map_entry_get: index out of range");
    }

    frame = pmm_meminfo.map_addr[index];

    return frame;
}

static void pmm_test(void)
{

}




void pmm_init(multiboot_info_t *mbd, uint32_t magic)
{
    pmm_check_magic(mbd, magic);
    
    pmm_fill_meminfo(mbd);

    pmm_printinfo(mbd);

    /* Pon a 0 la memoria de la tabla de frames */
    memset(pmm_meminfo.map_addr, PMM_FRAME_FREE, pmm_meminfo.map_size);

    pmm_meminfo.free_mem = 0;


    /* Marca la primera frame como no disponible (es donde está la tabla) */
    pmm_map_entry_set(0, PMM_FRAME_USED);

    /* Marca el resto de frames como disponibles */
    for (int i = 1; i < PMM_NUM_FRAMES; i++)
    {
        pmm_map_entry_set(i, PMM_FRAME_FREE);
    }

    pmm_test();

}

/*
 * Encuentra la primera frame libre y la marca como usada
*/
void *pmm_alloc_frame(void)
{
    void *frame = NULL;

    if (pmm_meminfo.free_mem == 0)
    {
        return NULL;
    }

    for (int i = 0; i < PMM_NUM_FRAMES; i++)
    {
        uint8_t map_entry = pmm_map_entry_get(i);
        if ((map_entry & PMM_FRAME_USED) == PMM_FRAME_FREE)
        {
            pmm_map_entry_set(i, PMM_FRAME_USED);
            frame = (void *) PMM_BASE_ADDR + (i * PMM_FRAME_SIZE);
        }
    }

    return frame;
}

/*
 * Reserva espacio para n frames contiguas
*/
void *pmm_alloc_frames(uint32_t num_frames)
{
    /* Buscar num_frames frames contiguas
        Si no las encuentra, retorna NULL.
        Si sí las encuentra, marcarlas como usadas y retornar puntero a la
         primera
    */


    return NULL;
}

void pmm_free_frame(void *frame)
{
    /* 1. Convertir de dir. a índice de frame
     * 2. Marcar el índice de ese frame como disponible
     * (TODO: Comprobar si se han reservado varios frames contiguos y
     *  liberarlos todos adecuadamente) */
    uint32_t  index;
    
    /* Comprueba los límites.
     * Dentro de estos límites: No se puede hacer free al primer frame
     *  porque ahí está el mapa de memoria. Tampoco se puede hacer free a
     *  nada por debajo de esa dirección porque es memoria del kernel.
     *  Tampoco a direcciones por encima de la memoria usable (400 KB
     *  a partir de 0x00200000) */
    if ((uint32_t) frame < (PMM_BASE_ADDR + PMM_FRAME_SIZE) ||
        (uint32_t) frame > (PMM_BASE_ADDR + PMM_MEM_SIZE))
    {
        panic("pmm_free_frame: free out of bounds");
    }

    if ((uint32_t) frame % PMM_FRAME_SIZE != 0)
    {
        panic("pmm_free_frame: free unaligned");
    }

    index = (uint32_t) (frame - PMM_BASE_ADDR) / PMM_FRAME_SIZE;

    /* No debería poder darse este caso porque antes comprobamos que no
     * se pueda hacer free al primer frame (0x200000 - 0x201000) */
    if (index == 0)
    {
        panic("pmm_free_frame: something went very wrong");
    }

    /* TODO: considerar varios frames reservados de forma contigua
     * (FRAME_FIRST, FRAME_NEXT) */
    pmm_map_entry_set(index, PMM_FRAME_FREE);
    pmm_meminfo.free_mem += PMM_FRAME_SIZE;
}


pmm_info_t *pmm_get_meminfo(void)
{
    return &pmm_meminfo;
}
