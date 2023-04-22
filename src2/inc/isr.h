#ifndef _ISR_H_
#define _ISR_H_

#include "sys.h" /* regs_t */

void isr_init();
void isr_fault_handler(regs_t *r);

#endif
