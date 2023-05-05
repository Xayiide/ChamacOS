#include <stdint.h>      /* uint_t */

#include "task.h"
#include "lib/sys.h"     /* memset */
#include "drivers/vga.h" /* printk */


char *task_state_str(task_state st);

void task_diag(task_t task)
{
    printk("\tstack top: 0x%x\n", task.stack_top);
    printk("\tstate:     %s\n", task_state_str(task.state));
    printk("\tprio:      %d", task.prio);
    if (task.prio == TASK_PRIO_MAX)
        printk(" (MAX)\n");
    else if (task.prio == TASK_PRIO_MIN)
        printk(" (MIN)\n");
    else
        printk("\n");
    printk("\tid:        %d\n", task.id);
    printk("\tname:      %s\n", task.name);
}



char *task_state_str(task_state st)
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
