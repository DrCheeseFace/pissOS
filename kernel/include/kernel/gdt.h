#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <stdint.h>

struct GdtEntry {
	uint16_t limit;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t flags;
	uint8_t base_high;
} __attribute__((packed));

struct GdtPtr {
	uint16_t limit;
	unsigned int base;
} __attribute__((packed));

void gdt_init(void);

void gdt_gate_set(uint32_t num, uint32_t base, uint32_t limit, uint8_t access,
		  uint8_t gran);

#endif // !
