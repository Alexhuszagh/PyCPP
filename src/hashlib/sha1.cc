//  :copyright: (c) Steve Reid <steve@edmweb.com>.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "architecture.h"
#include "hashlib.h"
#include "hex.h"

#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>
#include <cstdio>
#include <cstring>
#include <stdexcept>

// CONSTANTS
// ---------

constexpr size_t SHA1_HASH_SIZE = 20;

// UNIONS
// ------

union Char64Union
{
    uint8_t c[64];
    uint32_t l[16];
};

// OBJECTS
// -------

/** \brief SHA1 context.
 */
struct sha1_context
{
    uint32_t state[5];
    uint32_t count[2];
    unsigned char buffer[64];
};

// FUNCTIONS
// ---------

#define ROL(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

#if BYTE_ORDER == LITTLE_ENDIAN
#   define BLK0(i) (block->l[i] = (ROL(block->l[i],24)&0xFF00FF00) \
    |(ROL(block->l[i],8)&0x00FF00FF))
#elif BYTE_ORDER == BIG_ENDIAN
#   define BLK0(i) block->l[i]
#endif

#define BLK(i) (block->l[i&15] = ROL(block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15]^block->l[i&15],1))

/**
 *  \brief (R0+R1), R2, R3, R4 are the different operations used in SHA1.
 */
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+BLK0(i)+0x5A827999+ROL(v,5);w=ROL(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+BLK(i)+0x5A827999+ROL(v,5);w=ROL(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+BLK(i)+0x6ED9EBA1+ROL(v,5);w=ROL(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+BLK(i)+0x8F1BBCDC+ROL(v,5);w=ROL(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+BLK(i)+0xCA62C1D6+ROL(v,5);w=ROL(w,30);


/*
 *  Hash a single 512-bit block. This is the core of the algorithm.
 */
void sha1_transform(uint32_t state[5], const uint8_t buffer[64])
{
    uint32_t a, b, c, d, e;
    Char64Union block[1];  /* use array to appear as a pointer */
    memcpy(block, buffer, 64);

    /* Copy context->state[] to working vars */
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    // ROUND 1
    R0(a,b,c,d,e, 0);
    R0(e,a,b,c,d, 1);
    R0(d,e,a,b,c, 2);
    R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4);
    R0(a,b,c,d,e, 5);
    R0(e,a,b,c,d, 6);
    R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8);
    R0(b,c,d,e,a, 9);
    R0(a,b,c,d,e,10);
    R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12);
    R0(c,d,e,a,b,13);
    R0(b,c,d,e,a,14);
    R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16);
    R1(d,e,a,b,c,17);
    R1(c,d,e,a,b,18);
    R1(b,c,d,e,a,19);

    // ROUND 2
    R2(a,b,c,d,e,20);
    R2(e,a,b,c,d,21);
    R2(d,e,a,b,c,22);
    R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24);
    R2(a,b,c,d,e,25);
    R2(e,a,b,c,d,26);
    R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28);
    R2(b,c,d,e,a,29);
    R2(a,b,c,d,e,30);
    R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32);
    R2(c,d,e,a,b,33);
    R2(b,c,d,e,a,34);
    R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36);
    R2(d,e,a,b,c,37);
    R2(c,d,e,a,b,38);
    R2(b,c,d,e,a,39);

    // ROUND 3
    R3(a,b,c,d,e,40);
    R3(e,a,b,c,d,41);
    R3(d,e,a,b,c,42);
    R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44);
    R3(a,b,c,d,e,45);
    R3(e,a,b,c,d,46);
    R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48);
    R3(b,c,d,e,a,49);
    R3(a,b,c,d,e,50);
    R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52);
    R3(c,d,e,a,b,53);
    R3(b,c,d,e,a,54);
    R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56);
    R3(d,e,a,b,c,57);
    R3(c,d,e,a,b,58);
    R3(b,c,d,e,a,59);

    // ROUND 4
    R4(a,b,c,d,e,60);
    R4(e,a,b,c,d,61);
    R4(d,e,a,b,c,62);
    R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64);
    R4(a,b,c,d,e,65);
    R4(e,a,b,c,d,66);
    R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68);
    R4(b,c,d,e,a,69);
    R4(a,b,c,d,e,70);
    R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72);
    R4(c,d,e,a,b,73);
    R4(b,c,d,e,a,74);
    R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76);
    R4(d,e,a,b,c,77);
    R4(c,d,e,a,b,78);
    R4(b,c,d,e,a,79);

    /* Add the working vars back into context.state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;

    /* Wipe variables */
    a = b = c = d = e = 0;
    memset(block, '\0', sizeof(block));
}


