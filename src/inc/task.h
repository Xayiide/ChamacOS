#ifndef _TASK_H_
#define _TASK_H_

#include <stdint.h>

#define TASK_NAME_LEN  10
#define TASK_MAX_TASKS 10

#define TASK_PRIO_MAX 5
#define TASK_PRIO_MIN 1

#define TASK_ID_KTASK 0

typedef enum
{
    BLOCKED,
    READY,
    RUNNING
} task_state;

typedef struct
{
    uint32_t   stack_top;
    task_state state;
    uint8_t    prio;
    uint8_t    id;
    char       name[TASK_NAME_LEN + 1];
} __attribute__((packed)) task_t;

void  task_init(void);
void *task_get_tasks_array(void);
void  task_diag(void);


#endif
