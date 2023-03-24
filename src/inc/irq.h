#ifndef _IRQ_H_
#define _IRQ_H_

#include "system.h" /*regs_t */

void irq_init(void);
void irq_fault_handler(regs_t *r);

#endif