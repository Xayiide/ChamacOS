#include "sched.h"
#include "task.h"
#include "lib/sys.h"     /* NULL */
#include "drivers/vga.h" /* printk */

static task_t  *current_task;
static task_t   tasks[4096];
static uint8_t  active_tasks;

static task_t   ready[TASK_MAX_TASKS];
static uint8_t  ready_num;


void sched_init(void)
{
    uint8_t i;
    char krnn[TASK_NAME_LEN] = "IDLE\0";
    char unus[TASK_NAME_LEN] = "UNUSEDX\0";
    kmem_info_t *kmem;

    kmem = sys_get_kmem_info();
    memset(&tasks, 0x00, sizeof(task_t) * TASK_MAX_TASKS);
    memset(&ready, 0x00, sizeof(task_t) * TASK_MAX_TASKS);
    for (i = 0; i < TASK_MAX_TASKS; i++)
    {
        tasks[i].stack_top = NULL;
        tasks[i].state     = UNUSED;
        tasks[i].prio      = 0;
        tasks[i].id        = i;
        unus[6] = i + 0x30; /* X -> i en ASCII */
        strncpy(tasks[i].name, unus, TASK_NAME_LEN);
    }

    active_tasks = 0;
    ready_num    = 0;
    current_task = NULL;

    tasks[0].stack_top = kmem->kstack_start;
    tasks[0].state     = RUNNING;
    tasks[0].prio      = 0;
    tasks[0].id        = 0;
    strncpy(tasks[0].name, krnn, TASK_NAME_LEN);
    /* TODO: 1. lista de procesos
     *       2. Hacer Idle Task
     *       3. Establecer tiempo de rodaja
     *       4. Configurar PIT para manejar una interrupción cada X milis
     */
}

void sched_yield(void)
{
    current_task->state = READY;

}

void sched_end(void)
{

}

uint8_t sched_get_my_id(void)
{
    return current_task->id;
}


void *sched_get_tasks_array(void)
{
    return (void *) tasks;
}

task_t *sched_get_task(uint8_t tid)
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


void sched_diag(void)
{
    uint8_t i;
    vga_color(VGA_BACK_BLACK, VGA_FORE_RED);
    printk(" === SCHED DIAGNOSIS ===\n");
    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    printk("tasks:   0x%x\n", &tasks);
    printk("active:  %d\n", active_tasks);
    printk("current:");
    if (current_task == NULL)
        printk(" None (NULL)\n");
    else
        printk(" 0x%x [0x%x]\n", current_task, &current_task);
#ifdef SCHED_DEBUG
    for (i = 0; i < TASK_MAX_TASKS; i++)
    {
        printk(" === [%d] ===\n", i);
        task_diag(tasks[i]);
        printk("\n");
    }
#else
    i = 0; i++; /* Para que el compilador no llore */
#endif
}
