//  :copyright: (c) 2011-2017 RHash.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  [reference] https://github.com/rhash/RHash
 */

#include <pycpp/hashlib.h>
#include <pycpp/preprocessor/architecture.h>
#include <pycpp/preprocessor/byteorder.h>
#include <pycpp/secure/stdlib.h>
#include <pycpp/stl/stdexcept.h>
#include <assert.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr size_t SHA3_224_HASH_SIZE = 28;
static constexpr size_t SHA3_256_HASH_SIZE = 32;
static constexpr size_t SHA3_384_HASH_SIZE = 48;
static constexpr size_t SHA3_512_HASH_SIZE = 64;
static constexpr size_t SHA3_MAX_PERMUTATION_SIZE = 25;
static constexpr size_t QWORDS = 24;
static constexpr int SHA3_ROUNDS = 24;
static constexpr uint64_t KECCAK_ROUND_CONSTANTS[SHA3_ROUNDS] = {
    0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808AULL, 0x8000000080008000ULL,
    0x000000000000808BULL, 0x0000000080000001ULL, 0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008AULL, 0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000AULL,
    0x000000008000808BULL, 0x800000000000008BULL, 0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL, 0x000000000000800AULL, 0x800000008000000AULL,
    0x8000000080008081ULL, 0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
};

// OBJECTS
// -------

/**
 *  \brief SHA3 context.
 */
struct sha3_context
{
    uint64_t hash[SHA3_MAX_PERMUTATION_SIZE];
    uint64_t message[QWORDS];
    uint32_t rest;
    uint32_t block_size;
};

// FUNCTIONS
// ---------

#define ROTL64(qword, n) ((qword) << (n) ^ ((qword) >> (64 - (n))))


static void keccak_init(sha3_context* ctx, uint32_t bits) noexcept
{
    /* NB: The Keccak capacity parameter = bits * 2 */
    uint32_t rate = 1600 - bits * 2;

    secure_zero(ctx, sizeof(sha3_context));
    ctx->block_size = rate / 8;
    assert(rate <= 1600 && (rate % 64) == 0);
}


void sha3_224_init(sha3_context* ctx) noexcept
{
    keccak_init(ctx, 224);
}


void sha3_256_init(sha3_context* ctx) noexcept
{
    keccak_init(ctx, 256);
}


void sha3_384_init(sha3_context* ctx) noexcept
{
    keccak_init(ctx, 384);
}


void sha3_512_init(sha3_context* ctx) noexcept
{
    keccak_init(ctx, 512);
}


/*
 *  Keccak theta() transformation
 */
static void keccak_theta(uint64_t *A) noexcept
{
    unsigned int x;
    uint64_t C[5], D[5];

    for (x = 0; x < 5; x++) {
        C[x] = A[x] ^ A[x + 5] ^ A[x + 10] ^ A[x + 15] ^ A[x + 20];
    }
    D[0] = ROTL64(C[1], 1) ^ C[4];
    D[1] = ROTL64(C[2], 1) ^ C[0];
    D[2] = ROTL64(C[3], 1) ^ C[1];
    D[3] = ROTL64(C[4], 1) ^ C[2];
    D[4] = ROTL64(C[0], 1) ^ C[3];

    for (x = 0; x < 5; x++) {
        A[x]      ^= D[x];
        A[x + 5]  ^= D[x];
        A[x + 10] ^= D[x];
        A[x + 15] ^= D[x];
        A[x + 20] ^= D[x];
    }
}

/*
 *  Keccak pi() transformation
 */
static void keccak_pi(uint64_t *A) noexcept
{
    uint64_t A1;
    A1 = A[1];
    A[ 1] = A[ 6];
    A[ 6] = A[ 9];
    A[ 9] = A[22];
    A[22] = A[14];
    A[14] = A[20];
    A[20] = A[ 2];
    A[ 2] = A[12];
    A[12] = A[13];
    A[13] = A[19];
    A[19] = A[23];
    A[23] = A[15];
    A[15] = A[ 4];
    A[ 4] = A[24];
    A[24] = A[21];
    A[21] = A[ 8];
    A[ 8] = A[16];
    A[16] = A[ 5];
    A[ 5] = A[ 3];
    A[ 3] = A[18];
    A[18] = A[17];
    A[17] = A[11];
    A[11] = A[ 7];
    A[ 7] = A[10];
    A[10] = A1;
    // note: A[ 0] is left as is
}

