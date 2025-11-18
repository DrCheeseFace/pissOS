#include <kernel/misc.h>
#include <kernel/paging.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

global_variable uint32_t *page_directory;
global_variable page_state_t page_frames_state[MAX_PAGE_FRAME_COUNT];
global_variable uintptr_t page_frames_start_addr;
global_variable size_t page_frames_len = 0;
global_variable page_frame_t pre_frames[BATCH_PAGES_ALLOCED_MAX];

internal void set_page_frames_start_addr(multiboot_info_t *mbd,
					 mmap_entry_t **entry_with_frame_addr);

internal void page_frames_init(multiboot_info_t *mbd);
internal page_frame_t kmalloc_frame_int(void);

void paging_init(uint32_t magic, multiboot_info_t *mbd)
{
#ifdef DEBUG_LOGGING
	printf("init paging\n");
#endif
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		abort("ERROR: invalid memory map given by grub bootloader");
	}

	if (!(mbd->flags >> 6 & 0x1)) {
		abort("ERROR: invalid memory map given by grub bootloader");
	}

	page_frames_init(mbd);

	page_frame_t page_directory_phys = kmalloc_frame_int();
	if (page_directory_phys == (page_frame_t)ERROR) {
		abort("ERROR: failed to allocate frame for page directory");
	}
	page_directory = (uint32_t *)page_directory_phys;

	page_frame_t first_page_table_phys = kmalloc_frame_int();
	if (first_page_table_phys == (page_frame_t)ERROR) {
		abort("ERROR: failed to allocate frame for first page table");
	}
	uint32_t *first_page_table = (uint32_t *)first_page_table_phys;
	for (uint16_t i = 0; i < PAGES_PER_TABLE; i++) {
		first_page_table[i] = (i * PAGE_SIZE) | 3;
	}

	page_directory[0] = first_page_table_phys | 3;

	loadPageDirectory(page_directory_phys);
	enablePaging();

#ifdef DEBUG_LOGGING
	printf("init paging OK\n");
#endif
}

internal void page_frames_init(multiboot_info_t *mbd)
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

		page_frames_state[i] = PAGE_STATE_FREE;
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

internal page_frame_t kmalloc_frame_int(void)
{
	uint32_t i = 0;
	while (page_frames_state[i] != PAGE_STATE_FREE) {
		i++;
		if (i == page_frames_len) {
			printf("WARNING: run out of page frames\n");
			return (ERROR);
		}
	}
	page_frames_state[i] = PAGE_STATE_USED;
	return (page_frames_start_addr + (i * PAGE_SIZE));
}

void kfree_frame(page_frame_t a)
{
	page_frame_t offset = a - page_frames_start_addr;

	uint32_t index = ((uint32_t)offset) / PAGE_SIZE;

	if (index < page_frames_len) {
		page_frames_state[index] = PAGE_STATE_FREE;
	}
}

page_frame_t kmalloc_frame(void)
{
	local_persist uint8_t allocate = 1;
	local_persist uint8_t pframe = 0;
	page_frame_t ret;

	if (pframe == BATCH_PAGES_ALLOCED_MAX) {
		allocate = 1;
	}

	if (allocate == 1) {
		for (int i = 0; i < BATCH_PAGES_ALLOCED_MAX; i++) {
			pre_frames[i] = kmalloc_frame_int();
		}
		pframe = 0;
		allocate = 0;
	}
	ret = pre_frames[pframe];
	pframe++;
	return (ret);
}
