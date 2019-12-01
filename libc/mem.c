#include "mem.h"
#include "../cpu/type.h"

void memory_copy(u8 *source, u8 *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(u8 *dest, u8 val, u32 len) {
    u8 *temp = (u8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

/*
Dynamic Memory Allocation (DMA) code below.

For now, the code is smarter than just growing pointer 
but still far from good dynamic allocation.
TODO: Instead of reusing memory sectors it will be more practical to remove them and
later in linked list detect available memory spaces and fill them with new sectors.
*/

/*
Private functions and variables
*/

// start of available memory, head of the linked list
u32 free_mem_addr = 0x10000;

// pre-defined numbers of bytes to allocate
// 0x200(h) = 512(d) bytes
u32 PAGE_SIZE = 0x200;

// memory sector (MEM_SEC)
typedef struct MEM_SEC {
    struct MEM_SEC *next;
    u32 is_used;
    u32 pages;
} MEM_SEC;

u32 calculate_pages_num(u32 size) {
    u32 pages_num = size / PAGE_SIZE;
    if ((size % PAGE_SIZE) > 0)
        pages_num++;
    return pages_num;
}

// find if any defined before MEM_SEC is not used 
// and if it has enough pages to use
MEM_SEC *find_free_sector(u32 pages_num) {
    MEM_SEC *p = (MEM_SEC*) free_mem_addr;
    while (p != NULL) {
        if (p->is_used == 0 && p->pages >= pages_num)
            return p;
        p = p->next;
    }
    return NULL;
}

MEM_SEC *insert_sector(u32 pages) {
    MEM_SEC *p = (MEM_SEC*) free_mem_addr;
    while (p != NULL) {
        if (p->next == NULL) {
            u32 new_addr = (u32) p + sizeof(MEM_SEC) + p->pages * PAGE_SIZE;
            MEM_SEC *new_sector = (MEM_SEC*) new_addr;
            new_sector->is_used = 1;
            new_sector->pages = pages;
            new_sector->next = NULL;

            p->next = new_sector;
            return new_sector;
        }
        p = p->next;
    }
    return NULL;
}

u32 calculate_memory_pointer(MEM_SEC *mem_sec) {
    return (u32) mem_sec + (u32) sizeof(MEM_SEC); 
}

/*
Public functions
*/

void free(void *addr) {
    MEM_SEC *p = (MEM_SEC*) free_mem_addr;
    while (p != NULL) {
        if (((u32) p + sizeof(MEM_SEC)) == (u32) addr) {
            p->is_used = 0;
            return;
        }
        p = p->next;
    }
}

u32 kmalloc(u32 size) {
    u32 pages_num = calculate_pages_num(size);

    // allocate memory for the first time in the whole system
    if (free_mem_addr == 0x10000) {
        free_mem_addr += 1;
        MEM_SEC *msec = (MEM_SEC*) free_mem_addr;
        msec->next = NULL;
        msec->is_used = 1;
        msec->pages = pages_num;
        return calculate_memory_pointer(msec);
    }

    MEM_SEC *sector = find_free_sector(pages_num);
    if (sector != NULL) {
        sector->is_used = 1;
    } else {
        sector = insert_sector(pages_num);
    }
    return calculate_memory_pointer(sector);
}