/* Keccak chi() transformation */
static void keccak_chi(uint64_t *A) noexcept
{
    int i;
    for (i = 0; i < 25; i += 5) {
        uint64_t A0 = A[0 + i], A1 = A[1 + i];
        A[0 + i] ^= ~A1 & A[2 + i];
        A[1 + i] ^= ~A[2 + i] & A[3 + i];
        A[2 + i] ^= ~A[3 + i] & A[4 + i];
        A[3 + i] ^= ~A[4 + i] & A0;
        A[4 + i] ^= ~A0 & A1;
    }
}


static void sha3_permutation(uint64_t *state) noexcept
{
    int r;
    for (r = 0; r < SHA3_ROUNDS; r++)
    {
        keccak_theta(state);

        // apply Keccak rho() transformation
        state[ 1] = ROTL64(state[ 1],  1);
        state[ 2] = ROTL64(state[ 2], 62);
        state[ 3] = ROTL64(state[ 3], 28);
        state[ 4] = ROTL64(state[ 4], 27);
        state[ 5] = ROTL64(state[ 5], 36);
        state[ 6] = ROTL64(state[ 6], 44);
        state[ 7] = ROTL64(state[ 7],  6);
        state[ 8] = ROTL64(state[ 8], 55);
        state[ 9] = ROTL64(state[ 9], 20);
        state[10] = ROTL64(state[10],  3);
        state[11] = ROTL64(state[11], 10);
        state[12] = ROTL64(state[12], 43);
        state[13] = ROTL64(state[13], 25);
        state[14] = ROTL64(state[14], 39);
        state[15] = ROTL64(state[15], 41);
        state[16] = ROTL64(state[16], 45);
        state[17] = ROTL64(state[17], 15);
        state[18] = ROTL64(state[18], 21);
        state[19] = ROTL64(state[19],  8);
        state[20] = ROTL64(state[20], 18);
        state[21] = ROTL64(state[21],  2);
        state[22] = ROTL64(state[22], 61);
        state[23] = ROTL64(state[23], 56);
        state[24] = ROTL64(state[24], 14);

        keccak_pi(state);
        keccak_chi(state);

        // apply iota(state, r)
        *state ^= KECCAK_ROUND_CONSTANTS[r];
    }
}

/**
 *  The core transformation. Process the specified block of data.
 */
static void sha3_process_block(uint64_t* hash, const uint64_t* block, size_t block_size) noexcept
{
    /* expanded loop */
    hash[ 0] ^= le64toh(block[ 0]);
    hash[ 1] ^= le64toh(block[ 1]);
    hash[ 2] ^= le64toh(block[ 2]);
    hash[ 3] ^= le64toh(block[ 3]);
    hash[ 4] ^= le64toh(block[ 4]);
    hash[ 5] ^= le64toh(block[ 5]);
    hash[ 6] ^= le64toh(block[ 6]);
    hash[ 7] ^= le64toh(block[ 7]);
    hash[ 8] ^= le64toh(block[ 8]);
    /* if not sha3-512 */
    if (block_size > 72) {
        hash[ 9] ^= le64toh(block[ 9]);
        hash[10] ^= le64toh(block[10]);
        hash[11] ^= le64toh(block[11]);
        hash[12] ^= le64toh(block[12]);
        /* if not sha3-384 */
        if (block_size > 104) {
            hash[13] ^= le64toh(block[13]);
            hash[14] ^= le64toh(block[14]);
            hash[15] ^= le64toh(block[15]);
            hash[16] ^= le64toh(block[16]);
            /* if not sha3-256 */
            if (block_size > 136) {
                hash[17] ^= le64toh(block[17]);
                /* if not sha3-224 */
                if (block_size > 144) {
                    hash[18] ^= le64toh(block[18]);
                    hash[19] ^= le64toh(block[19]);
                    hash[20] ^= le64toh(block[20]);
                    hash[21] ^= le64toh(block[21]);
                    hash[22] ^= le64toh(block[22]);
                    hash[23] ^= le64toh(block[23]);
                    hash[24] ^= le64toh(block[24]);
                }
            }
        }
    }
    // make a permutation of the hash
    sha3_permutation(hash);
}


/**
 *  Calculate message hash.
 *  Can be called repeatedly with chunks of the message to be hashed.
 */
