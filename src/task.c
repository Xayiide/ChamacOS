#include <stdint.h> /* uint_t */

#include "task.h"
#include "sys.h"    /* memset */
#include "vga.h"    /* printk */

static task_t tasks[TASK_MAX_TASKS];

void task_init(void)
{
    kmem_info_t *kmem;
    char name[10] = "KERNEL\0";
    memset(&tasks, 0x00, sizeof(task_t) * TASK_MAX_TASKS);

    /* Usamos la tarea 0 como la que sería tarea del kernel */
    kmem = sys_get_kmem_info();
    tasks[0].stack_top = kmem->kstack_start;
    tasks[0].state     = RUNNING;
    tasks[0].prio      = TASK_PRIO_MAX;
    tasks[0].id        = TASK_ID_KTASK;
    strncpy(tasks[0].name, name, 10);
}

void *task_get_tasks_array(void)
{
    return (void *) tasks;
}

void task_diag(void)
{
    vga_color(VGA_BACK_BLACK, VGA_FORE_RED);
    printk(" === TASK DIAGNOSIS ===\n");
    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    printk("tasks: 0x%x\n", &tasks);
}
