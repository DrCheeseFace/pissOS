#include <kernel/memory_map.h>
#include <kernel/misc.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

global_variable page_frame_t page_frames[PAGE_FRAME_COUNT];
global_variable size_t page_frames_len = 0;

internal uintptr_t get_first_aligned_page_frame_addr(
	multiboot_info_t *mbd, mmap_entry_t **entry_with_frame_addr)
{
	mmap_entry_t *entry = (mmap_entry_t *)mbd->mmap_addr;
	uintptr_t unaligned_first_available_addr = 0x0;
	uint32_t kernel_end_addr = (uint32_t)endkernel;

	while ((uint32_t)entry < mbd->mmap_addr + mbd->mmap_length) {
		if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
			uintptr_t region_start = (uintptr_t)entry->addr_low;
			uintptr_t region_end =
				region_start + (uintptr_t)entry->len_low;

			if (region_start <= kernel_end_addr &&
			    kernel_end_addr < region_end) {
				unaligned_first_available_addr =
					kernel_end_addr;
			}

			if (region_start > kernel_end_addr) {
				unaligned_first_available_addr = region_start;
			}

			if (unaligned_first_available_addr != 0x0) {
				// aligning to 4096
				uintptr_t aligned_addr =
					(unaligned_first_available_addr +
					 PAGE_SIZE - 1) &
					~(PAGE_SIZE - 1);

				if (aligned_addr <
				    entry->addr_low + entry->len_low) {
					*entry_with_frame_addr = entry;
					return aligned_addr;
				}

				// gets to this line if the aligned address is outside of the available region
			}
		}

		entry = (mmap_entry_t *)((unsigned int)entry + entry->size +
					 sizeof(entry->size));
	}

#ifdef DEBUG_LOGGING
	printf("WARNING: failed to retrieve first available address after kernel code\n");
#endif
	entry_with_frame_addr = NULL;
	return 0x0;
}

void page_frames_init(multiboot_info_t *mbd)
{
	mmap_entry_t *entry = NULL;
	uintptr_t first_aligned_addr =
		get_first_aligned_page_frame_addr(mbd, &entry);
	if (entry == NULL) {
		abort("ERROR: failed to retrieve first available address after kernel code\n");
	}

	uintptr_t current_addr = first_aligned_addr;
	int i = 0;
	while (current_addr < entry->addr_low + entry->len_low - PAGE_SIZE) {
		if (i >= PAGE_FRAME_COUNT) {
#ifdef DEBUG_LOGGING
			printf("WARNING: ran out of page_frames array space\n");
#endif
			break;
		}

		page_frames[i].page_frame_t = current_addr;
		page_frames[i].used = FALSE;
		page_frames_len++;

		current_addr += PAGE_SIZE;
		i++;
	}
}

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

	ignore(page_frames_len);
	ignore(page_frames);

	page_frames_init(mbd);

#ifdef DEBUG_LOGGING
	printf("initting memory map OK\n");
#endif
}
