#pragma once

#include <stdint.h>

extern void delay(uint64_t cycles);
extern void put32(uint64_t dst, uint32_t src);
extern uint32_t get32(uint64_t src);
