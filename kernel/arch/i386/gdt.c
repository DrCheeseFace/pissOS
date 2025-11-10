#include <kernel/gdt.h>

extern void gdt_flush(void *);

struct GdtEntry gdt_entries[5];
struct GdtPtr gdt_ptr;

void gdt_init(void)
{
	gdt_ptr.limit = (sizeof(struct GdtEntry) * 5) - 1;
	gdt_ptr.base = (unsigned int)&gdt_entries;

	gdt_gate_set(0, 0, 0, 0, 0); // Null Descriptor

	gdt_gate_set(1, 0, 0XFFFFFFFF, 0x9A, 0xCF); // kernel mode code segment

	gdt_gate_set(2, 0, 0XFFFFFFFF, 0x92, 0xCF); // kernel mode data segment

	gdt_gate_set(3, 0, 0XFFFFFFFF, 0xFA, 0xCF); // user mode code segment

	gdt_gate_set(4, 0, 0XFFFFFFFF, 0xF2, 0xCF); // user mode data segment

	gdt_flush(&gdt_ptr);
}

void gdt_gate_set(uint32_t num, uint32_t base, uint32_t limit, uint8_t access,
		  uint8_t gran)
{
	gdt_entries[num].access = access;

	gdt_entries[num].base_low = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_low = (base >> 24) & 0xFF;

	gdt_entries[num].limit = (limit & 0xFFFF);
	gdt_entries[num].flags = (limit >> 16) & 0x0F;
	gdt_entries[num].flags |= (gran & 0xF0);
}