/**
 *  \brief Initialize SHA1 context.
 */
void sha1_init(sha1_context* ctx)
{
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xc3d2e1f0;
    ctx->count[0] = ctx->count[1] = 0;
}


/**
 *  \brief Update hash with data.
 */
void sha1_update(sha1_context* ctx, const uint8_t* data, uint32_t len)
{
    uint32_t i, j;

    j = ctx->count[0];
    if ((ctx->count[0] += len << 3) < j)
        ctx->count[1]++;
    ctx->count[1] += (len>>29);
    j = (j >> 3) & 63;
    if ((j + len) > 63) {
        memcpy(&ctx->buffer[j], data, (i = 64-j));
        sha1_transform(ctx->state, ctx->buffer);
        for ( ; i + 63 < len; i += 64) {
            sha1_transform(ctx->state, &data[i]);
        }
        j = 0;
    }
    else i = 0;
    memcpy(&ctx->buffer[j], &data[i], len - i);
}


/**
 *  \brief Add padding and return the message digest.
 */
void sha1_final(uint8_t digest[20], sha1_context* ctx)
{
    unsigned i;
    uint8_t finalcount[8];
    uint8_t c;

    for (i = 0; i < 8; i++) {
        finalcount[i] = (uint8_t)((ctx->count[(i >= 4 ? 0 : 1)]
         >> ((3-(i & 3)) * 8) ) & 255);  /* Endian independent */
    }

    c = 0200;
    sha1_update(ctx, &c, 1);
    while ((ctx->count[0] & 504) != 448) {
    c = 0000;
        sha1_update(ctx, &c, 1);
    }
    sha1_update(ctx, finalcount, 8);  /* Should cause a sha1_transform() */
    for (i = 0; i < 20; i++) {
        digest[i] = (uint8_t)
         ((ctx->state[i>>2] >> ((3-(i & 3)) * 8) ) & 255);
    }

    /* Wipe variables */
    memset(ctx, '\0', sizeof(*ctx));
    memset(&finalcount, '\0', sizeof(finalcount));
}


// OBJECTS
// -------


sha1_hash::sha1_hash()
{
    ctx = new sha1_context;
    sha1_init(ctx);
}


sha1_hash::sha1_hash(const void* src, size_t srclen)
{
    ctx = new sha1_context;
    sha1_init(ctx);
    update(src, srclen);
}


sha1_hash::sha1_hash(const string_view& str)
{
    ctx = new sha1_context;
    sha1_init(ctx);
    update(str);
}


sha1_hash::~sha1_hash()
{
    delete ctx;
}


void sha1_hash::update(const void* src, size_t srclen)
{
    long length = srclen;
    const uint8_t* first = reinterpret_cast<const uint8_t*>(src);

    while (length > 0) {
        long shift = length > 512 ? 512 : length;
        sha1_update(ctx, first, shift);
        length -= shift;
        first += shift;
    }
}


void sha1_hash::update(const string_view& str)
{
    update(str.data(), str.size());
}


size_t sha1_hash::digest(void* dst, size_t dstlen) const
{
    if (dstlen < SHA1_HASH_SIZE) {
        throw std::runtime_error("dstlen not large enough to store SHA1 digest.");
    }

    sha1_context copy = *ctx;
    sha1_final(reinterpret_cast<uint8_t*>(dst), &copy);
    return SHA1_HASH_SIZE;
}


size_t sha1_hash::hexdigest(void* dst, size_t dstlen) const
{
    if (dstlen < 2 * SHA1_HASH_SIZE) {
        throw std::runtime_error("dstlen not large enough to store SHA1 hex digest.");
    }

    int8_t* hash = new int8_t[SHA1_HASH_SIZE];
    digest(hash, SHA1_HASH_SIZE);
    return hex_i8(hash, SHA1_HASH_SIZE, dst, dstlen);
}


std::string sha1_hash::digest() const
{
    char* hash = new char[SHA1_HASH_SIZE];
    digest(hash, SHA1_HASH_SIZE);

    std::string output(hash, SHA1_HASH_SIZE);
    delete[] hash;
    return output;
}


std::string sha1_hash::hexdigest() const
{
    return hex_i8(digest());
}

// CLEANUP
// -------

#undef ROL
#undef BLK0
#undef BLK
#undef R0
#undef R1
#undef R2
#undef R3
#undef R4

#include <warnings/pop.h>
