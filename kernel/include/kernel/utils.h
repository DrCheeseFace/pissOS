#ifndef _KERNEL_UTILS_H
#define _KERNEL_UTILS_H

#include <stdint.h>

struct InterruptRegisters {
	uint32_t cr2;
	uint32_t ds;
	uint32_t edi, esi, ibp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, csm, eflags, useresp, ss;
};

#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20

void outPortB(uint16_t port, uint8_t value);

#endif
