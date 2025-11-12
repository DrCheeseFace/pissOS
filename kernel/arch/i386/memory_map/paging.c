#include <kernel/memory_map.h>
#include <kernel/misc.h>
#include <kernel/paging.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

global_variable enum PageFrameState page_frames_state[MAX_PAGE_FRAME_COUNT];
global_variable uintptr_t page_frames_start_addr;
global_variable size_t page_frames_len = 0;

internal void set_page_frames_start_addr(multiboot_info_t *mbd,
					 mmap_entry_t **entry_with_frame_addr);

void page_frames_init(multiboot_info_t *mbd)
{
	mmap_entry_t *entry = NULL;
	set_page_frames_start_addr(mbd, &entry);
	if (entry == NULL) {
		abort("ERROR: failed to retrieve first available address after kernel "
		      "code\n");
	}

	uintptr_t current_addr = page_frames_start_addr;
	int i = 0;
	while (current_addr < entry->addr_low + entry->len_low - PAGE_SIZE) {
		if (i >= MAX_PAGE_FRAME_COUNT) {
#ifdef DEBUG_LOGGING
			printf("WARNING: ran out of page_frames array space\n");
#endif
			break;
		}

		page_frames_state[i] = PAGE_FRAME_STATE_FREE;
		page_frames_len++;

		current_addr += PAGE_SIZE;
		i++;
	}
}

internal void set_page_frames_start_addr(multiboot_info_t *mbd,
					 mmap_entry_t **entry_with_frame_addr)
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
					page_frames_start_addr = aligned_addr;
					return;
				}

				// gets to this line if the aligned address is outside of the available
				// region
			}
		}

		entry = (mmap_entry_t *)((unsigned int)entry + entry->size +
					 sizeof(entry->size));
	}

#ifdef DEBUG_LOGGING
	printf("WARNING: failed to retrieve first available address after kernel "
	       "code\n");
#endif
	entry_with_frame_addr = NULL;
}

page_frame_t kalloc_frame_int(void)
{
	uint32_t i = 0;
	while (page_frames_state[i] != PAGE_FRAME_STATE_FREE) {
		i++;
		if (i == page_frames_len) {
			printf("WARNING: run out of page frames");
			return (ERROR);
		}
	}
	page_frames_state[i] = PAGE_FRAME_STATE_USED;
	return (page_frames_start_addr + (i * PAGE_SIZE));
}

void kfree_frame(page_frame_t a)
{
	a = a - page_frames_start_addr;
	if (a == 0) {
		uint32_t index = (uint32_t)a;
		page_frames_state[index] = PAGE_FRAME_STATE_USED;
	}
	else {
		uint32_t index = ((uint32_t)a) / 0x1000;
		page_frames_state[index] = PAGE_FRAME_STATE_FREE;
	}
}
