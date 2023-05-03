#ifndef _ISR_H_
#define _ISR_H_

#include "lib/sys.h" /* regs_t */

void isr_init(void);
void isr_fault_handler(regs_t *r);
void isr_diag(void);

#endif
