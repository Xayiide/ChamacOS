#ifndef _TASK_H_
#define _TASK_H_

#include <stdint.h>

typedef struct
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* PUSHAD */
    uint32_t eflags; /* PUSHFD */
    uint32_t eip;
} __attribute__((packed)) Registers;

typedef struct
{
    Registers regs;
    struct Task *next;
} Task;

void task_init(void);
void task_create(Task *, void(*)(), uint32_t);
void task_yield(void);
void task_start_ChamacOS(void);

extern void task_switch(Registers *old, Registers *new); /* boot.asm */

#endif
