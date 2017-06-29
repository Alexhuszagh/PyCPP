//  :copyright: (c) 2011-2017 RHash.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  [reference] https://github.com/rhash/RHash
 */

#include "architecture.h"
#include "byteorder.h"
#include "hashlib.h"
#include "hex.h"

#include <cstring>
#include <stdexcept>

// CONSTANTS
// ---------

static constexpr size_t SHA384_HASH_SIZE = 48;
static constexpr size_t SHA512_HASH_SIZE = 64;
static constexpr size_t SHA512_BLOCK_SIZE = 128;

/*
 *  SHA-384 and SHA-512 constants for 80 rounds. These qwords represent
 *  the first 64 bits of the fractional parts of the cube
 *  roots of the first 80 prime numbers.
 */
static const uint64_t ENCODE[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL,
    0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL,
    0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
    0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL,
    0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL,
    0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL,
    0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL,
    0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
    0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL,
    0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL,
    0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL,
    0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL,
    0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
    0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

// OBJECTS
// -------

/** \brief Sha512 context.
 */
struct sha2_512_context
{
    uint64_t message[16];
    uint64_t length;
    uint64_t hash[8];
    unsigned digest_length;
};

// FUNCTIONS
// ---------

#define Ch(x,y,z) ((z) ^ ((x) & ((y) ^ (z))))
#define Maj(x,y,z) (((x) & (y)) ^ ((z) & ((x) ^ (y))))

#define ROTR64(qword, n) ((qword) >> (n) ^ ((qword) << (64 - (n))))
#define Sigma0(x) (ROTR64((x), 28) ^ ROTR64((x), 34) ^ ROTR64((x), 39))
#define Sigma1(x) (ROTR64((x), 14) ^ ROTR64((x), 18) ^ ROTR64((x), 41))
#define sigma0(x) (ROTR64((x),  1) ^ ROTR64((x),  8) ^ ((x) >> 7))
#define sigma1(x) (ROTR64((x), 19) ^ ROTR64((x), 61) ^ ((x) >> 6))

#define RECALCULATE_W(W,n) (W[n] += \
    (sigma1(W[(n - 2) & 15]) + W[(n - 7) & 15] + sigma0(W[(n - 15) & 15])))

#define ROUND(a,b,c,d,e,f,g,h,k,data) { \
    uint64_t T1 = h + Sigma1(e) + Ch(e,f,g) + k + (data); \
    d += T1, h = T1 + Sigma0(a) + Maj(a,b,c); }

#define ROUND_1_16(a,b,c,d,e,f,g,h,n) \
    ROUND(a,b,c,d,e,f,g,h, ENCODE[n], W[n] = be64toh(block[n]))

#define ROUND_17_80(a,b,c,d,e,f,g,h,n) \
    ROUND(a,b,c,d,e,f,g,h, k[n], RECALCULATE_W(W, n))


/**
 *  The core transformation. Process a 512-bit block.
 */
static void sha512_process_block(uint64_t* hash, uint64_t* block)
{
    uint64_t A, B, C, D, E, F, G, H;
    uint64_t W[16];
    const uint64_t *k;
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

    for (i = 16, k = &ENCODE[16]; i < 80; i += 16, k += 16) {
        ROUND_17_80(A, B, C, D, E, F, G, H,  0);
        ROUND_17_80(H, A, B, C, D, E, F, G,  1);
        ROUND_17_80(G, H, A, B, C, D, E, F,  2);
        ROUND_17_80(F, G, H, A, B, C, D, E,  3);
        ROUND_17_80(E, F, G, H, A, B, C, D,  4);
        ROUND_17_80(D, E, F, G, H, A, B, C,  5);
        ROUND_17_80(C, D, E, F, G, H, A, B,  6);
        ROUND_17_80(B, C, D, E, F, G, H, A,  7);
        ROUND_17_80(A, B, C, D, E, F, G, H,  8);
        ROUND_17_80(H, A, B, C, D, E, F, G,  9);
        ROUND_17_80(G, H, A, B, C, D, E, F, 10);
        ROUND_17_80(F, G, H, A, B, C, D, E, 11);
        ROUND_17_80(E, F, G, H, A, B, C, D, 12);
        ROUND_17_80(D, E, F, G, H, A, B, C, 13);
        ROUND_17_80(C, D, E, F, G, H, A, B, 14);
        ROUND_17_80(B, C, D, E, F, G, H, A, 15);
    }

    hash[0] += A, hash[1] += B, hash[2] += C, hash[3] += D;
    hash[4] += E, hash[5] += F, hash[6] += G, hash[7] += H;
}


