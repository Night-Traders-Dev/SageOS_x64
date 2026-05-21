#include "phys_alloc.h"
#include "dmesg.h"
#include <string.h>

#define MAX_PHYS_PAGES 1048576 /* 4GB support */
static uint8_t bitmap[MAX_PHYS_PAGES / 8];
static uint64_t next_free_page = 0;

static void mark_used(uint64_t page) {
    bitmap[page / 8] |= (1 << (page % 8));
}

static void mark_free(uint64_t page) {
    bitmap[page / 8] &= ~(1 << (page % 8));
}

static int is_used(uint64_t page) {
    return bitmap[page / 8] & (1 << (page % 8));
}

extern char __kernel_start[];
extern char __kernel_end[];

void phys_init(SageOSBootInfo *info) {
    // 1. Mark ALL pages as USED by default
    memset(bitmap, 0xFF, sizeof(bitmap));

    // 2. Parse the UEFI memory map if available to free conventional memory pages
    if (info && info->memory_map && info->memory_map_size && info->memory_desc_size) {
        uint64_t desc_size = info->memory_desc_size;
        uint64_t map_size = info->memory_map_size;
        uint8_t *map_ptr = (uint8_t*)(uintptr_t)info->memory_map;

        for (uint64_t offset = 0; offset < map_size; offset += desc_size) {
            uint32_t type = *(uint32_t*)(map_ptr + offset);
            uint64_t physical_start = *(uint64_t*)(map_ptr + offset + 8);
            uint64_t number_of_pages = *(uint64_t*)(map_ptr + offset + 24);

            // EfiConventionalMemory is type 7
            if (type == 7) {
                uint64_t start_page = physical_start / PAGE_SIZE;
                for (uint64_t i = 0; i < number_of_pages; i++) {
                    uint64_t page = start_page + i;
                    if (page < MAX_PHYS_PAGES) {
                        mark_free(page);
                    }
                }
            }
        }
    } else {
        // Fallback if no UEFI memory map
        for (uint64_t i = 256; i < 65536; i++) {
            mark_free(i);
        }
    }

    // 3. Mark critical regions as USED to override conventional memory free state
    
    /* Mark the first 1MB as used */
    for (uint64_t i = 0; i < 256; i++) {
        mark_used(i);
    }

    /* Mark all kernel pages as used */
    uint64_t start_page = (uint64_t)__kernel_start / PAGE_SIZE;
    uint64_t end_page = ((uint64_t)__kernel_end + PAGE_SIZE - 1) / PAGE_SIZE;
    for (uint64_t i = start_page; i < end_page; i++) {
        mark_used(i);
    }

    /* Mark backbuffer pages as used if present */
    if (info && info->backbuffer_address) {
        uint64_t bb_start = info->backbuffer_address / PAGE_SIZE;
        uint64_t bb_pages = 4096; /* 16MB */
        for (uint64_t i = bb_start; i < bb_start + bb_pages; i++) {
            mark_used(i);
        }
    }

    /* Mark the memory map pages as used */
    if (info && info->memory_map) {
        uint64_t map_start = info->memory_map / PAGE_SIZE;
        uint64_t map_pages = (info->memory_map_size + PAGE_SIZE - 1) / PAGE_SIZE;
        for (uint64_t i = map_start; i < map_start + map_pages; i++) {
            mark_used(i);
        }
    }

    dmesg_log("phys_alloc: initialized using UEFI memory map");
}

void* phys_alloc(void) {
    for (uint64_t i = next_free_page; i < MAX_PHYS_PAGES; i++) {
        if (!is_used(i)) {
            mark_used(i);
            next_free_page = i + 1;
            return (void*)(i * PAGE_SIZE);
        }
    }
    for (uint64_t i = 0; i < next_free_page; i++) {
        if (!is_used(i)) {
            mark_used(i);
            next_free_page = i + 1;
            return (void*)(i * PAGE_SIZE);
        }
    }
    return NULL;
}

void phys_free(void *addr) {
    uint64_t page = (uint64_t)addr / PAGE_SIZE;
    mark_free(page);
    if (page < next_free_page) {
        next_free_page = page;
    }
}
