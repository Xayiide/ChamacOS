#ifndef _PIC_H_
#define _PIC_H_

#define PIC1_OFFSET   0x20 /* 32 decimal. 0-31 son las interrupts de intel */
#define PIC2_OFFSET   PIC1_OFFSET + 8

//#define PIC1          0x20
//#define PIC1_CMD      PIC1
//#define PIC1_DAT      0x21

//#define PIC2          0xA0
//#define PIC2_CMD      PIC2
//#define PIC2_DAT      0xA1

#define PIC_ICW1_INIT 0x11
#define PIC_ICW4_8086 0x01 /* Modo 8086/88         */
#define PIC_ICW4_AUTO 0x02 /* Modo auto EOI        */
#define PIC_ICW4_BUFS 0x08 /* Buffered mode slave  */
#define PIC_ICW4_BUFM 0x0C /* Buffered mode master */
#define PIC_ICW4_SFN  0x10 /* Special Fully Nested */

#define PIC_EOI       0x20

void pic_remap(void);

#endif
