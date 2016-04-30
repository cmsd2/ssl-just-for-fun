#pragma once

#define GET_BIT(array, bit) \
        (array[(int)(bit / 8)] & (0x80 >> (bit % 8)))

#define SET_BIT(array, bit) \
        (array[(int)(bit / 8)] |= (0x80 >> (bit % 8)))

#define CLEAR_BIT(array, bit) \
        (array[(int)(bit / 8)] &= ~(0x80 >> (bit % 8)))

void xor(unsigned char *target, const unsigned char *src, int len);
