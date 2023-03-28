#include <stdint.h> /* uint_t */
#include <stdarg.h> /* va_arg */

#include "vga.h"
#include "sys.h"    /* memset */

static uint16_t *vga_mem = 0;
static uint16_t  vga_x   = 0;
static uint16_t  vga_y   = 0;
static uint32_t  vga_col = VGA_BLACK_WHITE;

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
        vga_mem[(vga_y * VGA_W + vga_x)] = (vga_col << 8) | c;
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

    blank = 0x20 | (vga_col << 8);

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

    blank = 0x20 | (vga_col << 8);

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

void vga_color(uint8_t back, uint8_t fore)
{
    vga_col = (back << 4) | (fore & VGA_BLACK_WHITE);
}

/* TODO sacar a otro fichero (stdio.h) */
void printk(const char *format, ...)
{
    const char *curr = format;
    va_list     arg;

    va_start(arg, format);

    while (*curr != '\0')
    {
        if (*curr == '%')
        {
            curr++; /* Avanzamos al caracter siguiente */
            switch (*curr)
            {
            case 'c': {
                int c = va_arg(arg, int);
                vga_puts((const char *) &c);
                break;
            }
            case 'd': {
                int d = va_arg(arg, int);
                if (d < 0)
                {
                    d = -d;
                    vga_puts("-\0");
                }
                vga_puts(changebase((uint32_t) d, BASE_10));
                break;
            }
            case 's': {
                char *s = va_arg(arg, char *);
                vga_puts(s);
                break;
            }
            case 'x': {
                int x = va_arg(arg, uint32_t);
                vga_puts(changebase(x, BASE_16));
                break;
            }
            default:
                break;
            }
        }
        else
        {
            vga_putchar(*curr);
        }
        curr++;
    }
    va_end(arg);
}