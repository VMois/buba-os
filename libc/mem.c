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

// head of linkedlist
u32 free_mem_addr = 0x10000;

typedef struct memory_sector {
    struct memory_sector *next;
    u32 is_used;
    u32 pages;
} MEM_SEC;

u32 PAGE_SIZE = 0x1000;
u32 calculate_pages_num(u32 size) {
    u32 pages_num = size / PAGE_SIZE;
    if ((size % PAGE_SIZE) > 0)
        pages_num++;
    return pages_num;
}

MEM_SEC *find_empty_slot(u32 pages_num) {
    MEM_SEC *p = (MEM_SEC*) free_mem_addr;
    while (p != NULL) {
        if (p->is_used == 0 && p->pages >= pages_num)
            return p;
        p = p->next;
    }
    return NULL;
}

MEM_SEC *find_last_slot() {
    MEM_SEC *p = (MEM_SEC*) free_mem_addr;
    while (p != NULL)
        if (p->next == NULL)
            return p;
    return NULL;
}

void insert(u32 addr, u32 pages) {
    MEM_SEC *new_slot = (MEM_SEC*) addr;
    new_slot->is_used = 1;
    new_slot->pages = pages;
    new_slot->next = NULL;
}

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

    // it is a first time we allocate memory
    if (free_mem_addr == 0x10000) {
        free_mem_addr += 1;
        MEM_SEC *msec = (MEM_SEC*) free_mem_addr;
        msec->next = NULL;
        msec->is_used = 1;
        msec->pages = pages_num;
        return (u32) msec + (u32) sizeof(MEM_SEC);
    } else {
        MEM_SEC *slot = find_empty_slot(pages_num);
        if (slot == NULL) {
            slot = find_last_slot();
            u32 new_addr = (u32) slot + sizeof(MEM_SEC) + slot->pages * PAGE_SIZE;
            insert(new_addr, pages_num);
            return (u32) new_addr + (u32) sizeof(MEM_SEC) + (u32) pages_num * PAGE_SIZE;
        }
        slot->is_used = 1;
        return (u32) slot + (u32) sizeof(MEM_SEC);
    }
}
