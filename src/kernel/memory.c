/* memory.c - Heap allocation with free-list and coalescing */
#include "memory.h"
#include "serial.h"
#include "string.h"

#define HEAP_ALIGN 8
#define MIN_ALLOC 16

typedef struct {
    uint32_t size;      /* size of this block (including header) */
    int free;           /* 1 = free, 0 = allocated */
    struct {
        void *prev;
        void *next;
    } list;
} mem_block_t;

static uint32_t heap_start = 0;
static uint32_t heap_size = 0;
static uint32_t heap_used = 0;
static mem_block_t *free_list = NULL;

static void print_u32(uint32_t v) {
    char buf[12];
    int pos = 0;
    if (v == 0) { serial_putc('0'); return; }
    while (v) {
        buf[pos++] = '0' + (v % 10);
        v /= 10;
    }
    while (pos--) serial_putc(buf[pos]);
}

static void print_hex(uint32_t v) {
    serial_puts("0x");
    int i;
    for (i = 7; i >= 0; i--) {
        uint8_t nibble = (v >> (i * 4)) & 0xF;
        serial_putc(nibble < 10 ? '0' + nibble : 'a' + nibble - 10);
    }
}

void mem_init(uint32_t start, uint32_t size) {
    heap_start = start;
    heap_size = size;
    heap_used = 0;

    /* Initialize first block: entire heap is free */
    mem_block_t *first = (mem_block_t*)heap_start;
    first->size = heap_size;
    first->free = 1;
    first->list.prev = NULL;
    first->list.next = NULL;
    free_list = first;

    serial_puts("[MEM] Initialized at ");
    print_hex(heap_start);
    serial_puts(" size=");
    print_u32(heap_size);
    serial_puts("\n");
}

/* Align size up to HEAP_ALIGN boundary */
static size_t align_up(size_t sz) {
    return (sz + HEAP_ALIGN - 1) & ~(HEAP_ALIGN - 1);
}

/* Try to coalesce with next block if both are free */
static void coalesce(mem_block_t *blk) {
    if (!blk) return;
    
    mem_block_t *next = (mem_block_t*)blk->list.next;
    if (next && next->free) {
        /* Merge blk with next */
        blk->size += next->size;
        blk->list.next = next->list.next;
        if (next->list.next) {
            ((mem_block_t*)next->list.next)->list.prev = blk;
        }
    }

    /* Also try to coalesce with previous */
    mem_block_t *prev = (mem_block_t*)blk->list.prev;
    if (prev && prev->free) {
        prev->size += blk->size;
        prev->list.next = blk->list.next;
        if (blk->list.next) {
            ((mem_block_t*)blk->list.next)->list.prev = prev;
        }
    }
}

void* malloc(size_t size) {
    if (size == 0) return NULL;
    if (!heap_start) return NULL;

    /* Required size: block header + payload, aligned */
    size_t req = align_up(sizeof(mem_block_t) + size);
    if (req < MIN_ALLOC) req = MIN_ALLOC;

    /* First-fit: find first free block large enough */
    mem_block_t *blk = free_list;
    while (blk) {
        if (blk->free && blk->size >= req) {
            /* Found suitable block. Split if too large. */
            if (blk->size > req + MIN_ALLOC) {
                /* Create new free block from remainder */
                mem_block_t *new_blk = (mem_block_t*)((uint8_t*)blk + req);
                new_blk->size = blk->size - req;
                new_blk->free = 1;
                new_blk->list.prev = blk;
                new_blk->list.next = blk->list.next;
                if (blk->list.next) {
                    ((mem_block_t*)blk->list.next)->list.prev = new_blk;
                }
                blk->list.next = new_blk;
                blk->size = req;
            }
            
            /* Mark as allocated and return payload */
            blk->free = 0;
            heap_used += req;
            return (void*)((uint8_t*)blk + sizeof(mem_block_t));
        }
        blk = (mem_block_t*)blk->list.next;
    }

    return NULL; /* Out of memory */
}

void free(void* ptr) {
    if (!ptr || !heap_start) return;

    /* Get block header (located before payload) */
    mem_block_t *blk = (mem_block_t*)ptr - 1;

    if (blk->free) {
        serial_puts("[MEM] Double-free detected at ");
        print_hex((uint32_t)ptr);
        serial_puts("\n");
        return;
    }

    blk->free = 1;
    heap_used -= blk->size;

    /* Attempt coalescing */
    coalesce(blk);
}

void* realloc(void* ptr, size_t new_size) {
    if (!ptr) return malloc(new_size);
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    /* Get current block */
    mem_block_t *blk = (mem_block_t*)ptr - 1;
    size_t old_size = blk->size - sizeof(mem_block_t);

    /* If new size fits in current block, resize in-place */
    if (new_size <= old_size) {
        return ptr;
    }

    /* Allocate new block, copy data, free old */
    void *new_ptr = malloc(new_size);
    if (!new_ptr) return NULL;

    size_t i;
    uint8_t *src = (uint8_t*)ptr;
    uint8_t *dst = (uint8_t*)new_ptr;
    for (i = 0; i < old_size; i++) {
        dst[i] = src[i];
    }

    free(ptr);
    return new_ptr;
}

void mem_stats(void) {
    uint32_t free_total = 0;
    uint32_t free_count = 0;
    uint32_t alloc_count = 0;

    mem_block_t *blk = free_list;
    while (blk) {
        if (blk->free) {
            free_total += blk->size;
            free_count++;
        } else {
            alloc_count++;
        }
        blk = (mem_block_t*)blk->list.next;
    }

    serial_puts("[MEM STATS]\n");
    serial_puts("  Total heap:   ");
    print_u32(heap_size);
    serial_puts(" bytes\n");
    serial_puts("  Used:         ");
    print_u32(heap_used);
    serial_puts(" bytes\n");
    serial_puts("  Free:         ");
    print_u32(free_total);
    serial_puts(" bytes\n");
    serial_puts("  Free blocks:  ");
    print_u32(free_count);
    serial_puts("\n");
    serial_puts("  Alloc blocks: ");
    print_u32(alloc_count);
    serial_puts("\n");
}

void mem_dump(void) {
    serial_puts("[MEM DUMP]\n");
    int idx = 0;
    mem_block_t *blk = free_list;
    while (blk) {
        serial_puts("  [");
        print_u32(idx++);
        serial_puts("] addr=");
        print_hex((uint32_t)blk);
        serial_puts(" size=");
        print_u32(blk->size);
        serial_puts(" state=");
        serial_puts(blk->free ? "FREE" : "USED");
        serial_puts("\n");
        blk = (mem_block_t*)blk->list.next;
    }
}
