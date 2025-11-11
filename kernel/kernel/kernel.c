#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/keyboard.h>
#include <kernel/ssp.h>
#include <kernel/timer.h>
#include <kernel/tty.h>

void init_systems(void)
{
	tty_init();
	gdt_init();
	idt_init();
	timer_init();
	keyboard_init();
}

void kernel_main(void)
{
	init_systems();

	for (;;)
		;
}
