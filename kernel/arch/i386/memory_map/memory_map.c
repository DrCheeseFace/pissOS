#include <kernel/memory_map.h>
#include <kernel/misc.h>
#include <kernel/paging.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void memory_map_init(uint32_t magic, multiboot_info_t *mbd)
{
#ifdef DEBUG_LOGGING
	printf("initting memory map\n");
#endif
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		abort("ERROR: invalid memory map given by grub bootloader");
	}

	if (!(mbd->flags >> 6 & 0x1)) {
		abort("ERROR: invalid memory map given by grub bootloader");
	}

	page_frames_init(mbd);

#ifdef DEBUG_LOGGING
	printf("initting memory map OK\n");
#endif
}
