#include <kernel/handlers.h>
#include <kernel/idt.h>
#include <kernel/utils.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static idt_entry_t idt_entries[IDT_MAX_DESCRIPTORS];
static idtr_t idt_ptr;
void (*irq_routines[16])(struct InterruptRegisters *) = { 0 };

extern void idt_flush(uint32_t);

const char *exception_messages[] = { "division by zero",
				     "debug",
				     "non maskable interrupt",
				     "breakpoint",
				     "into detected overflow",
				     "out of bounds",
				     "invalid opcode",
				     "no coprocessor",
				     "double fault",
				     "coprocessor segment overrun",
				     "bad tss",
				     "segment not present",
				     "stack fault",
				     "general protection fault",
				     "page fault",
				     "unknown interrupt",
				     "coprocessor fault",
				     "alignment fault",
				     "machine check",
				     "reserved",
				     "reserved",
				     "reserved",
				     "reserved",
				     "reserved",
				     "reserved",
				     "reserved",
				     "reserved",
				     "reserved",
				     "reserved",
				     "reserved",
				     "reserved",
				     "reserved" };

void irq_handlers_init(void);

static void idt_gate_set(uint8_t num, uint32_t base, uint16_t sel,
			 uint8_t flags)
{
	idt_entries[num].base_low = base & 0xFFFF;
	idt_entries[num].base_high = (base >> 16) & 0xFFFF;
	idt_entries[num].sel = sel;
	idt_entries[num].always0 = 0;
	idt_entries[num].flags = flags | 0x60;
}

void idt_init(void)
{
	idt_ptr.limit = sizeof(idt_entries) - 1;
	idt_ptr.base = (uint32_t)&idt_entries;

	memset(idt_entries, 0, sizeof(idt_entries));

	// initialize PICS
	outPortB(PIC1, 0x11);
	outPortB(PIC2, 0x11);

	outPortB(PIC1_DATA, 0x20);
	outPortB(PIC2_DATA, 0x28);

	outPortB(PIC1_DATA, 0x04);
	outPortB(PIC2_DATA, 0x02);

	outPortB(PIC1_DATA, 0x01);
	outPortB(PIC2_DATA, 0x01);

	outPortB(PIC1_DATA, 0x0);
	outPortB(PIC2_DATA, 0x0);

	idt_gate_set(0, (uint32_t)isr0, 0x08, 0x8E);
	idt_gate_set(1, (uint32_t)isr1, 0x08, 0x8E);
	idt_gate_set(2, (uint32_t)isr2, 0x08, 0x8E);
	idt_gate_set(3, (uint32_t)isr3, 0x08, 0x8E);
	idt_gate_set(4, (uint32_t)isr4, 0x08, 0x8E);
	idt_gate_set(5, (uint32_t)isr5, 0x08, 0x8E);
	idt_gate_set(6, (uint32_t)isr6, 0x08, 0x8E);
	idt_gate_set(7, (uint32_t)isr7, 0x08, 0x8E);
	idt_gate_set(8, (uint32_t)isr8, 0x08, 0x8E);
	idt_gate_set(9, (uint32_t)isr9, 0x08, 0x8E);
	idt_gate_set(10, (uint32_t)isr10, 0x08, 0x8E);
	idt_gate_set(11, (uint32_t)isr11, 0x08, 0x8E);
	idt_gate_set(12, (uint32_t)isr12, 0x08, 0x8E);
	idt_gate_set(13, (uint32_t)isr13, 0x08, 0x8E);
	idt_gate_set(14, (uint32_t)isr14, 0x08, 0x8E);
	idt_gate_set(15, (uint32_t)isr15, 0x08, 0x8E);
	idt_gate_set(16, (uint32_t)isr16, 0x08, 0x8E);
	idt_gate_set(17, (uint32_t)isr17, 0x08, 0x8E);
	idt_gate_set(18, (uint32_t)isr18, 0x08, 0x8E);
	idt_gate_set(19, (uint32_t)isr19, 0x08, 0x8E);
	idt_gate_set(20, (uint32_t)isr20, 0x08, 0x8E);
	idt_gate_set(21, (uint32_t)isr21, 0x08, 0x8E);
	idt_gate_set(22, (uint32_t)isr22, 0x08, 0x8E);
	idt_gate_set(23, (uint32_t)isr23, 0x08, 0x8E);
	idt_gate_set(24, (uint32_t)isr24, 0x08, 0x8E);
	idt_gate_set(25, (uint32_t)isr25, 0x08, 0x8E);
	idt_gate_set(26, (uint32_t)isr26, 0x08, 0x8E);
	idt_gate_set(27, (uint32_t)isr27, 0x08, 0x8E);
	idt_gate_set(28, (uint32_t)isr28, 0x08, 0x8E);
	idt_gate_set(29, (uint32_t)isr29, 0x08, 0x8E);
	idt_gate_set(30, (uint32_t)isr30, 0x08, 0x8E);
	idt_gate_set(31, (uint32_t)isr31, 0x08, 0x8E);

	// interrupt requests
	idt_gate_set(32, (uint32_t)irq0, 0x08, 0x8E);
	idt_gate_set(33, (uint32_t)irq1, 0x08, 0x8E);
	idt_gate_set(34, (uint32_t)irq2, 0x08, 0x8E);
	idt_gate_set(35, (uint32_t)irq3, 0x08, 0x8E);
	idt_gate_set(36, (uint32_t)irq4, 0x08, 0x8E);
	idt_gate_set(37, (uint32_t)irq5, 0x08, 0x8E);
	idt_gate_set(38, (uint32_t)irq6, 0x08, 0x8E);
	idt_gate_set(39, (uint32_t)irq7, 0x08, 0x8E);
	idt_gate_set(40, (uint32_t)irq8, 0x08, 0x8E);
	idt_gate_set(41, (uint32_t)irq9, 0x08, 0x8E);
	idt_gate_set(42, (uint32_t)irq10, 0x08, 0x8E);
	idt_gate_set(43, (uint32_t)irq11, 0x08, 0x8E);
	idt_gate_set(44, (uint32_t)irq12, 0x08, 0x8E);
	idt_gate_set(45, (uint32_t)irq13, 0x08, 0x8E);
	idt_gate_set(46, (uint32_t)irq14, 0x08, 0x8E);
	idt_gate_set(47, (uint32_t)irq15, 0x08, 0x8E);

	// system calls
	idt_gate_set(128, (uint32_t)isr128, 0x08, 0x8E);
	idt_gate_set(177, (uint32_t)isr177, 0x08, 0x8E);

	outPortB(0x43, 0x36);
	outPortB(0x40, 0x0);
	outPortB(0x40, 0x0);

	irq_handlers_init();

	idt_flush((uint32_t)&idt_ptr);

	printf("idt init OK\n");
}

void irq_handlers_init(void)
{
	irq_install_handler(0, temp_timer_handler);
}

void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *r))
{
	irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
	irq_routines[irq] = 0;
}

void isr_handler(struct InterruptRegisters *regs)
{
	if (regs->int_no < 32) {
		printf(exception_messages[regs->int_no]);
		printf("\n");
		printf("system halting\n");
		abort();
	}
}

void irq_handler(struct InterruptRegisters *regs)
{
	void (*handler)(struct InterruptRegisters *regs);
	handler = irq_routines[regs->int_no - 32];

	if (handler) {
		handler(regs);
	}

	if (regs->int_no >= 40) {
		outPortB(PIC2, PIC_EOI);
	}

	outPortB(PIC1, PIC_EOI);
}