void sha3_update(void* ptr, const void *buf, size_t size) noexcept
{
    auto* ctx = (sha3_context*) ptr;
    auto* msg = (const uint8_t*) buf;

    size_t index = (size_t)ctx->rest;
    size_t block_size = (size_t)ctx->block_size;

    ctx->rest = (unsigned)((ctx->rest + size) % block_size);

    // fill partial block
    if (index) {
        size_t left = block_size - index;
        memcpy((char*)ctx->message + index, msg, (size < left ? size : left));
        if (size < left) {
            return;
        }

        // process partial block
        sha3_process_block(ctx->hash, ctx->message, block_size);
        msg  += left;
        size -= left;
    }
    while (size >= block_size) {
        uint64_t* aligned_message_block;
        if (IS_ALIGNED_64(msg)) {
            /* the most common case is processing of an already aligned message
            without copying it */
            aligned_message_block = (uint64_t*)msg;
        } else {
            memcpy(ctx->message, msg, block_size);
            aligned_message_block = ctx->message;
        }

        sha3_process_block(ctx->hash, aligned_message_block, block_size);
        msg  += block_size;
        size -= block_size;
    }
    if (size) {
        memcpy(ctx->message, msg, size); // save leftovers
    }
}


/**
 *  Store calculated hash into the given array.
 */
void sha3_final(void* ptr, void* buf) noexcept
{
    auto* ctx = (sha3_context*) ptr;
    auto* result = (uint8_t*) buf;

    size_t digest_length = 100 - ctx->block_size / 2;
    size_t block_size = ctx->block_size;

    // clear the rest of the data queue
    memset((char*)ctx->message + ctx->rest, 0, block_size - ctx->rest);
    ((char*)ctx->message)[ctx->rest] |= 0x06;
    ((char*)ctx->message)[block_size - 1] |= 0x80;

    // process final block
    sha3_process_block(ctx->hash, ctx->message, block_size);

    assert(block_size > digest_length);
    if (result) {
        memcpy_htole64(result, ctx->hash, digest_length);
    }

    secure_zero(&digest_length, sizeof(digest_length));
    secure_zero(&block_size, sizeof(block_size));
    secure_zero(ctx, sizeof(*ctx));
}


// OBJECTS
// -------



sha3_224_hash::sha3_224_hash():
    ctx(make_unique<sha3_context>())
{
    sha3_224_init(ctx.get());
}


sha3_224_hash::sha3_224_hash(const void* src, size_t srclen):
    ctx(make_unique<sha3_context>())
{
    sha3_224_init(ctx.get());
    update(src, srclen);
}


sha3_224_hash::sha3_224_hash(const string_wrapper& str):
    ctx(make_unique<sha3_context>())
{
    sha3_224_init(ctx.get());
    update(str);
}


sha3_224_hash::~sha3_224_hash() noexcept
{
    secure_zero(ctx.get(), sizeof(*ctx));
}


void sha3_224_hash::update(const void* src, size_t srclen) noexcept
{
    hash_update(ctx.get(), src, srclen, sha3_update);
}


void sha3_224_hash::update(const string_wrapper& str) noexcept
{
    update(str.data(), str.size());
}


void sha3_224_hash::digest(void*& dst, size_t dstlen) const
{
    sha3_context copy = *ctx;
    return hash_digest(&copy, dst, dstlen, SHA3_224_HASH_SIZE, sha3_final);
}


void sha3_224_hash::hexdigest(void*& dst, size_t dstlen) const
{
    sha3_context copy = *ctx;
    return hash_hexdigest(&copy, dst, dstlen, SHA3_224_HASH_SIZE, sha3_final);
}


secure_string sha3_224_hash::digest() const
{
    sha3_context copy = *ctx;
    return hash_digest(&copy, SHA3_224_HASH_SIZE, sha3_final);
}


secure_string sha3_224_hash::hexdigest() const
{
    sha3_context copy = *ctx;
    return hash_hexdigest(&copy, SHA3_224_HASH_SIZE, sha3_final);
}


void sha3_224_hash::swap(sha3_224_hash& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    swap(ctx, rhs.ctx);
}


sha3_256_hash::sha3_256_hash():
    ctx(make_unique<sha3_context>())
{
    sha3_256_init(ctx.get());
}


sha3_256_hash::sha3_256_hash(const void* src, size_t srclen):
    ctx(make_unique<sha3_context>())
{
    sha3_256_init(ctx.get());
    update(src, srclen);
}


sha3_256_hash::sha3_256_hash(const string_wrapper& str):
    ctx(make_unique<sha3_context>())
{
    sha3_256_init(ctx.get());
    update(str);
}


sha3_256_hash::~sha3_256_hash() noexcept
{
    secure_zero(ctx.get(), sizeof(*ctx));
}


void sha3_256_hash::update(const void* src, size_t srclen) noexcept
{
    hash_update(ctx.get(), src, srclen, sha3_update);
}


void sha3_256_hash::update(const string_wrapper& str) noexcept
{
    update(str.data(), str.size());
}


void sha3_256_hash::digest(void*& dst, size_t dstlen) const
{
    sha3_context copy = *ctx;
    return hash_digest(&copy, dst, dstlen, SHA3_256_HASH_SIZE, sha3_final);
}


