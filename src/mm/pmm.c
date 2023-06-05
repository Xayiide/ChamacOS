#include <stdint.h>      /* uint_t      */
#include <stddef.h>      /* size_t      */

#include "pmm.h"
#include "kernel/multiboot.h"
#include "drivers/vga.h" /* printk      */
#include "lib/sys.h"     /* panic, NULL */

static pmm_info_t meminfo =
{
        .installed_mem = 0,
        .free_mem      = 0,
        .pmm_map       = (uint8_t *) PMM_BASE_ADDR,
        .pmm_map_size  = 0,
        .num_frames    = 0,
        .first_free    = 0
};

static void    pmm_check_magic(multiboot_info_t *mbd, uint32_t magic);
static void    pmm_fill_meminfo(multiboot_info_t *mbd);
static void    pmm_printinfo(void);

static void    pmm_map_init(multiboot_info_t *mbd);
static void    pmm_set_first(void);
static void    pmm_set_frame(void *frame, frame_status_t s);
static void    pmm_set_frames(void *frame, size_t num, frame_status_t s);

static void    pmm_map_entry_set(uint32_t index, frame_status_t s);
static frame_status_t pmm_map_entry_get(uint32_t index);


void pmm_init(multiboot_info_t *mbd, uint32_t magic)
{
    pmm_check_magic(mbd, magic);
    pmm_fill_meminfo(mbd);
    pmm_map_init(mbd);
    pmm_printinfo();
    pmm_set_first();
}

void *pmm_alloc_frame(void)
{
    void *addr;

    /* No hay memoria libre */
    if (meminfo.free_mem == 0 || meminfo.first_free == 0)
    {
#ifdef DEBUG_ALLOC
        printk("pmm: free_mem: %d || first_free = %d\n", meminfo.free_mem,
                meminfo.first_free);
#endif
        return NULL;
    }

    addr = PMM_INDX2ADDR(meminfo.first_free);
    pmm_set_frame(addr, FRAME_USED);
    pmm_set_first();

    return addr;
}

void pmm_frame_free(void *frame)
{
    pmm_set_frame(frame, FRAME_FREE);
    pmm_set_first();
}

void pmm_diag(void)
{
    vga_color(VGA_BACK_BLACK, VGA_FORE_RED);
    printk(" === PMM DIAGNOSIS ===\n");
    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    printk("  installed memory: %d MB\n", meminfo.installed_mem/1024/1024);
    printk("  free memory:      %d MB\n", meminfo.free_mem/1024/1024);
    printk("  dir. del mapa:    0x%x\n", meminfo.pmm_map);
    printk("  tam. del mapa:    %d bytes\n", meminfo.pmm_map_size);
    printk("  numero de frames: %d\n", meminfo.num_frames);
    printk("  1er ind. libre:   %d [0x%x]\n", meminfo.first_free,
            PMM_INDX2ADDR(meminfo.first_free));
    printk("  Dir. meminfo:     0x%x\n", &meminfo);
}

pmm_info_t *pmm_get_meminfo(void)
{
    return &meminfo;
}


static void pmm_check_magic(multiboot_info_t *mbd, uint32_t magic)
{
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
        panic("MULTIBOOT: Numero magico invalido");

    if (!(mbd->flags >> 6 & 0x01))
        panic("MULTIBOOT: Mapa de memoria invalido");

    printk("MB Magic: 0x%x\n", magic);
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
            printk(" [0x%x -> 0x%x] (%d KBs)\n", mmmt->addr_lo,
                    (mmmt->addr_lo + mmmt->len_lo), mmmt->len_lo/1024);
        }
    }

    meminfo.num_frames   = meminfo.installed_mem / PMM_FRAME_SIZE;
    meminfo.pmm_map_size = meminfo.num_frames / 8;

    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
}

static void pmm_printinfo(void)
{
    vga_color(VGA_BACK_BLACK, VGA_FORE_CYAN);
    printk("Memoria instalada: %d bytes\n", meminfo.installed_mem);
    printk("Numero de frames:  %d\n", meminfo.num_frames);
    printk("Tamanio del mapa:  %d bytes ", meminfo.pmm_map_size);
    printk("en la dir 0x%x [indice %d]\n", meminfo.pmm_map,
            PMM_ADDR2INDX(meminfo.pmm_map));
    printk("1er marco libre:   %d [0x%x]\n", meminfo.first_free,
            PMM_INDX2ADDR(meminfo.first_free));
    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
}

