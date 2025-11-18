#ifndef _KERNEL_MEMORY_MANAGER_H
#define _KERNEL_MEMORY_MANAGER_H

#include <stdbool.h>
#include <stdint.h>

// enum mem_header_state {
// 	MEM_HEADER_STATE_FREE = 0x01,
// 	MEM_HEADER_STATE_USED,
// };
//
// struct mem_header {
// 	uint32_t size; // requested size
// 	uint32_t actual_size; // upto page boundary
// 	struct mem_header *next_free_block;
// 	enum mem_header_state used;
// }__attribute__((packed));
//
// #define MEMORY_MAP_START 0xDEADC0DE
//
// void *kmalloc(uint32_t size);
//
// // void *kcalloc(uint32_t size);
// //
// // void *krealloc(void* ptr);
//
// void kfree(void *ptr);
//
#endif
