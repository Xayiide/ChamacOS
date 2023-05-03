#include <stdint.h>      /* uint_t */

#include "task.h"
#include "lib/sys.h"     /* memset */
#include "drivers/vga.h" /* printk */

static task_t tasks[TASK_MAX_TASKS];
static uint8_t active_tasks = 0;

static char *task_state_str(task_state st);

void task_init(void)
{
    uint8_t i;
    kmem_info_t *kmem;
    char name[] = "KERNEL\0";
    char unus[TASK_NAME_LEN] = "UNUSED_\0";
    memset(&tasks, 0x00, sizeof(task_t) * TASK_MAX_TASKS);

    /* Usamos la tarea 0 como la que sería tarea del kernel */
    kmem = sys_get_kmem_info();
    tasks[0].stack_top = kmem->kstack_start;
    tasks[0].state     = RUNNING;
    tasks[0].prio      = TASK_PRIO_MAX;
    tasks[0].id        = TASK_ID_KTASK;
    strncpy(tasks[0].name, name, TASK_NAME_LEN);

    active_tasks = 1;

    for (i = 1; i < TASK_MAX_TASKS; i++)
    {
        tasks[i].stack_top = NULL;
        tasks[i].state     = UNUSED;
        tasks[i].prio      = 0;
        tasks[i].id        = i;
        unus[6] = i + 0x30; /* ASCII */
        strncpy(tasks[i].name, unus, TASK_NAME_LEN);
    }
}

void *task_get_tasks_array(void)
{
    return (void *) tasks;
}

task_t *task_get(uint8_t tid)
{
    task_t *t = NULL;
    if (tid >= TASK_MAX_TASKS)
        t = NULL;
    else if (tasks[tid].state == UNUSED || tasks[tid].state == DED)
        t = NULL;
    else
        t = &tasks[tid];

    return t;
}

void task_diag(void)
{
    uint8_t i;
    vga_color(VGA_BACK_BLACK, VGA_FORE_RED);
    printk(" === TASK DIAGNOSIS ===\n");
    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    printk("tasks: 0x%x\n", &tasks);
    for (i = 0; i < active_tasks; i++)
    {
        printk("\t[%d] stack top: 0x%x\n", i, tasks[i].stack_top);
        printk("\t    state:     %s\n", task_state_str(tasks[i].state));
        printk("\t    prio:      %d", tasks[i].prio);
        if (tasks[i].prio == TASK_PRIO_MAX)
            printk(" (MAX)\n");
        else if (tasks[i].prio == TASK_PRIO_MIN)
            printk(" (MIN)\n");
        else
            printk("\n");
        printk("\t    id:        %d\n", tasks[i].id);
        printk("\t    name:      %s\n", tasks[i].name);
    }

}



static char *task_state_str(task_state st)
{
    char *str;
    switch(st)
    {
    case UNUSED:
        str = "Unused";
        break;
    case BLOCKED:
        str = "Blocked";
        break;
    case READY:
        str = "Ready";
        break;
    case RUNNING:
        str = "Running";
        break;
    case DED:
        str = "Ded";
        break;
    default:
        panic("task_state_str: Reached default");
        break;
    }

    return str;
}