static void pmm_map_init(multiboot_info_t *mbd)
{
    uint32_t i;
    multiboot_memory_map_t *mmmt;
    kmem_info_t *kmem;

    /* Ponemos a unos el mapa de memoria. Esto nos conviene para luego marcar
     * como disponibles sólo los rangos que nos interesan (y conocemos bien).
     * El resto mejor que queden como inusables, a riesgo de romper algo.  */
    memset(meminfo.pmm_map, 0xFF, meminfo.pmm_map_size);

    kmem = sys_get_kmem_info();

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
    pmm_set_frames((void *) 0x0, (uint32_t) kmem->krn_start / PMM_FRAME_SIZE,
                   FRAME_USED);

    /* Marcamos el frame correspondiente al mapa de memoria (0x90000) como
     * usado. Esto no sería necesario porque lo acabamos de hacer, pero en
     * caso de que cambiemos la dirección del mapa pues viene bien */
    pmm_set_frames(meminfo.pmm_map, meminfo.pmm_map_size / PMM_FRAME_SIZE,
                   FRAME_USED);

    /* Marcar como usadas los marcos correspondientes al código, stack y datos
     * del kernel. Esta información nos la da el linker script. */
    pmm_set_frames((void *) kmem->krn_start,
                   (kmem->krn_end - kmem->krn_start) / PMM_FRAME_SIZE,
                   FRAME_USED);
}

static void pmm_set_first(void)
{
    uint32_t i, found = 0;
    for (i = 0; i < meminfo.num_frames; i++)
    {
        if (pmm_map_entry_get(i) == FRAME_FREE)
        {
            meminfo.first_free = i;
            found = 1;
            break;
        }
    }

    if (found == 0)
        meminfo.first_free = 0;
}

static void pmm_set_frame(void *frame, frame_status_t s)
{
    uint32_t addr, indx;

    addr = (uint32_t) frame;

    if (addr > meminfo.installed_mem)
        panic("pmm_set_frame: frame out of bounds");

    indx = addr / PMM_FRAME_SIZE;
    pmm_map_entry_set(indx, s);

    if (s == FRAME_FREE)
        meminfo.free_mem += PMM_FRAME_SIZE;
    else if (s == FRAME_USED)
        meminfo.free_mem -= PMM_FRAME_SIZE;

    pmm_set_first();
}

static void pmm_set_frames(void *frame, size_t num, frame_status_t s)
{
    uint32_t i;

#ifdef DEBUG_PMM
    uint32_t end = (uint32_t) frame + ((num - 1) * PMM_FRAME_SIZE);
    printk(" [%x [%d] => %x [%d]] %s\n", frame,
                                         PMM_ADDR2INDX(frame),
                                         end,
                                         PMM_ADDR2INDX(end),
                                         (s == FRAME_FREE ? "FREE" : "USED"));
#endif

    for (i = 0; i < num; i++)
        pmm_set_frame(frame + (i * PMM_FRAME_SIZE), s);
}


/* TODO: Estas 2 bien podrían estar en otro fichero para que no sea tan lioso */
static void pmm_map_entry_set(uint32_t index, frame_status_t s)
{
    uint8_t  bit, byte, aux, mask;
    uint32_t slot;

    if (index > meminfo.num_frames)
        panic("pmm_map_entry_set: index out of range");

    bit  = index % 8;
    /* index - bit: para redondear y que la div entera sea correcta */
    slot = (index - bit) / 8;
    byte = meminfo.pmm_map[slot];
    mask = 0x80 >> bit;  /* Todo a 0 menos el bit que se quiere cambiar */
    aux  = byte & ~mask; /* Mantenemos todo menos el bit a cambiar */
    aux |= ((mask && (uint8_t) s) << 7) >> bit;

    meminfo.pmm_map[slot] = aux;
}

static frame_status_t pmm_map_entry_get(uint32_t index)
{
    frame_status_t ret;
    uint8_t  bit, val, aux;
    uint32_t slot;

    if (index > meminfo.num_frames)
        panic("pmm_map_entry_get: index out of range");

    bit  = index % 8;
    /* index - bit: para redondear y que la div entera sea correcta */
    slot = (index - bit) / 8;
    val  = meminfo.pmm_map[slot];
    aux  = val & (0b10000000 >> bit);

    if (aux == 0x00)
        ret = FRAME_FREE;
    else
        ret = FRAME_USED;

    return ret;
}
