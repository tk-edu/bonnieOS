#include "mm.h"

.section ".text.boot"

.globl _start
_start:
    // Get current processor ID from
    // the `mpidr_el1` system register
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF 
    // If the processor ID is 0, then branch to
    // main. Let the other three cores hang.
    cbz     x0, main 
    b       proc_hang

proc_hang:
    b proc_hang

main:
    // Move the relative addresses
    // of the `bss_begin` and `bss_end`
    // labels into x0 and x1 respectively
    adr     x0, bss_begin
    adr     x1, bss_end
    // Calculate the size of the bss section
    sub     x1, x1, x0
    // Zero out the memory in the bss section
    bl      memzero
    // Initialize the stack pointer and hand
    // off execution to the kernel
    mov     sp, #LOW_MEMORY
    bl      kernel_main