static void sha512_init(sha2_512_context *ctx)
{
    /*
     *  Initial values. These words were obtained by taking the first 32
     *  bits of the fractional parts of the square roots of the first
     *  eight prime numbers.
     */
    static const uint64_t SHA512_H0[8] = {
        0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL,
        0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
        0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
    };

    ctx->length = 0;
    ctx->digest_length = SHA512_HASH_SIZE;

    /* initialize algorithm state */
    memcpy(ctx->hash, SHA512_H0, sizeof(ctx->hash));
}


static void sha384_init(sha2_512_context *ctx)
{
    /**
     *  Initial values from FIPS 180-3. These words were obtained by taking
     *  the first sixty-four bits of the fractional parts of the square
     *  roots of ninth through sixteenth prime numbers.
     */
    static const uint64_t SHA384_H0[8] = {
        0xcbbb9d5dc1059ed8ULL, 0x629a292a367cd507ULL, 0x9159015a3070dd17ULL,
        0x152fecd8f70e5939ULL, 0x67332667ffc00b31ULL, 0x8eb44a8768581511ULL,
        0xdb0c2e0d64f98fa7ULL, 0x47b5481dbefa4fa4ULL
    };

    ctx->length = 0;
    ctx->digest_length = SHA384_HASH_SIZE;

    memcpy(ctx->hash, SHA384_H0, sizeof(ctx->hash));
}


void sha512_update(sha2_512_context *ctx, const uint8_t *msg, size_t size)
{
    size_t index = (size_t)ctx->length & 127;
    ctx->length += size;

    // fill partial block
    if (index) {
        size_t left = SHA512_BLOCK_SIZE - index;
        memcpy((char*)ctx->message + index, msg, (size < left ? size : left));
        if (size < left) {
            return;
        }

        // process partial block
        sha512_process_block(ctx->hash, ctx->message);
        msg  += left;
        size -= left;
    }
    while (size >= SHA512_BLOCK_SIZE) {
        uint64_t* aligned_message_block;
        if (IS_ALIGNED_64(msg)) {
            // the most common case is processing of an already
            // aligned message without copying it
            aligned_message_block = (uint64_t*)msg;
        } else {
            memcpy(ctx->message, msg, SHA512_BLOCK_SIZE);
            aligned_message_block = ctx->message;
        }

        sha512_process_block(ctx->hash, aligned_message_block);
        msg  += SHA512_BLOCK_SIZE;
        size -= SHA512_BLOCK_SIZE;
    }
    if (size) {
        // save leftovers
        memcpy(ctx->message, msg, size);
    }
}


/**
 *  Store calculated hash into the given array.
 */
void sha512_final(uint8_t* result, sha2_512_context *ctx)
{
    size_t index = ((unsigned)ctx->length & 127) >> 3;
    unsigned shift = ((unsigned)ctx->length & 7) * 8;

    // pad message and process the last block

    // append the byte 0x80 to the message
    ctx->message[index]   &= le64toh( ~(0xFFFFFFFFFFFFFFFFULL << shift) );
    ctx->message[index++] ^= le64toh( 0x80ULL << shift );

    /* if no room left in the message to store 128-bit message length */
    if (index >= 15) {
        if (index == 15) ctx->message[index] = 0;
        sha512_process_block(ctx->hash, ctx->message);
        index = 0;
    }
    while (index < 15) {
        ctx->message[index++] = 0;
    }
    ctx->message[15] = be64toh(ctx->length << 3);
    sha512_process_block(ctx->hash, ctx->message);

    if (result) {
        memcpy_be64toh(result, ctx->hash, ctx->digest_length);
    }

    secure_zero(ctx, sizeof(*ctx));
}


// OBJECTS
// -------


sha2_384_hash::sha2_384_hash()
{
    ctx = new sha2_512_context;
    sha384_init(ctx);
}


sha2_384_hash::sha2_384_hash(const void* src, size_t srclen)
{
    ctx = new sha2_512_context;
    sha384_init(ctx);
    update(src, srclen);
}


