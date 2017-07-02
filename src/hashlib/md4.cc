//  :copyright: (c) 2001 Alexander Peslyak <solar@openwall.com>.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  [reference] http://openwall.info/wiki/people/solar/software/public-domain-source-code/md4
 */

#include <hashlib.h>
#include <hex.h>
#include <processor.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>
#include <cstring>
#include <stdexcept>

// CONSTANTS
// ---------

static constexpr size_t MD4_HASH_SIZE = 16;

// OBJECTS
// -------

/** \brief MD4 context.
 */
struct md4_context
{
    uint32_t lo, hi;
    uint32_t a, b, c, d;
    uint8_t buffer[64];
    uint32_t block[MD4_HASH_SIZE];
};

// FUNCTIONS
// ---------

/*
 * The basic MD4 functions.
 *
 * F and G are optimized compared to their RFC 1320 definitions, with the
 * optimization for F borrowed from Colin Plumb's MD5 implementation.
 */
#define F(x, y, z)          ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)          (((x) & ((y) | (z))) | ((y) & (z)))
#define H(x, y, z)          ((x) ^ (y) ^ (z))

/*
 * The MD4 transformation for all three rounds.
 */
#define STEP(f, a, b, c, d, x, s) \
    (a) += f((b), (c), (d)) + (x); \
    (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s))));


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
 * uint32_t.  In practice, this problem may occur if these MD4 routines are
 * inlined into a calling function, or with future and dangerously advanced
 * link-time optimizations.  For the time being, keeping these MD4 routines in
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
 * counters.  There are no alignment requirements.
 */
static const void* body(md4_context *ctx, const void* data, uint32_t size)
{
    const uint8_t *ptr;
    uint32_t a, b, c, d;
    uint32_t saved_a, saved_b, saved_c, saved_d;
    uint32_t ac1 = 0x5a827999, ac2 = 0x6ed9eba1;

    ptr = (const uint8_t *)data;

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
        STEP(F, a, b, c, d, SET(0), 3)
        STEP(F, d, a, b, c, SET(1), 7)
        STEP(F, c, d, a, b, SET(2), 11)
        STEP(F, b, c, d, a, SET(3), 19)
        STEP(F, a, b, c, d, SET(4), 3)
        STEP(F, d, a, b, c, SET(5), 7)
        STEP(F, c, d, a, b, SET(6), 11)
        STEP(F, b, c, d, a, SET(7), 19)
        STEP(F, a, b, c, d, SET(8), 3)
        STEP(F, d, a, b, c, SET(9), 7)
        STEP(F, c, d, a, b, SET(10), 11)
        STEP(F, b, c, d, a, SET(11), 19)
        STEP(F, a, b, c, d, SET(12), 3)
        STEP(F, d, a, b, c, SET(13), 7)
        STEP(F, c, d, a, b, SET(14), 11)
        STEP(F, b, c, d, a, SET(15), 19)

        // ROUND 2
        STEP(G, a, b, c, d, GET(0) + ac1, 3)
        STEP(G, d, a, b, c, GET(4) + ac1, 5)
        STEP(G, c, d, a, b, GET(8) + ac1, 9)
        STEP(G, b, c, d, a, GET(12) + ac1, 13)
        STEP(G, a, b, c, d, GET(1) + ac1, 3)
        STEP(G, d, a, b, c, GET(5) + ac1, 5)
        STEP(G, c, d, a, b, GET(9) + ac1, 9)
        STEP(G, b, c, d, a, GET(13) + ac1, 13)
        STEP(G, a, b, c, d, GET(2) + ac1, 3)
        STEP(G, d, a, b, c, GET(6) + ac1, 5)
        STEP(G, c, d, a, b, GET(10) + ac1, 9)
        STEP(G, b, c, d, a, GET(14) + ac1, 13)
        STEP(G, a, b, c, d, GET(3) + ac1, 3)
        STEP(G, d, a, b, c, GET(7) + ac1, 5)
        STEP(G, c, d, a, b, GET(11) + ac1, 9)
        STEP(G, b, c, d, a, GET(15) + ac1, 13)

        // ROUND 3
        STEP(H, a, b, c, d, GET(0) + ac2, 3)
        STEP(H, d, a, b, c, GET(8) + ac2, 9)
        STEP(H, c, d, a, b, GET(4) + ac2, 11)
        STEP(H, b, c, d, a, GET(12) + ac2, 15)
        STEP(H, a, b, c, d, GET(2) + ac2, 3)
        STEP(H, d, a, b, c, GET(10) + ac2, 9)
        STEP(H, c, d, a, b, GET(6) + ac2, 11)
        STEP(H, b, c, d, a, GET(14) + ac2, 15)
        STEP(H, a, b, c, d, GET(1) + ac2, 3)
        STEP(H, d, a, b, c, GET(9) + ac2, 9)
        STEP(H, c, d, a, b, GET(5) + ac2, 11)
        STEP(H, b, c, d, a, GET(13) + ac2, 15)
        STEP(H, a, b, c, d, GET(3) + ac2, 3)
        STEP(H, d, a, b, c, GET(11) + ac2, 9)
        STEP(H, c, d, a, b, GET(7) + ac2, 11)
        STEP(H, b, c, d, a, GET(15) + ac2, 15)

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


void md4_init(md4_context *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;

    ctx->lo = 0;
    ctx->hi = 0;
}


void md4_update(void *ptr, const void *data, long size)
{
    auto* ctx = reinterpret_cast<md4_context*>(ptr);

    uint32_t saved_lo;
    unsigned long used, available;

    saved_lo = ctx->lo;
    if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo) {
        ctx->hi++;
    }
    ctx->hi += size >> 29;

    used = saved_lo & 0x3f;

    if (used) {
        available = 64 - used;

        if (size < available) {
            memcpy(&ctx->buffer[used], data, size);
            return;
        }

        memcpy(&ctx->buffer[used], data, available);
        data = (const unsigned char *)data + available;
        size -= available;
        body(ctx, ctx->buffer, 64);
    }

    if (size >= 64) {
        data = body(ctx, data, size & ~(unsigned long)0x3f);
        size &= 0x3f;
    }

    memcpy(ctx->buffer, data, size);
}

