#include <stdint.h>
#include <string.h>

void *memchr(const void *s, int c, size_t n)
{
	const unsigned char *p = (const unsigned char *)s;
	unsigned char uc = (unsigned char)c;

	while (n-- > 0) {
		if (*p == uc) {
			return (void *)p;
		}
		p++;
	}

	return NULL;
}