sha2_384_hash::sha2_384_hash(const string_view& str)
{
    ctx = new sha2_512_context;
    sha384_init(ctx);
    update(str);
}


sha2_384_hash::~sha2_384_hash()
{
    secure_zero(ctx, sizeof(*ctx));
    delete ctx;
}


void sha2_384_hash::update(const void* src, size_t srclen)
{
    long length = srclen;
    const uint8_t* first = reinterpret_cast<const uint8_t*>(src);

    while (length > 0) {
        long shift = length > 512 ? 512 : length;
        sha512_update(ctx, first, shift);
        length -= shift;
        first += shift;
    }
}


void sha2_384_hash::update(const string_view& str)
{
    update(str.data(), str.size());
}


size_t sha2_384_hash::digest(void* dst, size_t dstlen) const
{
    if (dstlen < SHA384_HASH_SIZE) {
        throw std::runtime_error("dstlen not large enough to store SHA384 digest.");
    }

    sha2_512_context copy = *ctx;
    sha512_final(reinterpret_cast<uint8_t*>(dst), &copy);
    return SHA384_HASH_SIZE;
}


size_t sha2_384_hash::hexdigest(void* dst, size_t dstlen) const
{
    if (dstlen < 2 * SHA384_HASH_SIZE) {
        throw std::runtime_error("dstlen not large enough to store SHA384 hex digest.");
    }

    int8_t* hash = new int8_t[SHA384_HASH_SIZE];
    digest(hash, SHA384_HASH_SIZE);
    return hex_i8(hash, SHA384_HASH_SIZE, dst, dstlen);
}


std::string sha2_384_hash::digest() const
{
    char* hash = new char[SHA384_HASH_SIZE];
    digest(hash, SHA384_HASH_SIZE);

    std::string output(hash, SHA384_HASH_SIZE);
    delete[] hash;
    return output;
}


std::string sha2_384_hash::hexdigest() const
{
    return hex_i8(digest());
}


sha2_512_hash::sha2_512_hash()
{
    ctx = new sha2_512_context;
    sha512_init(ctx);
}


sha2_512_hash::sha2_512_hash(const void* src, size_t srclen)
{
    ctx = new sha2_512_context;
    sha512_init(ctx);
    update(src, srclen);
}


sha2_512_hash::sha2_512_hash(const string_view& str)
{
    ctx = new sha2_512_context;
    sha512_init(ctx);
    update(str);
}


sha2_512_hash::~sha2_512_hash()
{
    secure_zero(ctx, sizeof(*ctx));
    delete ctx;
}


void sha2_512_hash::update(const void* src, size_t srclen)
{
    long length = srclen;
    const uint8_t* first = reinterpret_cast<const uint8_t*>(src);

    while (length > 0) {
        long shift = length > 512 ? 512 : length;
        sha512_update(ctx, first, shift);
        length -= shift;
        first += shift;
    }
}


void sha2_512_hash::update(const string_view& str)
{
    update(str.data(), str.size());
}


size_t sha2_512_hash::digest(void* dst, size_t dstlen) const
{
    if (dstlen < SHA512_HASH_SIZE) {
        throw std::runtime_error("dstlen not large enough to store SHA512 digest.");
    }

    sha2_512_context copy = *ctx;
    sha512_final(reinterpret_cast<uint8_t*>(dst), &copy);
    return SHA512_HASH_SIZE;
}


size_t sha2_512_hash::hexdigest(void* dst, size_t dstlen) const
{
    if (dstlen < 2 * SHA512_HASH_SIZE) {
        throw std::runtime_error("dstlen not large enough to store SHA512 hex digest.");
    }

    int8_t* hash = new int8_t[SHA512_HASH_SIZE];
    digest(hash, SHA512_HASH_SIZE);
    return hex_i8(hash, SHA512_HASH_SIZE, dst, dstlen);
}


std::string sha2_512_hash::digest() const
{
    char* hash = new char[SHA512_HASH_SIZE];
    digest(hash, SHA512_HASH_SIZE);

    std::string output(hash, SHA512_HASH_SIZE);
    delete[] hash;
    return output;
}


std::string sha2_512_hash::hexdigest() const
{
    return hex_i8(digest());
}
