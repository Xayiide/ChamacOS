#include <stdint.h> /* uint_t */

#include "vga.h"
#include "system.h"

static uint16_t *vga_mem = 0;
static uint16_t  vga_x   = 0;
static uint16_t  vga_y   = 0;

void vga_init()
{
    vga_mem = (uint16_t *) (VGA_ADDR);
    vga_clear();
}

void vga_putchar(uint8_t c)
{
    if (c == '\n')
    {
        vga_x = 0;
        vga_y++;
    }
    else if (c >= ' ')
    {
        vga_mem[(vga_y * VGA_W + vga_x)] = (VGA_BLACK_WHITE << 8) | c;
        vga_x++;
    }

    if (vga_x >= VGA_W)
    {
        vga_y++;
        vga_x = 0;
    }

    vga_scroll();
    vga_move_csr();
}

void vga_clear()
{
    uint8_t x, y;
    uint16_t blank;

    blank = 0x20 | (VGA_BLACK_WHITE << 8);

    for (y = 0; y < VGA_H; y++)
        for (x = 0; x < VGA_W; x++)
            vga_mem[(y * VGA_W + x)] = blank;

    vga_x = 0;
    vga_y = 0;
    vga_move_csr();
}

void vga_scroll()
{
    uint16_t blank;
    uint16_t temp;

    blank = 0x20 | (VGA_BLACK_WHITE << 8);

    if (vga_y >= VGA_H)
    {
        /* mueve una linea para atras */
        temp = vga_y - VGA_H + 1;
        memcpy(vga_mem, vga_mem + temp * VGA_W, (VGA_H - temp) * VGA_W * 2);

        memsetw(vga_mem + (VGA_H - temp) * VGA_W, blank, VGA_W);

        vga_y = VGA_H - 1;
    }
}

void vga_puts(const char *str)
{
    int    i;
    size_t len;

    len = strlen(str);
    for (i = 0; i < len; i++)
        vga_putchar(str[i]);
}

/* http://www.brackeen.com/home/vga */
void vga_move_csr()
{
    uint16_t temp;

    temp = vga_y * VGA_W + vga_x;

    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}
