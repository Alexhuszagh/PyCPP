//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Public Domain/MIT, see licenses/mit.md for more details.

#include <pycpp/preprocessor/byteorder.h>
#include <pycpp/stl/stdexcept.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

// HELPERS
// -------


/**
 *  \brief Byte-swap value in-place.
 */
void bswap_impl(void* buf, int width) noexcept
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


void bswap(void* buf, int width) noexcept
{
    bswap_impl(buf, width);
}


void bswap(void* dst, void* src, int width) noexcept
{
    if (src != dst) {
        memcpy(dst, src, width);
    }
    bswap(dst, width);
}


void memcpy_bswap16(void* dst, void* src, size_t bytes) noexcept
{
    // bounds check
    assert(bytes % 2 == 0 && "Trailing data for memcpy_bswap16.");

    // copy bytes
    uint16_t* dst_ = reinterpret_cast<uint16_t*>(dst);
    uint16_t* src_ = reinterpret_cast<uint16_t*>(src);
    for (size_t i = 0; i < bytes / 2; ++i) {
        dst_[i] = bswap16(src_[i]);
    }
}


void memcpy_bswap32(void* dst, void* src, size_t bytes) noexcept
{
    // bounds check
    assert(bytes % 4 == 0 && "Trailing data for memcpy_bswap32.");

    // copy bytes
    uint32_t* dst_ = reinterpret_cast<uint32_t*>(dst);
    uint32_t* src_ = reinterpret_cast<uint32_t*>(src);
    for (size_t i = 0; i < bytes / 4; ++i) {
        dst_[i] = bswap32(src_[i]);
    }
}


void memcpy_bswap64(void* dst, void* src, size_t bytes) noexcept
{
    // bounds check
    assert(bytes % 8 == 0 && "Trailing data for memcpy_bswap64.");

    // copy bytes
    uint64_t* dst_ = reinterpret_cast<uint64_t*>(dst);
    uint64_t* src_ = reinterpret_cast<uint64_t*>(src);
    for (size_t i = 0; i < bytes / 8; ++i) {
        dst_[i] = bswap64(src_[i]);
    }
}


void memcpy_bswap(void* dst, void* src, size_t bytes, int width) noexcept
{
    // bounds check
    assert(bytes % width == 0 && "Trailing data for memcpy_bswap.");

    // copy bytes
    for (size_t i = 0; i < bytes; i += width) {
        bswap(dst, src, width);
    }
}


#if defined(NEED_BSWAPXX)

uint16_t bswap16(uint16_t i) noexcept
{
    uint16_t v = i;
    bswap(&v, 2);
    return v;
}


uint32_t bswap32(uint32_t i) noexcept
{
    uint32_t v = i;
    bswap(&v, 4);
    return v;
}


uint64_t bswap64(uint64_t i) noexcept
{
    uint64_t v = i;
    bswap(&v, 8);
    return v;
}

#endif
