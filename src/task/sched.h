#ifndef _SCHED_H_
#define _SCHED_H_

#include "task.h"

void    sched_init(void);
void    sched_yield(void);
void    sched_end(void);
uint8_t sched_get_my_id(void);

void *sched_get_tasks_array(void);
task_t *sched_get_task(uint8_t tid);

void sched_diag(void);

#endif
