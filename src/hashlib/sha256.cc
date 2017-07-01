//  :copyright: (c) 2011-2017 RHash.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  [reference] https://github.com/rhash/RHash
 */

#include <architecture.h>
#include <byteorder.h>
#include <hashlib.h>
#include <hex.h>
#include <processor.h>
#include <cstring>
#include <stdexcept>

// CONSTANTS
// ---------

static constexpr size_t SHA224_HASH_SIZE = 28;
static constexpr size_t SHA256_HASH_SIZE = 32;
static constexpr size_t SHA256_BLOCK_SIZE = 64;
static constexpr uint32_t ENCODE[] = {0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};

// OBJECTS
// -------

/** \brief SHA256 context.
 */
struct sha2_256_context
{
    uint64_t length;
    uint32_t digest_length;
    uint32_t message[16];
    uint32_t hash[8];
};

// FUNCTIONS
// ---------

#define Ch(x,y,z)  ((z) ^ ((x) & ((y) ^ (z))))
#define Maj(x,y,z) (((x) & (y)) ^ ((z) & ((x) ^ (y))))

#define ROTR32(dword, n) ((dword) >> (n) ^ ((dword) << (32 - (n))))
#define Sigma0(x) (ROTR32((x), 2) ^ ROTR32((x), 13) ^ ROTR32((x), 22))
#define Sigma1(x) (ROTR32((x), 6) ^ ROTR32((x), 11) ^ ROTR32((x), 25))
#define sigma0(x) (ROTR32((x), 7) ^ ROTR32((x), 18) ^ ((x) >>  3))
#define sigma1(x) (ROTR32((x),17) ^ ROTR32((x), 19) ^ ((x) >> 10))

#define RECALCULATE_W(W,n) (W[n] += \
    (sigma1(W[(n - 2) & 15]) + W[(n - 7) & 15] + sigma0(W[(n - 15) & 15])))

#define ROUND(a,b,c,d,e,f,g,h,k,data) { \
    uint32_t T1 = h + Sigma1(e) + Ch(e,f,g) + k + (data); \
    d += T1, h = T1 + Sigma0(a) + Maj(a,b,c); }

#define ROUND_1_16(a,b,c,d,e,f,g,h,n) \
    ROUND(a,b,c,d,e,f,g,h, ENCODE[n], W[n] = be32toh(block[n]))

#define ROUND_17_64(a,b,c,d,e,f,g,h,n) \
    ROUND(a,b,c,d,e,f,g,h, k[n], RECALCULATE_W(W, n))


/**
 *  The core transformation. Process a 512-bit block.
 */
static void sha256_process_block(uint32_t* hash, uint32_t* block)
{
    uint32_t A, B, C, D, E, F, G, H;
    uint32_t W[16];
    const uint32_t *k;
    int i;

    A = hash[0], B = hash[1], C = hash[2], D = hash[3];
    E = hash[4], F = hash[5], G = hash[6], H = hash[7];

    /* Compute SHA using alternate Method: FIPS 180-3 6.1.3 */
    ROUND_1_16(A, B, C, D, E, F, G, H, 0);
    ROUND_1_16(H, A, B, C, D, E, F, G, 1);
    ROUND_1_16(G, H, A, B, C, D, E, F, 2);
    ROUND_1_16(F, G, H, A, B, C, D, E, 3);
    ROUND_1_16(E, F, G, H, A, B, C, D, 4);
    ROUND_1_16(D, E, F, G, H, A, B, C, 5);
    ROUND_1_16(C, D, E, F, G, H, A, B, 6);
    ROUND_1_16(B, C, D, E, F, G, H, A, 7);
    ROUND_1_16(A, B, C, D, E, F, G, H, 8);
    ROUND_1_16(H, A, B, C, D, E, F, G, 9);
    ROUND_1_16(G, H, A, B, C, D, E, F, 10);
    ROUND_1_16(F, G, H, A, B, C, D, E, 11);
    ROUND_1_16(E, F, G, H, A, B, C, D, 12);
    ROUND_1_16(D, E, F, G, H, A, B, C, 13);
    ROUND_1_16(C, D, E, F, G, H, A, B, 14);
    ROUND_1_16(B, C, D, E, F, G, H, A, 15);

    for (i = 16, k = &ENCODE[16]; i < 64; i += 16, k += 16) {
        ROUND_17_64(A, B, C, D, E, F, G, H,  0);
        ROUND_17_64(H, A, B, C, D, E, F, G,  1);
        ROUND_17_64(G, H, A, B, C, D, E, F,  2);
        ROUND_17_64(F, G, H, A, B, C, D, E,  3);
        ROUND_17_64(E, F, G, H, A, B, C, D,  4);
        ROUND_17_64(D, E, F, G, H, A, B, C,  5);
        ROUND_17_64(C, D, E, F, G, H, A, B,  6);
        ROUND_17_64(B, C, D, E, F, G, H, A,  7);
        ROUND_17_64(A, B, C, D, E, F, G, H,  8);
        ROUND_17_64(H, A, B, C, D, E, F, G,  9);
        ROUND_17_64(G, H, A, B, C, D, E, F, 10);
        ROUND_17_64(F, G, H, A, B, C, D, E, 11);
        ROUND_17_64(E, F, G, H, A, B, C, D, 12);
        ROUND_17_64(D, E, F, G, H, A, B, C, 13);
        ROUND_17_64(C, D, E, F, G, H, A, B, 14);
        ROUND_17_64(B, C, D, E, F, G, H, A, 15);
    }

    hash[0] += A, hash[1] += B, hash[2] += C, hash[3] += D;
    hash[4] += E, hash[5] += F, hash[6] += G, hash[7] += H;
}


