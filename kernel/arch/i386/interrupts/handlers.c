#include <kernel/tty.h>
#include <kernel/utils.h>
#include <stdio.h>

void temp_timer_handler(struct InterruptRegisters *regs)
{
	(void)regs;
	printf("temp_timer_handler_called");
}
