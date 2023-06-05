#include <stdint.h>
#include "task.h"

#include "mm/pmm.h"
#include "drivers/vga.h"

static Task *runningTask;
static Task task1_TCB;
static Task task2_TCB;

static void task_diag(Task *t);


static void task1(void)
{
    Task *last;

    while (1) {
        vga_color(VGA_BACK_BLACK, VGA_FORE_CYAN);
        last = runningTask;
        printk("[TASK 1] Switching to task 2...\n");
        runningTask = (Task *) runningTask->next;
        vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
        switchTask(&last->regs, &runningTask->regs);
        vga_color(VGA_BACK_BLACK, VGA_FORE_CYAN);
        printk("[TASK 1] Returned to task 1\n");
        vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    }
}

static void task2(void)
{
    Task *last;

    while (1) {
        vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);

        last = runningTask;
        printk("[TASK 2] Multitasking world!\n");
        runningTask = (Task *) runningTask->next;
        switchTask(&last->regs, &runningTask->regs);
        printk("[TASK 2] Returned to task 2...\n");

        vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    }
}

void initTasking()
{
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(task1_TCB.regs.eflags)::"%eax");

    createTask(&task1_TCB, task1, task1_TCB.regs.eflags);
    createTask(&task2_TCB, task2, task1_TCB.regs.eflags);
    task1_TCB.next = (struct Task *) &task2_TCB;
    task2_TCB.next = (struct Task *) &task1_TCB;

    runningTask = &task1_TCB;
}

void startChamacOS()
{
    task1();
}

void createTask(Task *task, void (*main)(), uint32_t flags)
{
    static uint8_t num = 0;

    if (num == 0) {
        task->regs.edi    = 0x11111ED1;
        task->regs.esi    = 0x11111E51;
        /* Aquí iría EBP */
        task->regs.esp    = (uint32_t) pmm_alloc_frame() /*+ 0x1000*/;
        task->regs.ebx    = 0x11111EBB;
        task->regs.edx    = 0x11111EDD;
        task->regs.ecx    = 0x11111ECC;
        task->regs.eax    = 0x11111EAA;
        task->regs.eflags = flags;
        task->regs.eip    = (uint32_t) main;
        task->next        = 0;
        num++;
    }
    else
    {
        task->regs.edi    = 0x22222ED1;
        task->regs.esi    = 0x22222E51;
        /* Aquí iría EBP */
        task->regs.esp    = (uint32_t) pmm_alloc_frame() /*+ 0x1000*/;
        task->regs.ebx    = 0x22222EBB;
        task->regs.edx    = 0x22222EDD;
        task->regs.ecx    = 0x22222ECC;
        task->regs.eax    = 0x22222EAA;
        task->regs.eflags = flags;
        task->regs.eip    = (uint32_t) main;
        task->next        = 0;
    }
}

void yield()
{
    Task *last = runningTask;
    runningTask = (Task *) runningTask->next;
    task_diag(last);
    switchTask(&last->regs, &runningTask->regs);
    task_diag(last);
}

static void task_diag(Task *t)
{
    vga_color(VGA_BACK_BLACK, VGA_FORE_RED);
    printk("Task @ %x || ", t);
    printk("Regs @ %x || ", &t->regs);
    printk("Next @ %x -> 0x%x\n", &t->next, t->next);
    vga_color(VGA_BACK_BLACK, VGA_FORE_WHITE);
    printk("[EDI @ %x]   0x%x\n", &t->regs.edi,    t->regs.edi);
    printk("[ESI @ %x]   0x%x\n", &t->regs.esi,    t->regs.esi);
    printk("[EBP @ %x]   0x%x\n", &t->regs.ebp,    t->regs.ebp);
    printk("[ESP @ %x]   0x%x\n", &t->regs.esp,    t->regs.esp);
    printk("[EBX @ %x]   0x%x\n", &t->regs.ebx,    t->regs.ebx);
    printk("[EDX @ %x]   0x%x\n", &t->regs.edx,    t->regs.edx);
    printk("[ECX @ %x]   0x%x\n", &t->regs.ecx,    t->regs.ecx);
    printk("[EAX @ %x]   0x%x\n", &t->regs.eax,    t->regs.eax);
    printk("[EFL @ %x]   0x%x\n", &t->regs.eflags, t->regs.eflags);
    printk("[EIP @ %x]   0x%x\n", &t->regs.eip,    t->regs.eip);
}
