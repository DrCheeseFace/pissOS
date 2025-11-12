#ifndef _KERNEL_MEMORY_MAP_H
#define _KERNEL_MEMORY_MAP_H

#include <kernel/multiboot.h>
#include <stdint.h>

// why the FUCK does this have to be a uint8_t array
extern uint8_t endkernel[];

typedef multiboot_memory_map_t mmap_entry_t;

#endif
