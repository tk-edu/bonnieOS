#include <stdint.h>

#include "peripherals/mini_uart.h"
#include "peripherals/gpio.h"
#include "utils.h"

void uart_init(void) {
    uint32_t selector;

    selector = get32(GPFSEL1);
    // Clean GPIO14
    selector &= ~(7 << 12);
    // Select alternative function 5 for GPIO14 (uart transmitter)
    selector |= 2 << 12;
    // Clean GPIO15
    selector &= ~(7 << 15);
    // Set alternative function 5 for GPIO15 (uart receiver)
    selector |= 2 << 15;
    // Put the constructed value in the selection register
    put32(GPFSEL1, selector);

    put32(GPPUD, 0);
    delay(150);
    put32(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);
    put32(GPPUDCLK0, 0);

    put32(AUX_ENABLES, 1);       // Enable mini uart (this also enables access to its registers)
    put32(AUX_MU_CNTL_REG, 0);   // Disable auto flow control and disable receiver and transmitter)
    put32(AUX_MU_IER_REG, 0);    // Disable receive and transmit interrupts
    put32(AUX_MU_LCR_REG, 3);    // Enable 8-bit mode
    put32(AUX_MU_MCR_REG, 0);    // Set RTS line to always be high
#if RPI_VERSION == 3
    put32(AUX_MU_BAUD_REG, 270); // Set baud rate to 115200 @ 250 MHz
#endif
#if RPI_VERSION == 4
    put32(AUX_MU_BAUD_REG, 541); // Set baud rate to 115200 @ 500 MHz
#endif

    put32(AUX_MU_CNTL_REG, 3);   // Finally, enable transmitter and receiver
}

void uart_send(char c) {
    while (1) {
        if (get32(AUX_MU_LSR_REG) & 0x20)
            break;
    }
    put32(AUX_MU_IO_REG, c);
}

char uart_recv(void) {
    while (1) {
        if (get32(AUX_MU_LSR_REG) & 0x01)
            break;
    }
    return (get32(AUX_MU_IO_REG) & 0xFF);
}

void uart_send_string(char* str) {
    for (int i = 0; str[i] != '\0'; i++)
        uart_send((char)str[i]);
}
