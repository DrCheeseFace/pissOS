#include <kernel/memory_manager.h>
#include <kernel/misc.h>
#include <kernel/paging.h>
#include <stddef.h>

// void memory_manager_init(void)
// {
// 	page_frame_t mem_root_page = kmalloc_frame();
//         *(uint32_t*)mem_root_page = MEMORY_MAP_START;
// }

// void *kmalloc(uint32_t size)
// {
// 	(void)size;
// 	return NULL;
// }
//
// void kfree(void *ptr)
// {
// 	ignore(ptr);
// }
