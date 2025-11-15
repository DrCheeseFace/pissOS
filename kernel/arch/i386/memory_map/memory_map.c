#include <kernel/memory_map.h>
#include <kernel/misc.h>
#include <kernel/paging.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

global_variable uint32_t (*page_directory)[PAGE_DIRECTORY_LEN];
// uint32_t page_directory[1024]

void memory_map_init(uint32_t magic, multiboot_info_t *mbd)
{
#ifdef DEBUG_LOGGING
	printf("init memory map\n");
#endif
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		abort("ERROR: invalid memory map given by grub bootloader");
	}

	if (!(mbd->flags >> 6 & 0x1)) {
		abort("ERROR: invalid memory map given by grub bootloader");
	}

	page_frames_init(mbd);

	page_frame_t page_directory_frame = kalloc_frame();
	page_directory = (void *)page_directory_frame;

	page_frame_t page_table_frame = kalloc_frame();
	uint32_t *first_page_table = (uint32_t *)page_table_frame;

	for (uint16_t i = 0; i < PAGE_DIRECTORY_LEN; i++) {
		((uint32_t *)page_directory)[i] = PAGE_DIRECTORY_ENTRY_INIT;
	}
	for (uint16_t i = 0; i < 1024; i++) {
		first_page_table[i] = (i * 0x1000) | 3;
	}

	// wtf figure out how this is working
	((uint32_t *)page_directory)[0] = page_table_frame | 3;

	loadPageDirectory((uint32_t)page_directory);
	enablePaging();

#ifdef DEBUG_LOGGING
	printf("init memory map OK\n");
#endif
}