static void sha224_init(sha2_256_context* ctx)
{
    /* Initial values from FIPS 180-3. These words were obtained by taking
     * bits from 33th to 64th of the fractional parts of the square
     * roots of ninth through sixteenth prime numbers. */
    static const unsigned SHA224_H0[8] = {
        0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
        0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
    };

    ctx->length = 0;
    ctx->digest_length = SHA224_HASH_SIZE;

    memcpy(ctx->hash, SHA224_H0, sizeof(ctx->hash));
}


static void sha256_init(sha2_256_context* ctx)
{
    /* Initial values. These words were obtained by taking the first 32
     * bits of the fractional parts of the square roots of the first
     * eight prime numbers. */
    static const uint32_t SHA256_H0[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    ctx->length = 0;
    ctx->digest_length = SHA256_HASH_SIZE;

    /* initialize algorithm state */
    memcpy(ctx->hash, SHA256_H0, sizeof(ctx->hash));
}


/**
 *  Calculate message hash.
 *  Can be called repeatedly with chunks of the message to be hashed.
 */
static void sha256_update(sha2_256_context *ctx, const uint8_t *msg, size_t size)
{
    size_t index = (size_t)ctx->length & 63;
    ctx->length += size;

    // fill partial block
    if (index) {
        size_t left = SHA256_BLOCK_SIZE - index;
        memcpy((char*)ctx->message + index, msg, (size < left ? size : left));
        if (size < left){
            return;
        }

        // process partial block
        sha256_process_block(ctx->hash, (uint32_t*)ctx->message);
        msg  += left;
        size -= left;
    }
    while (size >= SHA256_BLOCK_SIZE) {
        uint32_t* aligned_message_block;
        if (IS_ALIGNED_32(msg)) {
            /* the most common case is processing of an already aligned message
            without copying it */
            aligned_message_block = (uint32_t*)msg;
        } else {
            memcpy(ctx->message, msg, SHA256_BLOCK_SIZE);
            aligned_message_block = (uint32_t*)ctx->message;
        }

        sha256_process_block(ctx->hash, aligned_message_block);
        msg  += SHA256_BLOCK_SIZE;
        size -= SHA256_BLOCK_SIZE;
    }
    if (size) {
        memcpy(ctx->message, msg, size); /* save leftovers */
    }
}


/**
 *  Store calculated hash into the given array.
 */
static void sha256_final(uint8_t* result, sha2_256_context *ctx)
{
    size_t index = ((uint32_t)ctx->length & 63) >> 2;
    uint32_t shift = ((uint32_t)ctx->length & 3) * 8;

    /* pad message and run for last block */

    /* append the byte 0x80 to the message */
    ctx->message[index]   &= le32toh(~(0xFFFFFFFF << shift));
    ctx->message[index++] ^= le32toh(0x80 << shift);

    /* if no room left in the message to store 64-bit message length */
    if (index > 14) {
        /* then fill the rest with zeros and process it */
        while (index < 16) {
            ctx->message[index++] = 0;
        }
        sha256_process_block(ctx->hash, ctx->message);
        index = 0;
    }
    while (index < 14) {
        ctx->message[index++] = 0;
    }
    ctx->message[14] = be32toh( (unsigned)(ctx->length >> 29) );
    ctx->message[15] = be32toh( (unsigned)(ctx->length << 3) );
    sha256_process_block(ctx->hash, ctx->message);

    if (result) {
        memcpy_be32toh(result, ctx->hash, ctx->digest_length);
    }

    secure_zero(&index, sizeof(index));
    secure_zero(&shift, sizeof(shift));
    secure_zero(ctx, sizeof(*ctx));
}


// OBJECTS
// -------


sha2_224_hash::sha2_224_hash()
{
    ctx = new sha2_256_context;
    sha224_init(ctx);
}


sha2_224_hash::sha2_224_hash(const void* src, size_t srclen)
{
    ctx = new sha2_256_context;
    sha224_init(ctx);
    update(src, srclen);
}


sha2_224_hash::sha2_224_hash(const secure_string_view& str)
{
    ctx = new sha2_256_context;
    sha224_init(ctx);
    update(str);
}


sha2_224_hash::~sha2_224_hash()
{
    secure_zero(ctx, sizeof(*ctx));
    delete ctx;
}


void sha2_224_hash::update(const void* src, size_t srclen)
{
    long length = srclen;
    const uint8_t* first = reinterpret_cast<const uint8_t*>(src);

    while (length > 0) {
        long shift = length > 512 ? 512 : length;
        sha256_update(ctx, first, shift);
        length -= shift;
        first += shift;
    }
}


void sha2_224_hash::update(const secure_string_view& str)
{
    update(str.data(), str.size());
}


size_t sha2_224_hash::digest(void* dst, size_t dstlen) const
{
    if (dstlen < SHA224_HASH_SIZE) {
        throw std::runtime_error("dstlen not large enough to store SHA224 digest.");
    }

    sha2_256_context copy = *ctx;
    sha256_final(reinterpret_cast<uint8_t*>(dst), &copy);
    return SHA224_HASH_SIZE;
}


size_t sha2_224_hash::hexdigest(void* dst, size_t dstlen) const
{
    if (dstlen < 2 * SHA224_HASH_SIZE) {
        throw std::runtime_error("dstlen not large enough to store SHA224 hex digest.");
    }

    int8_t* hash = new int8_t[SHA224_HASH_SIZE];
    digest(hash, SHA224_HASH_SIZE);
    return hex_i8(hash, SHA224_HASH_SIZE, dst, dstlen);
}


secure_string sha2_224_hash::digest() const
{
    static constexpr size_t size = SHA224_HASH_SIZE;

    char* hash = new char[size];
    digest(hash, size);
    secure_string output(hash, size);

    secure_zero(hash, size);
    delete[] hash;

    return output;
}


secure_string sha2_224_hash::hexdigest() const
{
    static constexpr size_t size = 2 * SHA224_HASH_SIZE;

    char* dst = new char[size];
    hexdigest(dst, size);
    secure_string output(dst, size);

    secure_zero(dst, size);
    delete[] dst;

    return output;
}


sha2_256_hash::sha2_256_hash()
{
    ctx = new sha2_256_context;
    sha256_init(ctx);
}


sha2_256_hash::sha2_256_hash(const void* src, size_t srclen)
{
    ctx = new sha2_256_context;
    sha256_init(ctx);
    update(src, srclen);
}


sha2_256_hash::sha2_256_hash(const secure_string_view& str)
{
    ctx = new sha2_256_context;
    sha256_init(ctx);
    update(str);
}


sha2_256_hash::~sha2_256_hash()
{
    secure_zero(ctx, sizeof(*ctx));
    delete ctx;
}


void sha2_256_hash::update(const void* src, size_t srclen)
{
    long length = srclen;
    const uint8_t* first = reinterpret_cast<const uint8_t*>(src);

    while (length > 0) {
        long shift = length > 512 ? 512 : length;
        sha256_update(ctx, first, shift);
        length -= shift;
        first += shift;
    }
}


void sha2_256_hash::update(const secure_string_view& str)
{
    update(str.data(), str.size());
}


size_t sha2_256_hash::digest(void* dst, size_t dstlen) const
{
    if (dstlen < SHA256_HASH_SIZE) {
        throw std::runtime_error("dstlen not large enough to store SHA256 digest.");
    }

    sha2_256_context copy = *ctx;
    sha256_final(reinterpret_cast<uint8_t*>(dst), &copy);
    return SHA256_HASH_SIZE;
}


size_t sha2_256_hash::hexdigest(void* dst, size_t dstlen) const
{
    if (dstlen < 2 * SHA256_HASH_SIZE) {
        throw std::runtime_error("dstlen not large enough to store SHA256 hex digest.");
    }

    int8_t* hash = new int8_t[SHA256_HASH_SIZE];
    digest(hash, SHA256_HASH_SIZE);
    return hex_i8(hash, SHA256_HASH_SIZE, dst, dstlen);
}


secure_string sha2_256_hash::digest() const
{
    static constexpr size_t size = SHA256_HASH_SIZE;

    char* hash = new char[size];
    digest(hash, size);
    secure_string output(hash, size);

    secure_zero(hash, size);
    delete[] hash;

    return output;
}


secure_string sha2_256_hash::hexdigest() const
{
    static constexpr size_t size = 2 * SHA256_HASH_SIZE;

    char* dst = new char[size];
    hexdigest(dst, size);
    secure_string output(dst, size);

    secure_zero(dst, size);
    delete[] dst;

    return output;
}
