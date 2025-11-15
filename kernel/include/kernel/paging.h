#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H

#include <kernel/multiboot.h>
#include <stdint.h>

extern uint8_t endkernel[];
typedef multiboot_memory_map_t mmap_entry_t;

#define PAGE_SIZE 0x1000 // 4096
#define MAX_PAGE_FRAME_COUNT 1024 * 1024

#define BATCH_PAGES_ALLOCED_MAX 20
#define PAGE_DIRECTORY_LEN 1024
#define PAGE_DIRECTORY_ENTRY_INIT 0x00000002

typedef uintptr_t page_frame_t;
#define ERROR 0xCAFEBABE

enum page_frame_state {
	PAGE_FRAME_STATE_FREE = 0x00,
	PAGE_FRAME_STATE_USED,
};

void paging_init(uint32_t magic, multiboot_info_t *mbd);

page_frame_t kalloc_frame(void);

void kfree_frame(page_frame_t a);

extern void loadPageDirectory(uint32_t addr);

extern void enablePaging(void);

#endif
