#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void)
{
	terminal_initialize();

	for (;;) {
		printf("helooooo new lined me -> ");
		printf("new line \n inbetween -> ");
		printf("its me again\n");
	}
}
