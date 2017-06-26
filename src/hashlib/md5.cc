//  :copyright: (c) 2001 Alexander Peslyak <solar@openwall.com>.
//  :copyright: (c) 2016 The Regents of the University of California.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup crosscxx
 *  \brief MD5 hash function.
 *
 *  [reference] http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 */

#include "hashlib.h"
#include "hex.h"
#include "processor.h"

#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>
#include <cstring>
#include <stdexcept>

// CONSTANTS
// ---------

static constexpr size_t MD5_HASH_SIZE = 16;

// OBJECTS
// -------

/** \brief MD5 context.
 */
struct MD5_CTX
{
    uint32_t lo, hi;
    uint32_t a, b, c, d;
    uint8_t buffer[64];
    uint32_t block[MD5_HASH_SIZE];
};

// FUNCTIONS
// ---------

/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in Colin Plumb's
 * implementation.
 */
#define F(x, y, z)          ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)          ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)          (((x) ^ (y)) ^ (z))
#define H2(x, y, z)         ((x) ^ ((y) ^ (z)))
#define I(x, y, z)          ((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */
#define STEP(f, a, b, c, d, x, t, s) \
    (a) += f((b), (c), (d)) + (x) + (t); \
    (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
    (a) += (b);

/*
 * SET reads 4 input bytes in little-endian byte order and stores them in a
 * properly aligned word in host byte order.
 *
 * The check for little-endian architectures that tolerate unaligned memory
 * accesses is just an optimization.  Nothing will break if it fails to detect
 * a suitable architecture.
 *
 * Unfortunately, this optimization may be a C strict aliasing rules violation
 * if the caller's data buffer has effective type that cannot be aliased by
 * uint32_t.  In practice, this problem may occur if these MD5 routines are
 * inlined into a calling function, or with future and dangerously advanced
 * link-time optimizations.  For the time being, keeping these MD5 routines in
 * their own translation unit avoids the problem.
 */
#if defined(HAVE_X86) || defined(HAVE_VAX)
#   define SET(n) (*(uint32_t *)&ptr[(n) * 4])
#   define GET(n) SET(n)
#else
#   define SET(n) \
        (ctx->block[(n)] = \
        (uint32_t)ptr[(n) * 4] | \
        ((uint32_t)ptr[(n) * 4 + 1] << 8) | \
        ((uint32_t)ptr[(n) * 4 + 2] << 16) | \
        ((uint32_t)ptr[(n) * 4 + 3] << 24))
#   define GET(n) (ctx->block[(n)])
#endif


/*
 * This processes one or more 64-byte data blocks, but does NOT update the bit
 * counters. There are no alignment requirements.
 */
static const void * body(MD5_CTX* ctx, const void *data, uint32_t size)
{
    const uint8_t* ptr;
    uint32_t a, b, c, d;
    uint32_t saved_a, saved_b, saved_c, saved_d;

    ptr = (const uint8_t*)data;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    do {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;

        // ROUND 1
        STEP(F, a, b, c, d, SET(0), 0xd76aa478, 7)
        STEP(F, d, a, b, c, SET(1), 0xe8c7b756, 12)
        STEP(F, c, d, a, b, SET(2), 0x242070db, 17)
        STEP(F, b, c, d, a, SET(3), 0xc1bdceee, 22)
        STEP(F, a, b, c, d, SET(4), 0xf57c0faf, 7)
        STEP(F, d, a, b, c, SET(5), 0x4787c62a, 12)
        STEP(F, c, d, a, b, SET(6), 0xa8304613, 17)
        STEP(F, b, c, d, a, SET(7), 0xfd469501, 22)
        STEP(F, a, b, c, d, SET(8), 0x698098d8, 7)
        STEP(F, d, a, b, c, SET(9), 0x8b44f7af, 12)
        STEP(F, c, d, a, b, SET(10), 0xffff5bb1, 17)
        STEP(F, b, c, d, a, SET(11), 0x895cd7be, 22)
        STEP(F, a, b, c, d, SET(12), 0x6b901122, 7)
        STEP(F, d, a, b, c, SET(13), 0xfd987193, 12)
        STEP(F, c, d, a, b, SET(14), 0xa679438e, 17)
        STEP(F, b, c, d, a, SET(15), 0x49b40821, 22)

        // ROUND 2
        STEP(G, a, b, c, d, GET(1), 0xf61e2562, 5)
        STEP(G, d, a, b, c, GET(6), 0xc040b340, 9)
        STEP(G, c, d, a, b, GET(11), 0x265e5a51, 14)
        STEP(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)
        STEP(G, a, b, c, d, GET(5), 0xd62f105d, 5)
        STEP(G, d, a, b, c, GET(10), 0x02441453, 9)
        STEP(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
        STEP(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)
        STEP(G, a, b, c, d, GET(9), 0x21e1cde6, 5)
        STEP(G, d, a, b, c, GET(14), 0xc33707d6, 9)
        STEP(G, c, d, a, b, GET(3), 0xf4d50d87, 14)
        STEP(G, b, c, d, a, GET(8), 0x455a14ed, 20)
        STEP(G, a, b, c, d, GET(13), 0xa9e3e905, 5)
        STEP(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)
        STEP(G, c, d, a, b, GET(7), 0x676f02d9, 14)
        STEP(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)

        // ROUND 3
        STEP(H, a, b, c, d, GET(5), 0xfffa3942, 4)
        STEP(H2, d, a, b, c, GET(8), 0x8771f681, 11)
        STEP(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
        STEP(H2, b, c, d, a, GET(14), 0xfde5380c, 23)
        STEP(H, a, b, c, d, GET(1), 0xa4beea44, 4)
        STEP(H2, d, a, b, c, GET(4), 0x4bdecfa9, 11)
        STEP(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)
        STEP(H2, b, c, d, a, GET(10), 0xbebfbc70, 23)
        STEP(H, a, b, c, d, GET(13), 0x289b7ec6, 4)
        STEP(H2, d, a, b, c, GET(0), 0xeaa127fa, 11)
        STEP(H, c, d, a, b, GET(3), 0xd4ef3085, 16)
        STEP(H2, b, c, d, a, GET(6), 0x04881d05, 23)
        STEP(H, a, b, c, d, GET(9), 0xd9d4d039, 4)
        STEP(H2, d, a, b, c, GET(12), 0xe6db99e5, 11)
        STEP(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
        STEP(H2, b, c, d, a, GET(2), 0xc4ac5665, 23)

        // ROUND 4
        STEP(I, a, b, c, d, GET(0), 0xf4292244, 6)
        STEP(I, d, a, b, c, GET(7), 0x432aff97, 10)
        STEP(I, c, d, a, b, GET(14), 0xab9423a7, 15)
        STEP(I, b, c, d, a, GET(5), 0xfc93a039, 21)
        STEP(I, a, b, c, d, GET(12), 0x655b59c3, 6)
        STEP(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)
        STEP(I, c, d, a, b, GET(10), 0xffeff47d, 15)
        STEP(I, b, c, d, a, GET(1), 0x85845dd1, 21)
        STEP(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)
        STEP(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
        STEP(I, c, d, a, b, GET(6), 0xa3014314, 15)
        STEP(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
        STEP(I, a, b, c, d, GET(4), 0xf7537e82, 6)
        STEP(I, d, a, b, c, GET(11), 0xbd3af235, 10)
        STEP(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)
        STEP(I, b, c, d, a, GET(9), 0xeb86d391, 21)

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;

        ptr += 64;
    } while (size -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

    return ptr;
}


/** \brief Initialize MD5 context.
 */
void md5_init(MD5_CTX* ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;

    ctx->lo = 0;
    ctx->hi = 0;
}


/** \brief Update hash with data.
 */
void md5_update(MD5_CTX* ctx, const void* data, uint32_t size)
{
    uint32_t saved_lo;
    uint32_t used, available;

    saved_lo = ctx->lo;
    if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
        ctx->hi++;
    ctx->hi += size >> 29;

    used = saved_lo & 0x3f;

    if (used) {
        available = 64 - used;

        if (size < available) {
            memcpy(&ctx->buffer[used], data, size);
            return;
        }

        memcpy(&ctx->buffer[used], data, available);
        data = (const uint8_t*)data + available;
        size -= available;
        body(ctx, ctx->buffer, 64);
    }

    if (size >= 64) {
        data = body(ctx, data, size & ~(uint32_t)0x3f);
        size &= 0x3f;
    }

    memcpy(ctx->buffer, data, size);
}

#define MD5_OUT(dst, src) \
    (dst)[0] = (uint8_t)(src); \
    (dst)[1] = (uint8_t)((src) >> 8); \
    (dst)[2] = (uint8_t)((src) >> 16); \
    (dst)[3] = (uint8_t)((src) >> 24);


/** \brief Add padding and return the message digest.
 */
void md5_final(uint8_t* result, MD5_CTX* ctx)
{
    uint32_t used, available;

    used = ctx->lo & 0x3f;

    ctx->buffer[used++] = 0x80;

    available = 64 - used;

    if (available < 8) {
        memset(&ctx->buffer[used], 0, available);
        body(ctx, ctx->buffer, 64);
        used = 0;
        available = 64;
    }

    memset(&ctx->buffer[used], 0, available - 8);

    ctx->lo <<= 3;
    MD5_OUT(&ctx->buffer[56], ctx->lo)
    MD5_OUT(&ctx->buffer[60], ctx->hi)

    body(ctx, ctx->buffer, 64);

    MD5_OUT(&result[0], ctx->a)
    MD5_OUT(&result[4], ctx->b)
    MD5_OUT(&result[8], ctx->c)
    MD5_OUT(&result[12], ctx->d)

    memset(ctx, 0, sizeof(*ctx));
}


size_t md5_hash(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    if (dstlen < MD5_HASH_SIZE) {
        throw std::runtime_error("dstlen not large enough to store MD5 hash.");
    }

    MD5_CTX context;
    long length = srclen;
    const char* first = reinterpret_cast<const char*>(src);

    md5_init(&context);
    while (length > 0) {
        long shift = length > 512 ? 512 : length;
        md5_update(&context, first, shift);
        length -= shift;
        first += shift;
    }
    md5_final(reinterpret_cast<uint8_t*>(dst), &context);

    return MD5_HASH_SIZE;
}


std::string md5_hash(const std::string &str)
{
    char* hash = new char[MD5_HASH_SIZE];
    md5_hash(str.data(), str.size(), hash, MD5_HASH_SIZE);

    std::string output(hash, MD5_HASH_SIZE);
    delete[] hash;
    return output;
}


size_t md5_digest(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    char* hash = new char[MD5_HASH_SIZE];
    md5_hash(src, srclen, hash, MD5_HASH_SIZE);
    auto size = hex_i8(hash, MD5_HASH_SIZE, dst, dstlen);
    delete[] hash;
    return size;
}


std::string md5_digest(const std::string& str)
{
    return hex_i8(md5_hash(str));
}

// CLEANUP
// -------

#undef F
#undef G
#undef H
#undef H2
#undef I
#undef STEP
#undef SET
#undef GET
#undef MD5_OUT

#include <warnings/pop.h>
