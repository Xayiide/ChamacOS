#ifndef _PIT_H_
#define _PIT_H_

#include <stdint.h> /* uint_t */

//#define PIT_CH0_DATA 0x40
//#define PIT_CH1_DATA 0x41
//#define PIT_CH2_DATA 0x42
//#define PIT_CMD      0x43 /* solo escritura */

#define PIT_OSC_FQ   1193182 /* Frecuencia del oscilador del PIT */

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


void pit_set_phase(uint32_t count);
void pit_install_handler();

#endif
