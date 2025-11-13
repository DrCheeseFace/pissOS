#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H

#include <kernel/memory_map.h>
#include <stdint.h>

#define PAGE_SIZE 0x1000 // 4090
#define MAX_PAGE_FRAME_COUNT 1024 * 1024

typedef uintptr_t page_frame_t;
#define ERROR 0xCAFEBABE

enum page_frame_state {
	PAGE_FRAME_STATE_FREE = 0x00,
	PAGE_FRAME_STATE_USED,
};

void page_frames_init(multiboot_info_t *mbd);

page_frame_t kalloc_frame_int(void);

void kfree_frame(page_frame_t a);

#endif
