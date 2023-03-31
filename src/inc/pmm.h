#ifndef _PMM_H_
#define _PMM_H_

#include <stdint.h>

#include "multiboot.h"

void pmm_init(multiboot_info_t *mbd, uint32_t magic);

#endif