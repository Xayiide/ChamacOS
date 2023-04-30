#include <stdint.h> /* uint_t */

#include "task.h"
#include "sys.h"    /* memset */
#include "vga.h"    /* printk */

static task_t tasks[TASK_MAX_TASKS];

void task_init(void)
{
    memset(&tasks, 0x00, sizeof(task_t) * TASK_MAX_TASKS);
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
