#include <stdint.h>

#include "peripherals/gpio.h"
#include "mini_uart.h"
#include "shell.h"
#include "utils.h"
#include "libc.h"
#include "io.h"

void echo(char* text) {
    printf("%s\n\r", text);
}

void gpio(int pin, int value) {
    uint32_t GPFSEL;
    switch (pin) {
        case 0: case 1: case 2:
        case 3: case 4: case 5:
        case 6: case 7: case 8: case 9:
            GPFSEL = GPFSEL0;
            break;
        case 10: case 11: case 12:
        case 13: case 16: case 17:
        case 18: case 19:
            GPFSEL = GPFSEL1;
            break;
        case 20: case 21: case 22:
        case 23: case 24: case 25:
        case 26: case 27:
            GPFSEL = GPFSEL2;
            break;
        case 14: case 15:
            printf("Error: Cannot toggle GPIO pins 14 or 15; currently in use for UART\n\r");
            return;
        default:
            printf("Error: unsupported GPIO pin\n\r");
            return;
    }
    uint32_t selector = get32(GPFSEL);
    // Clear selector
    selector &= ~(7 << 12);
    // Set GPIO pin to be an output
    uint32_t pin_bit_offset = 0;
    if (GPFSEL == GPFSEL0)
        pin_bit_offset = pin * 3;
    else if (GPFSEL == GPFSEL1) {
        for (int i = pin; i <= 19; i++)
            pin_bit_offset += 3;
    }
    else if (GPFSEL == GPFSEL2) {
        for (int i = pin; i <= 27; i++)
            pin_bit_offset += 3;
    }
    selector |= 1 << (pin_bit_offset);
    put32(GPFSEL, selector);
    // Toggle pin
    put32(value == 1 ? GPSET0 : GPCLR0, 1);
    uint32_t gp_set_clear_reg = get32(value == 1 ? GPSET0 : GPCLR0);
    gp_set_clear_reg |= 1 << pin;
    put32(value == 1 ? GPSET0 : GPCLR0, gp_set_clear_reg);
}

char* shell_run_command(char* cmd, char args[50][512], int num_args) {
    if (!strncmp(cmd, "echo", 4))
        echo(args[0]);
    else if (!strncmp(cmd, "gpio", 4)) {
        /* 1 is high, 0 is low.
        On  = tie low
        Off = tie high */
        int value = 0;
        if (!strncmp(args[1], "on", 2))
            value = 1;
        printf("Toggling pin %d\n\r", atoi(args[0]));
        gpio(atoi(args[0]), value);
    }
}

void shell_run() {
    // Command can be 512 bytes long
    char cmd_buffer[512] = {0};
    // There can be 50 arguments, each 512 bytes long
    char args_buffer[50][512] = {0};
    int num_args = 0;

    // Start off by filling the command buffer
    char* cur_buffer = cmd_buffer;
    int buffer_len = 0;

    char next = '\0';
    while (1) {
        next = uart_recv();
        cur_buffer[buffer_len++] = next;
        /* Assumes your serial terminal is only
        sending \r, and not \r\n */
        if (next == '\r') {
            print("\n\r");
            cur_buffer[buffer_len - 1] = '\0';
            shell_run_command(cmd_buffer, args_buffer, num_args);

            // Clear buffers for next command
            for (int i = 0; i < strlen(cmd_buffer); i++)
                cmd_buffer[i] = '\0';
            for (int i = 0; i < num_args; i++) {
                for (int j = 0; j < strlen(args_buffer[i]); j++)
                    args_buffer[i][j] = '\0';
            }
            buffer_len = 0;

            cur_buffer = cmd_buffer;
            num_args = 0;

            continue;
        }
        // Parse argument
        else if (next == ' ') {
            /* Remove the trailing space and null-
            terminate the previbw2bus buffer */
            cur_buffer[buffer_len - 1] = '\0';
            cur_buffer = &args_buffer[num_args++];
            buffer_len = 0;
        }
        //be able to backspace in the shell
        else if((int)next == 127){
            uart_send('\010');
            uart_send(' ');
            uart_send('\010');
            continue;
        }
        
        // Echo character
        uart_send(next);
        //printf("%d", (int)next);
    }
}
