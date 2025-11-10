#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__)) void abort(void)
{
#if defined(__is_libk)

	printf("kernel: panic: abort()\n");

	__asm__ volatile("cli");

	while (1) {
		__asm__ volatile("hlt");
	}

#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort()\n");
	while (1) {
	}
#endif

	__builtin_unreachable();
}
