#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <stddef.h> /* size_t */
#include <stdint.h> /* uint_t */

#ifndef NULL
    #define NULL ((void *) 0)
#endif

/* TODO: mover a math.h? */
typedef enum
{
    BASE_10 = 10,
    BASE_16 = 16
} base_t;

/* gs es lo último en ser apilado.
 * Lo primero que apilamos nosotros es err_code e int_no.
 * Antes de eso, la CPU apila primero ss, old_esp, eflags, cs y eip. */
typedef struct
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* pushad */
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, old_esp, ss;
} __attribute__((packed)) regs_t;

typedef struct
{
    uint32_t *kstack_start, *kstack_end;
    char     *krn_start,    *krn_end;
    char     *text_start,   *text_end;
    char     *bss_start,    *bss_end;
} __attribute__((packed)) kmem_info_t;

void         sys_fillinfo(void);
void         sys_diag(void);
kmem_info_t *sys_get_kmem_info(void);

/* main.c (futuro mem) */
void   *memcpy (void *dest, const void *src, size_t n);
void   *memset (void *dest, uint8_t byte, size_t n);
void   *memsetw(void *dest, uint16_t val, size_t n);
size_t  strlen (const char *s);
char   *strcpy(char *dest, const char *src);
char   *strncpy(char *dest, const char *src, size_t n);

/* estadísticas del sistema */
void     sys_uptime_add_sec();
uint32_t sys_uptime();

/* activar y desactivar interrupciones */
void    sti(); /* activar    */
void    cli(); /* desactivar */

/* misceláneo -- FIXME: mover a math.h? */
char *changebase(uint32_t num, base_t base);

__attribute__((noreturn)) void panic(const char *str);

#endif
