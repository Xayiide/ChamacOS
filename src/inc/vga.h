#ifndef _VGA_H_
#define _VGA_H_

#define VGA_ADDR 0xB8000
#define VGA_W    80
#define VGA_H    25

#define VGA_BLACK_WHITE 0x0F /* Fondo negro letras blancas */
#define VGA_WHITE_BLACK 0xF0

void vga_init();
void vga_putchar(uint8_t c); /* static? */
void vga_clear();
void vga_scroll(); /* static? */
void vga_puts(const char *str);
void vga_move_csr();

#endif
