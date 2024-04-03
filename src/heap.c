#include <stddef.h>

#include "heap.h"
#include "io.h"

// https://stackoverflow.com/a/2403643
int next_highest_multiple(int n, int m) {
    return n % m ? n = n + (m - n % m) : n;
}

// Allocates in multiples of 8 (minimum allocarion being 8 bytes)
Allocation* heap_allocate(int num_bytes) {
    print("Allocating...");
    for (int i = 0; i < 1024; i++) {
        if (!ALLOCATIONS[i].size) {
            NUM_ALLOCATIONS++;
            /* Dumb solution to find the index the
            the current allocation should have on
            the heap */
            int heap_index = 0;
            for (int j = 0; j < i; j++)
                heap_index += ALLOCATIONS[i].size;
            printf("Found space at index %d\r\n", i);
            ALLOCATIONS[i].memory = &HEAP[heap_index];
            ALLOCATIONS[i].size = next_highest_multiple(num_bytes, 8);
            return &ALLOCATIONS[i];
        }
    }
    /* Return NULL if there is
    no more memory to allocate */
    return NULL;
}

Allocation* heap_reallocate(Allocation* allocation, int new_size) {
    return NULL; // TODO
}

void heap_free(Allocation* allocation) {
    allocation->size = 0;
}

void heap_print() {
    for (int i = 0; i < NUM_ALLOCATIONS; i++)
        printf("Allocation %d:\r\n  Size: %d\r\n", i, ALLOCATIONS[i].size);
}
