/* memory.h - Memory manager (heap allocation) */
#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

/* Initialize the memory manager with available heap */
void mem_init(uint32_t heap_start, uint32_t heap_size);

/* Allocate memory, returns pointer or NULL on failure */
void* malloc(size_t size);

/* Free previously allocated memory */
void free(void* ptr);

/* Realloc - resize an allocation */
void* realloc(void* ptr, size_t size);

/* Get heap statistics */
void mem_stats(void);

/* Dump all allocations (debug) */
void mem_dump(void);

#endif
