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

void outb(uint16_t port, uint8_t value);

uint8_t inb(uint16_t port);

#endif
