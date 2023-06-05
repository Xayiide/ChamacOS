#ifndef _PMM_H_
#define _PMM_H_

#include <stdint.h> /* uint_t */
#include "kernel/multiboot.h"

typedef struct
{
    uint32_t installed_mem;
    uint32_t free_mem;
    uint8_t *pmm_map;
    uint32_t pmm_map_size;
    uint32_t num_frames;
    uint32_t first_free;
} pmm_info_t;

typedef enum
{
    FRAME_FREE = 0x00,
    FRAME_USED = 0x01
} frame_status_t;

#define PMM_BASE_ADDR  0x00090000
#define PMM_FRAME_SIZE 0x1000 /* 4096 bits */

#define PMM_INDX2ADDR(indx) (void *) (indx * PMM_FRAME_SIZE)
#define PMM_ADDR2INDX(addr) (uint32_t) ((uint32_t) addr / PMM_FRAME_SIZE)

void pmm_init(multiboot_info_t *mbd, uint32_t magic);
void *pmm_alloc_frame(void);
void pmm_free_frame(void *frame);
void pmm_diag(void);

pmm_info_t *pmm_get_meminfo(void);

#endif
