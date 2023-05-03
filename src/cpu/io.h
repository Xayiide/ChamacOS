#ifndef _IO_H_
#define _IO_H_

#include <stdint.h> /* uint_t */

uint8_t inb(uint16_t port);
void    outb(uint16_t port, uint8_t val);

/* PIC */
#define IO_PIC1_CMD 0x20
#define IO_PIC1_DAT 0x21
#define IO_PIC2_CMD 0xA0
#define IO_PIC2_DAT 0xA1

/* PIT */
#define IO_PIT_CH0_DAT 0x40
#define IO_PIT_CH1_DAT 0x41
#define IO_PIT_CH2_DAT 0x42
#define IO_PIT_CMD     0x43

/* VGA */
#define IO_VGA_CTR 0x3D4
#define IO_VGA_DAT 0x3D5

/* TECLADO */
#define IO_KB_DAT 0x60
#define IO_KB_STA 0x64

#endif