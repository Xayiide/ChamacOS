#include <stdint.h>    /* uint_t      */
#include "pmm.h"
#include "multiboot.h"

#include "vga.h"       /* printk      */
#include "sys.h"       /* panic, NULL */

typedef multiboot_memory_map_t multiboot_mmap_t; /* Ahorrar espacio */

/* TODO: hacer macro para convertir de indice del mapa a dirección, y
 * viceversa */

/* Definiciones estáticas */
static pmm_info_t pmm_meminfo =
    {.installed_mem = 0,
     .usable_mem    = PMM_MEM_SIZE,
     .free_mem      = 0,
     .map_addr      = (uint8_t *) PMM_BASE_ADDR,
     .map_size      = PMM_NUM_FRAMES};

static void    pmm_check_magic(multiboot_info_t *mbd, uint32_t magic);
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
    uint32_t i;
    for (i = 0; i < mbd->mmap_length; i += sizeof(multiboot_mmap_t))
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
        printk("(index: %d)\n", index);
        panic("pmm_map_entry_set: index out of range");
    }

    if (PMM_IS_FREE(status))
    {
        pmm_meminfo.free_mem += PMM_FRAME_SIZE;
    }
    else
    {
        pmm_meminfo.free_mem -= PMM_FRAME_SIZE;
    }

    //printk("free memory: %d\n", pmm_meminfo.free_mem);
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
    int   i;
    void *frame1;
    void *frame2;

    printk(" ===== MEMORY TEST =====\n");

    for (i = 0; i < 99; i++)
    {
        pmm_alloc_frame();
    }

    for (i = 1; i < 100; i++)
    {
        pmm_free_frame((void *) (PMM_BASE_ADDR + (i * 4096)));
    }

    frame1 = pmm_alloc_frames(50);
    printk("frame: 0x%x\n", frame1);
    frame2 = pmm_alloc_frames(25);
    printk("frame: 0x%x\n", frame2);

    pmm_free_frame(frame1);
    pmm_free_frame(frame2);

}




void pmm_init(multiboot_info_t *mbd, uint32_t magic)
{
    uint32_t i;


    pmm_check_magic(mbd, magic);
    
    pmm_fill_meminfo(mbd);

    pmm_printinfo(mbd);

    /* Pon a 0 la memoria de la tabla de frames */
    memset(pmm_meminfo.map_addr, PMM_FRAME_FREE, pmm_meminfo.map_size);


    pmm_meminfo.free_mem = 0;


    /* Marca el resto de frames como disponibles */
    for (i = 0; i < PMM_NUM_FRAMES; i++)
    {
        pmm_map_entry_set(i, PMM_FRAME_FREE);
    }

    /* Marca la primera frame como no disponible (es donde está la tabla) */
    pmm_map_entry_set(0, PMM_FRAME_USED);

    printk("Memoria libre: %d KB\n", pmm_meminfo.free_mem / 1024);

    pmm_test();

}

/*
 * Encuentra la primera frame libre y la marca como usada
*/
void *pmm_alloc_frame(void)
{
    uint32_t i;
    void *frame = NULL;

    if (pmm_meminfo.free_mem == 0)
    {
        return NULL;
    }

    for (i = 0; i < PMM_NUM_FRAMES; i++)
    {
        uint8_t map_entry = pmm_map_entry_get(i);
        if (PMM_IS_FREE(map_entry))
        {
            pmm_map_entry_set(i, PMM_FRAME_USED);
            frame = (void *) PMM_BASE_ADDR + (i * PMM_FRAME_SIZE);
            printk("Reservando 1 frame en la pos 0x%x (indice %d",
                    (uint32_t) frame, i);
            printk(", libre: %d KB)\n", pmm_meminfo.free_mem / 1024);
            break;
        }
    }

    return frame;
}

