#include <stdio.h>

#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/ssp.h>
#include <kernel/tty.h>

void init_systems(void)
{
	terminal_init();
	gdt_init();
	idt_init();
}

void kernel_main(void)
{
	init_systems();

	volatile int x;
	x = 1;
	x--;
	volatile int y;
	y = 3 / x;
	(void)y;

	for (;;) {
		printf("heloooo new lined me -> ");
		printf("its me again\n");
		printf("1\n");
		printf("2\n");
		printf("3\n");
	}
}
