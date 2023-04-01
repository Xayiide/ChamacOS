#ifndef _PMM_H_
#define _PMM_H_

#include <stdint.h>
#include "multiboot.h"

#define PMM_FRAME_SIZE 0x1000 /* 4096 */
#define PMM_NUM_FRAMES 100

/* Tenemos que almacenar la siguiente información:
 * - Dir inicio: 0x0020_0000 (Empezado el segundo MB para no borrar info
 *               del kernel)
 * - Tamaño mapa: 8 * PMM_NUM_FRAMES (1 byte por frame)
 * - Inicio mapa: Dir inicio + Tamaño mapa
 * 
 * Con esto ya podríamos empezar.
 * 1. Marcamos todos los frames como disponibles
 * 2. Marcamos los frames usados por el mapa como no disponibles
 * 3. Hacemos funciones:
 *    · Marcar un frame como disponible
 *    · Marcar un frame como no disponible
 */




void pmm_init(multiboot_info_t *mbd, uint32_t magic);

#endif