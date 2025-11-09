#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void)
{
	terminal_initialize();

	for (;;) {
		printf("helooooo\n new lined me -> ");
		printf("again\n");
		printf("43\n");
	}
}