/*
 * Reserva espacio para n frames contiguas
*/
void *pmm_alloc_frames(uint32_t num_frames)
{
    uint32_t i;
    uint32_t found = 0;
    uint32_t index = 0;
    uint32_t cons  = 0;

    uint32_t entry;

    for (i = 0; i < PMM_NUM_FRAMES; i++)
    {
        entry = pmm_map_entry_get(i);
        if (PMM_IS_FREE(entry))
        {
            cons++;
            if (cons == 1)
            {
                index = i;
            }
            else if (cons == num_frames)
            {
                found = 1;
                break;
            }
        }
        else
        {
            cons = 0;
        }
    }

    if (found == 1)
    {
        printk("Reservando %d frames en la pos 0x%x (%d -> %d)\n",
                num_frames, PMM_INDX2ADDR(index),
                index, index + (num_frames - 1));
        pmm_map_entry_set(index,
                (PMM_FRAME_FIRST | PMM_FRAME_NEXT | PMM_FRAME_USED));
        for (i = index + 1; i < (num_frames + index) - 1; i++)
        {
            pmm_map_entry_set(i, PMM_FRAME_NEXT | PMM_FRAME_USED);
        }
        pmm_map_entry_set(i, PMM_FRAME_USED);
        return (void *) PMM_INDX2ADDR(index);
    }
    else
    {
        printk("No se han encontrado %d frames consecutivas libres\n",
                num_frames);
        return NULL;
    }

}

/*
 * Marca la entrada correspondiente a la dirección frame como libre
 */
void pmm_free_frame(void *frame)
{
    uint32_t index;
    uint8_t  entry;

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

    /* Calcula el índice a partir de la dirección */
    index = PMM_ADDR2INDX(frame);

    /* No debería poder darse este caso porque antes comprobamos que no
     * se pueda hacer free al primer frame (0x00200000 - 0x00201000) */
    if (index == 0)
    {
        panic("pmm_free_frame: something went very wrong");
    }

    entry = pmm_map_entry_get(index);
    /* Caso 1: La entrada ya está libre. Imprimimos un warning y ya está.
     * Caso 2: La entrada se reservó ella sola. La liberamos y ya.
     * Caso 3: La entrada es la primera. La liberamos y liberamos mientras
               todas las entradas siguientes que tengan PMM_FRAME_NEXT activo.
       Caso 4: La entrada tiene siguiente pero no es la primera. En este caso
               se ha llamado a free sobre una frame intermedia. Ignoramos este
               caso para no complicarlo por ahora. Podríamos recorrer hacia
               atrás hasta encontrar el primer frame y ahí aplicar el caso 3,
               pero por ahora ignoraremos este caso. */
    if (PMM_IS_FREE(entry) == 1)
    {
        printk("WARNING: Called free on an empty frame (indx: %d)\n", index);
    }
    else if (PMM_IS_FIRST(entry) == 0 && PMM_HAS_NEXT(entry) == 0)
    {
        pmm_map_entry_set(index, PMM_FRAME_FREE);
        printk("[2] Liberando entry (0x%x) en el indice %d", entry, index);
        printk(", libre: %d KB\n", pmm_meminfo.free_mem / 1024);
    }
    else if (PMM_IS_FIRST(entry) == 1)
    {
        pmm_map_entry_set(index, PMM_FRAME_FREE);
        printk("[3] Liberando entry (0x%x) en el indice %d", entry, index);
        printk(", libre: %d KB\n", pmm_meminfo.free_mem / 1024);
        printk("Liberando entries... ");
        index++;
        entry = pmm_map_entry_get(index);
        while (PMM_HAS_NEXT(entry) == 1 && PMM_IS_FIRST(entry) == 0)
        {
            pmm_map_entry_set(index, PMM_FRAME_FREE);
            printk("%d ", index);
            index++;
            entry = pmm_map_entry_get(index);
        }
        pmm_map_entry_set(index, PMM_FRAME_FREE);
        printk("%d ", index);
        printk("\n Libre: %d KB\n", pmm_meminfo.free_mem / 1024);
    }
    else if (PMM_IS_FIRST(entry) == 0 && PMM_HAS_NEXT(entry) == 1)
    {

    }
}


pmm_info_t *pmm_get_meminfo(void)
{
    return &pmm_meminfo;
}
