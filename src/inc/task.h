#ifndef _TASK_H_
#define _TASK_H_

#include <stdint.h>

#define TASK_NAME_LEN  10
#define TASK_MAX_TASKS 10

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
} task_t;

void  task_init(void);
void *task_get_tasks_array(void);
void  task_diag(void);


#endif
