#include <stddef.h>

#include "libc.h"

void* memset(void* str, int c, size_t n) {
    unsigned char* ptr = (unsigned char*)str;
    while (n-- > 0)
        *ptr++ = c;
    return str;
}

void* memcpy(void* dest, const void* src, size_t n) {
    char* dest_ptr = (char*)dest;
    char* src_ptr = (char*)src;
    while ((*dest_ptr++ = *src_ptr++));
    return dest;
}

size_t strlen(const char* str) {
    int len = 0;
    while (*str++) len++;
    return len;
}

// https://stackoverflow.com/a/34873406
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while (*s1 && (*s1 == *s2) && n-- > 0) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
