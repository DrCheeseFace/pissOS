#include <kernel/utils.h>

void outb(uint16_t port, uint8_t value)
{
	__asm__ volatile("outb %1, %0" : : "dN"(port), "a"(value));
}

uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
	return ret;
}
