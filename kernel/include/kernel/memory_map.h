#ifndef _KERNEL_MEMORY_MAP_H
#define _KERNEL_MEMORY_MAP_H

#include <kernel/multiboot.h>
#include <stdint.h>

#define PAGE_SIZE 0x1000
#define PAGE_FRAME_COUNT 1024 * 1024

// why the FUCK does this have to be a uint8_t array
extern uint8_t endkernel[];

typedef struct {
	uintptr_t page_frame_t;
	short used;
} page_frame_t;

typedef multiboot_memory_map_t mmap_entry_t;

#endif
