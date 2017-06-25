//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Public Domain/MIT, see licenses/mit.md for more details.

#include "byteorder.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>          // TODO: remove

// HELPERS
// -------


/**
 *  \brief Byte-swap value in-place.
 */
void bswap_impl(void* buf, int width)
{
    auto *data = reinterpret_cast<uint8_t*>(buf);

    auto *lo = data;
    auto *hi = data + width - 1;
    uint8_t swap;

    while (lo < hi) {
        swap = *lo;
        *lo++ = *hi;
        *hi-- = swap;
    }
}

// FUNCTIONS
// ---------


void bswap(void* buf, int width)
{
    bswap_impl(buf, width);
}


void bswap(void* dst, void* src, int width)
{
    if (src != dst) {
        memcpy(dst, src, width);
    }
    bswap(dst, width);
}


#if defined(NEED_BSWAPXX)

uint16_t bswap16(uint16_t i)
{
    uint16_t v = i;
    bswap(&v, 2);
}


uint32_t bswap32(uint32_t i)
{
    uint32_t v = i;
    bswap(&v, 4);
}


uint64_t bswap64(uint64_t i)
{
    uint64_t v = i;
    bswap(&v, 8);
}

#endif
