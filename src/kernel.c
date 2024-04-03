#include "mini_uart.h"
#include "shell.h"
#include "heap.h"
#include "libc.h"
#include "io.h"

void kernel_main(void) {
    uart_init();
    print("\n\r\n\rbOSnnie - broken Operating System for no normal individials ever\n\r\n\r");
 
    Allocation* allocation = heap_allocate(10);
    heap_print();
    memset(allocation->memory, (unsigned char)1, allocation->size);
    
    // Doesn't return
    shell_run();
}
