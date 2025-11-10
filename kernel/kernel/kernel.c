#include <stdio.h>

#include <kernel/ssp.h>
#include <kernel/tty.h>

void kernel_main(void)
{
	terminal_initialize();

	for (;;) {
		printf("heloooo new lined me -> ");
		printf("its me again\n");
	}
}
