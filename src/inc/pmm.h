#ifndef _PMM_H_
#define _PMM_H_

#include <stdint.h>
#include "multiboot.h"

#define PMM_BASE_ADDR  0x00200000
#define PMM_FRAME_SIZE 0x1000 /* 4096 */
#define PMM_NUM_FRAMES 100
#define PMM_MEM_SIZE   PMM_FRAME_SIZE * PMM_NUM_FRAMES /* 400 KB */

/* Frame entry */
#define PMM_FRAME_FREE  0x00 /* 0000 0000 */
#define PMM_FRAME_USED  0x01 /* 0000 0001 */
#define PMM_FRAME_NEXT  0x41 /* 0100 0000 -> FRAME_NEXT incluye FRAME_USED  */
#define PMM_FRAME_FIRST 0xC1 /* 1100 0000 -> FRAME_FIRST incluye FRAME_NEXT */

typedef struct 
{
    uint32_t installed_mem; /* RAM total instalada (128 MB)  */
    uint32_t usable_mem;    /* RAM total que usamos (400 KB) */
    uint32_t free_mem;      /* De la RAM usable, la no usada */
    uint8_t *map_addr;      /* Dir. de la tabla de frames    */
    uint32_t map_size;      /* Tamaño de la tabla de frames  */
} pmm_info_t;



#define PMM_IS_FREE(f)                          \
({                                              \
    ((f & PMM_FRAME_USED) == PMM_FRAME_USED) ?  \
        0 : 1;                                  \
})

#define PMM_HAS_NEXT(f)                         \
({                                              \
    ((f & PMM_FRAME_NEXT) == PMM_FRAME_NEXT) ?  \
        1 : 0;                                  \
})

#define PMM_IS_FIRST(f)                          \
({                                               \
    ((f & PMM_FRAME_FIRST) == PMM_FRAME_FIRST) ? \
        1 : 0;                                   \
})


#define PMM_INDX2ADDR(indx) (PMM_BASE_ADDR + (indx * PMM_FRAME_SIZE))

#define PMM_ADDR2INDX(addr) (uint32_t) (addr - PMM_BASE_ADDR) / PMM_FRAME_SIZE

/* Tenemos que almacenar la siguiente información:
 * - Dir inicio: 0x0020_0000 (Empezado el segundo MB para no borrar info
 *               del kernel)
 * - Tamaño mapa: PMM_NUM_FRAMES (1 byte por frame) (x8 para tam. en bits)
 * - Inicio mapa: Dir inicio + Tamaño mapa
 * 
 * Con esto ya podríamos empezar.
 * 1. Marcamos todos los frames como disponibles
 * 2. Marcamos los frames usados por el mapa como no disponibles
 * 3. Hacemos funciones:
 *    · Marcar un frame como disponible
 *    · Marcar un frame como no disponible
 */





void  pmm_init(multiboot_info_t *mbd, uint32_t magic);
void *pmm_alloc_frame(void);
void *pmm_alloc_frames(uint32_t n);
void  pmm_free_frame(void *frame);
void  pmm_diag(void);

pmm_info_t *pmm_get_meminfo(void);

#endif
