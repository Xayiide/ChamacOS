#ifndef _TASK_H_
#define _TASK_H_

#include <stdint.h>

#define TASK_NAME_LEN  10
#define TASK_MAX_TASKS 10

#define TASK_PRIO_MAX 5
#define TASK_PRIO_MIN 1

#define TASK_ID_KTASK  0

typedef struct
{
    uint32_t eax, ebx, ecx, edx;
    uint32_t esi, edi, ebp, esp, eip;
} __attribute__((packed)) proc_regs; /* FIXME: ¿Necesito el packed? */

typedef enum
{
    UNUSED,
    BLOCKED,
    READY,
    RUNNING,
    DED
} task_state;

typedef struct
{
    uint32_t   *stack_top;
    task_state  state;
    uint8_t     prio;
    uint8_t     id;
    char        name[TASK_NAME_LEN + 1];
} __attribute__((packed)) task_t;

char *task_state_str(task_state st);
void  task_diag(task_t task);

/*
 * task_create()
 * task_delete()
 * init_sched() ?
 */


#endif
