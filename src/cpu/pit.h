#ifndef _PIT_H_
#define _PIT_H_

#include <stdint.h> /* uint_t */

/* La frecuencia más baja que se puede conseguir es poniendo el divisor de
 * frecuencia a 0 (lo hace la BIOS por defecto) lo cual da un divisor con
 * valor 65536. Esto provoca una frecuencia de salida de 18.2065 Hz, es decir,
 * un IRQ cada 54.9254 ms. Esta es nuestra configuración por defecto. */

/* Divisores útiles:
 * · 205:   5820.4       Hz
 * · 1192:  1000.99161   Hz
 * · 1193:  1000.15255   Hz
 * · 11831:  100.006     Hz
 * · 11832:   99.99849   Hz
 * · 62799:   19.0000159 Hz
 */

#define PIT_OSC_FQ   1193182 /* Frecuencia del osc. del PIT: 1.193182 MHz */

/* Channel select     [BB______] */
#define PIT_SEL_CH0  0b00000000
#define PIT_SEL_CH1  0b01000000
#define PIT_SEL_CH2  0b10000000
#define PIT_SEL_RB   0b11000000 /* Read-back command (8254 only)    */

/* Access mode        [__BB____] */
#define PIT_AM_LCV   0b00000000 /* Latch count value command        */
#define PIT_AM_LO    0b00010000 /* lobyte only                      */
#define PIT_AM_HI    0b00100000 /* hibyte only                      */
#define PIT_AM_LOHI  0b00110000 /* lobyte/hibyte                    */

/* Operating mode     [____BBB_] */
#define PIT_OPM_0    0b00000000 /* Interrupt on terminal count      */
#define PIT_OPM_1    0b00000010 /* Hardware re-triggerable one-shot */
#define PIT_OPM_2    0b00000100 /* Rate generator                   */
#define PIT_OPM_3    0b00000110 /* Square wave generator            */
#define PIT_OPM_4    0b00001000 /* Software triggered strobe        */
#define PIT_OPM_5    0b00001010 /* Hardware triggered strobe        */

/* Binary/BCD         [_______B] */
#define PIT_BIN      0b00000000 /* 16-bit binary                    */
#define PIT_BCD      0b00000001 /* 4-digit BCD                      */

typedef struct
{
    uint32_t num_ints; /* Cantidad de interrupts generados por el PIT */
    uint32_t ints_sec; /* Cantidad de interrupts por segundo          */
} pit_info_t;




uint32_t pit_get_num_ints(void); /* Devuelve num_ints */
uint32_t pit_get_freq(void);     /* Devuelve ints_sec */

void     pit_set_phase(uint16_t count);
void     pit_install_handler(void);
uint16_t pit_get_count(void);
void     pit_diag(void);


#endif
