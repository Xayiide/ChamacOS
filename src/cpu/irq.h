#ifndef _IRQ_H_
#define _IRQ_H_

#include "lib/sys.h" /*regs_t */

#define IRQ_NUM_INT 16

void irq_init(void);
void irq_fault_handler(regs_t *r);
void irq_install_handler(uint8_t irq, void (*handler)(regs_t *r));
void irq_uninstall_handler(uint8_t irq);

#endif
