#ifndef _VGA_H_
#define _VGA_H_

#include <stdint.h> /* uint_t */

#define VGA_ADDR 0xB8000
#define VGA_W    80
#define VGA_H    25

#define VGA_BLACK_WHITE 0x0F /* Fondo negro letras blancas */
#define VGA_WHITE_BLACK 0xF0

#define VGA_BACK_BLACK  0x00

#define VGA_FORE_BLUE   0x01
#define VGA_FORE_GREEN  0x02
#define VGA_FORE_CYAN   0x03
#define VGA_FORE_RED    0x04
#define VGA_FORE_MAGEN  0x05
#define VGA_FORE_BROWN  0x06
#define VGA_FORE_WHITE  0x0F

#define BYTE_TO_BIN_F "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BIN(byte)      \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')




//#define VGA_IO_CTRL 0x3D4
//#define VGA_IO_DATA 0x3D5

void vga_init(void);
void vga_putchar(uint8_t c); /* static? */
void vga_clear(void);
void vga_scroll(void);
void vga_puts(const char *str);
void vga_move_csr();
void vga_color(uint8_t back, uint8_t fore);
void vga_diag(void);

void printk(const char *format, ...);

#endif