void sha3_256_hash::hexdigest(void*& dst, size_t dstlen) const
{
    sha3_context copy = *ctx;
    return hash_hexdigest(&copy, dst, dstlen, SHA3_256_HASH_SIZE, sha3_final);
}


secure_string sha3_256_hash::digest() const
{
    sha3_context copy = *ctx;
    return hash_digest(&copy, SHA3_256_HASH_SIZE, sha3_final);
}


secure_string sha3_256_hash::hexdigest() const
{
    sha3_context copy = *ctx;
    return hash_hexdigest(&copy, SHA3_256_HASH_SIZE, sha3_final);
}


void sha3_256_hash::swap(sha3_256_hash& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    swap(ctx, rhs.ctx);
}


sha3_384_hash::sha3_384_hash():
    ctx(make_unique<sha3_context>())
{
    sha3_384_init(ctx.get());
}


sha3_384_hash::sha3_384_hash(const void* src, size_t srclen):
    ctx(make_unique<sha3_context>())
{
    sha3_384_init(ctx.get());
    update(src, srclen);
}


sha3_384_hash::sha3_384_hash(const string_wrapper& str):
    ctx(make_unique<sha3_context>())
{
    sha3_384_init(ctx.get());
    update(str);
}


sha3_384_hash::~sha3_384_hash() noexcept
{
    secure_zero(ctx.get(), sizeof(*ctx));
}


void sha3_384_hash::update(const void* src, size_t srclen) noexcept
{
    hash_update(ctx.get(), src, srclen, sha3_update);
}


void sha3_384_hash::update(const string_wrapper& str) noexcept
{
    update(str.data(), str.size());
}


void sha3_384_hash::digest(void*& dst, size_t dstlen) const
{
    sha3_context copy = *ctx;
    return hash_digest(&copy, dst, dstlen, SHA3_384_HASH_SIZE, sha3_final);
}


void sha3_384_hash::hexdigest(void*& dst, size_t dstlen) const
{
    sha3_context copy = *ctx;
    return hash_hexdigest(&copy, dst, dstlen, SHA3_384_HASH_SIZE, sha3_final);
}


secure_string sha3_384_hash::digest() const
{
    sha3_context copy = *ctx;
    return hash_digest(&copy, SHA3_384_HASH_SIZE, sha3_final);
}


secure_string sha3_384_hash::hexdigest() const
{
    sha3_context copy = *ctx;
    return hash_hexdigest(&copy, SHA3_384_HASH_SIZE, sha3_final);
}


void sha3_384_hash::swap(sha3_384_hash& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    swap(ctx, rhs.ctx);
}


sha3_512_hash::sha3_512_hash():
    ctx(make_unique<sha3_context>())
{
    sha3_512_init(ctx.get());
}


sha3_512_hash::sha3_512_hash(const void* src, size_t srclen):
    ctx(make_unique<sha3_context>())
{
    sha3_512_init(ctx.get());
    update(src, srclen);
}


sha3_512_hash::sha3_512_hash(const string_wrapper& str):
    ctx(make_unique<sha3_context>())
{
    sha3_512_init(ctx.get());
    update(str);
}


sha3_512_hash::~sha3_512_hash() noexcept
{
    secure_zero(ctx.get(), sizeof(*ctx));
}


void sha3_512_hash::update(const void* src, size_t srclen) noexcept
{
    hash_update(ctx.get(), src, srclen, sha3_update);
}


void sha3_512_hash::update(const string_wrapper& str) noexcept
{
    update(str.data(), str.size());
}


void sha3_512_hash::digest(void*& dst, size_t dstlen) const
{
    sha3_context copy = *ctx;
    return hash_digest(&copy, dst, dstlen, SHA3_512_HASH_SIZE, sha3_final);
}


void sha3_512_hash::hexdigest(void*& dst, size_t dstlen) const
{
    sha3_context copy = *ctx;
    return hash_hexdigest(&copy, dst, dstlen, SHA3_512_HASH_SIZE, sha3_final);
}


secure_string sha3_512_hash::digest() const
{
    sha3_context copy = *ctx;
    return hash_digest(&copy, SHA3_512_HASH_SIZE, sha3_final);
}


secure_string sha3_512_hash::hexdigest() const
{
    sha3_context copy = *ctx;
    return hash_hexdigest(&copy, SHA3_512_HASH_SIZE, sha3_final);
}


void sha3_512_hash::swap(sha3_512_hash& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    swap(ctx, rhs.ctx);
}

PYCPP_END_NAMESPACE