#define OUT(dst, src) \
    (dst)[0] = (unsigned char)(src); \
    (dst)[1] = (unsigned char)((src) >> 8); \
    (dst)[2] = (unsigned char)((src) >> 16); \
    (dst)[3] = (unsigned char)((src) >> 24);


void md4_final(void *ptr, void* buf)
{
    auto* ctx = (md4_context*) ptr;
    auto* result = (uint8_t*) buf;

    // unsigned char
    unsigned long used, available;

    used = ctx->lo & 0x3f;
    ctx->buffer[used++] = 0x80;
    available = 64 - used;

    if (available < 8) {
        secure_zero(&ctx->buffer[used], available);
        body(ctx, ctx->buffer, 64);
        used = 0;
        available = 64;
    }

    secure_zero(&ctx->buffer[used], available - 8);

    ctx->lo <<= 3;
    OUT(&ctx->buffer[56], ctx->lo)
    OUT(&ctx->buffer[60], ctx->hi)

    body(ctx, ctx->buffer, 64);

    OUT(&result[0], ctx->a)
    OUT(&result[4], ctx->b)
    OUT(&result[8], ctx->c)
    OUT(&result[12], ctx->d)

    secure_zero(&used, sizeof(used));
    secure_zero(&available, sizeof(available));
    secure_zero(ctx, sizeof(*ctx));
}

// OBJECTS
// -------


md4_hash::md4_hash()
{
    ctx = new md4_context;
    md4_init(ctx);
}


md4_hash::md4_hash(const void* src, size_t srclen)
{
    ctx = new md4_context;
    md4_init(ctx);
    update(src, srclen);
}


md4_hash::md4_hash(const secure_string_view& str)
{
    ctx = new md4_context;
    md4_init(ctx);
    update(str);
}


md4_hash::~md4_hash()
{
    secure_zero(ctx, sizeof(*ctx));
    delete ctx;
}


void md4_hash::update(const void* src, size_t srclen)
{
    hash_update(ctx, src, srclen, md4_update);
}


void md4_hash::update(const secure_string_view& str)
{
    update(str.data(), str.size());
}


size_t md4_hash::digest(void* dst, size_t dstlen) const
{
    md4_context copy = *ctx;
    return hash_digest(&copy, dst, dstlen, MD4_HASH_SIZE, md4_final);
}


size_t md4_hash::hexdigest(void* dst, size_t dstlen) const
{
    md4_context copy = *ctx;
    return hash_hexdigest(&copy, dst, dstlen, MD4_HASH_SIZE, md4_final);
}


secure_string md4_hash::digest() const
{
    md4_context copy = *ctx;
    return hash_digest(&copy, MD4_HASH_SIZE, md4_final);
}


secure_string md4_hash::hexdigest() const
{
    md4_context copy = *ctx;
    return hash_hexdigest(&copy, MD4_HASH_SIZE, md4_final);
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
#undef OUT

#include <warnings/pop.h>
