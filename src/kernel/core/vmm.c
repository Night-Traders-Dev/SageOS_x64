#include "vmm.h"
#include "phys_alloc.h"
#include <string.h>
#include "io.h"

#define ALIGN_DOWN(addr, size) ((addr) & ~((size) - 1))

static uint64_t *pml4;

void vmm_init(void) {
    pml4 = (uint64_t*)phys_alloc();
    memset(pml4, 0, PAGE_SIZE);

    // Identity-map the first 4GB of physical address space using 2MB huge pages (PS flag = bit 7)
    uint64_t *pdpt = (uint64_t*)phys_alloc();
    memset(pdpt, 0, PAGE_SIZE);
    pml4[0] = (uint64_t)pdpt | PAGE_PRESENT | PAGE_WRITABLE;

    for (uint64_t gb = 0; gb < 4; gb++) {
        uint64_t *pd = (uint64_t*)phys_alloc();
        memset(pd, 0, PAGE_SIZE);
        pdpt[gb] = (uint64_t)pd | PAGE_PRESENT | PAGE_WRITABLE;

        for (uint64_t entry = 0; entry < 512; entry++) {
            uint64_t paddr = (gb * 1024 * 1024 * 1024ULL) + (entry * 2 * 1024 * 1024ULL);
            pd[entry] = paddr | PAGE_PRESENT | PAGE_WRITABLE | (1ULL << 7);
        }
    }

    // Load standard CR3 page table root
    write_cr3((uint64_t)pml4);
}

void vmm_map(uint64_t vaddr, uint64_t paddr, uint64_t flags) {
    /* Simplified implementation for mapping one page */
    int pml4_idx = (vaddr >> 39) & 0x1FF;
    int pdpt_idx = (vaddr >> 30) & 0x1FF;
    int pd_idx   = (vaddr >> 21) & 0x1FF;
    int pt_idx   = (vaddr >> 12) & 0x1FF;

    if (!(pml4[pml4_idx] & PAGE_PRESENT)) {
        uint64_t *pdpt = (uint64_t*)phys_alloc();
        memset(pdpt, 0, PAGE_SIZE);
        pml4[pml4_idx] = (uint64_t)pdpt | PAGE_PRESENT | PAGE_WRITABLE;
    }

    uint64_t *pdpt = (uint64_t*)(pml4[pml4_idx] & ~0xFFF);
    if (!(pdpt[pdpt_idx] & PAGE_PRESENT)) {
        uint64_t *pd = (uint64_t*)phys_alloc();
        memset(pd, 0, PAGE_SIZE);
        pdpt[pdpt_idx] = (uint64_t)pd | PAGE_PRESENT | PAGE_WRITABLE;
    }

    uint64_t *pd = (uint64_t*)(pdpt[pdpt_idx] & ~0xFFF);
    if (!(pd[pd_idx] & PAGE_PRESENT)) {
        uint64_t *pt = (uint64_t*)phys_alloc();
        memset(pt, 0, PAGE_SIZE);
        pd[pd_idx] = (uint64_t)pt | PAGE_PRESENT | PAGE_WRITABLE;
    }

    uint64_t *pt = (uint64_t*)(pd[pd_idx] & ~0xFFF);
    pt[pt_idx] = (uint64_t)paddr | flags | PAGE_PRESENT;
}
