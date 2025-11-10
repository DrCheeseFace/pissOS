#include <stdio.h>

#include <kernel/gdt.h>
#include <kernel/ssp.h>
#include <kernel/tty.h>

void kernel_main(void)
{
	terminal_init();
	gdt_init();

	for (;;) {
		printf("heloooo new lined me -> ");
		printf("its me again\n");
		printf("1\n");
		printf("2\n");
		printf("3\n");
	}
}
