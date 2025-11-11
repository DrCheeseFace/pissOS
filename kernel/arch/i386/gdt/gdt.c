#include <kernel/gdt.h>

#include <stdio.h>
#include <string.h>

extern void gdt_flush(uint32_t);
extern void tss_flush(uint32_t);

static gdt_entry_t gdt_entries[6];
static gdtr_t gdt_ptr;
struct TssEntry tss_entry;

void gdt_init(void)
{
	gdt_ptr.limit = sizeof(gdt_entries) - 1;
	gdt_ptr.base = (uint32_t)&gdt_entries;

	gdt_gate_set(0, 0, 0, 0, 0); // Null Descriptor

	gdt_gate_set(1, 0, 0XFFFFFFFF, 0x9A, 0xCF); // kernel mode code segment

	gdt_gate_set(2, 0, 0XFFFFFFFF, 0x92, 0xCF); // kernel mode data segment

	gdt_gate_set(3, 0, 0XFFFFFFFF, 0xFA, 0xCF); // user mode code segment

	gdt_gate_set(4, 0, 0XFFFFFFFF, 0xF2, 0xCF); // user mode data segment

	tss_write(5, 0x10, 0x0); // task management segment

	gdt_flush((uint32_t)&gdt_ptr);

	tss_flush((uint32_t)&tss_entry);

	printf("gdt init OK\n");
}

void gdt_gate_set(uint32_t num, uint32_t base, uint32_t limit, uint8_t access,
		  uint8_t gran)
{
	gdt_entries[num].access = access;

	gdt_entries[num].base_low = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high = (base >> 24) & 0xFF;

	gdt_entries[num].limit = (limit & 0xFFFF);
	gdt_entries[num].flags = (limit >> 16) & 0x0F;
	gdt_entries[num].flags |= (gran & 0xF0);
}

void tss_write(uint32_t num, uint16_t ss0, uint32_t esp0)
{
	uint32_t base = (uint32_t)&tss_entry;
	uint32_t limit = sizeof(tss_entry) - 1;

	gdt_gate_set(num, base, limit, 0xE9, 0x0);

	memset(&tss_entry, 0, sizeof(tss_entry));

	tss_entry.ss0 = ss0;
	tss_entry.esp0 = esp0;

	tss_entry.cs = GDT_OFFSET_KERNEL_CODE | 0x3;
	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs =
		tss_entry.gs = 0x10 | 0x3;
}
