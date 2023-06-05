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

void initTasking(void);
void createTask(Task *, void(*)(), uint32_t);
void yield(void);
void startChamacOS(void);

extern void switchTask(Registers *old, Registers *new); /* boot.asm */

#endif
