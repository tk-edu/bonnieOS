#pragma once

typedef struct {
    unsigned char* memory;
    int size;
} Allocation;

// 2kb heap
static unsigned char HEAP[8192];
// Smallest allocation is 8 bytes
static Allocation ALLOCATIONS[1024];
static int NUM_ALLOCATIONS = 0;

Allocation* heap_allocate(int num_bytes);
Allocation* heap_reallocate(Allocation* allocation, int new_size);
void heap_print();
void heap_free(Allocation* allocation);
