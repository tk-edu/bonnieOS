#include "mini_uart.h"
#include "shell.h"
#include "libc.h"
#include "io.h"

void echo(char* text) {
    printf("%s\n\r", text);
}

char* shell_run_command(char* cmd, char args[50][512], int num_args) {
    if (!strncmp(cmd, "echo", 4))
        echo(args[0]);
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
