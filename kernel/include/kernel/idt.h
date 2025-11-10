#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <kernel/utils.h>
#include <stdint.h>

#define IDT_MAX_DESCRIPTORS 256

typedef struct {
	uint16_t base_low; // The lower 16 bits of the ISR's address
	uint16_t sel; // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t always0;
	uint8_t flags;
	uint16_t base_high; // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idtr_t;

void idt_init(void);

void idt_gate_set(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

void isr_handler(struct InterruptRegisters *regs);

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

extern void isr128(void);
extern void isr177(void);

#endif // ! _KERNEL_IDT_H
