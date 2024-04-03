#include <stdarg.h>

#include "mini_uart.h"
#include "libc.h"

// Digit-to-char
char dtoc(int d) {
    return '0' + d;
}

// Integer-to-string 
int itoa(char* buffer, int buffer_start, int buffer_end, int i) {
    /* Collect each place value (in reverse order,
    up to 50 places) */
    char place_values[50] = {0};
    int num_places = 0;
    if (i == 0) {
        place_values[0] = '0';
        num_places = 1;
    }
    else {
        // https://stackoverflow.com/a/3389287
        for (int j = 0; i > 0 && j < 50; j++) {
            place_values[j] = dtoc(i % 10);
            i /= 10;
            num_places++;
        }
    }
    // Put each place value back into the buffer
    int j = buffer_start;
    for (int k = num_places - 1; k >= 0 && j + k < buffer_end; k--, j++)
        buffer[j] = place_values[k];
    return j;
}

void print(char* string) {
    uart_send_string(string);
}

// TODO: other format specifiers, and using dyanmic allocation?
int printf(char* format, ...) {
    char buffer[2048 + 1] = {0};
    int format_len = strlen(format);

    va_list args;
    va_start(args, format);
    int i = 0;
    for (int j = 0; j < format_len; j++, i++) {
        // Increment `j` while parsing format specifier
        if (format[j] == '%' && ++j < format_len) {
            switch (format[j++]) {
                case 'd':
                    /* Set `i` to the index after the number has
                    been copied into the buffer */
                    i = itoa(buffer, i, 2048, va_arg(args, int));
                    break;
                case 's':
                    char* string = va_arg(args, char*);
                    for (int k = 0; k < strlen(string); k++, i++) {
                        buffer[i] = string[k];
                    }
                default:
                    break;
            }
        }
        buffer[i] = format[j];
    }
    va_end(args);
    buffer[i] = '\0';

    uart_send_string(buffer);
    return i;
}
