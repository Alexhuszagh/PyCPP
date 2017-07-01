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
#include <cassert>
#include <cstring>
#include <stdexcept>

// CONSTANTS
// ---------

static constexpr size_t sha3_224_hash_size = 28;
static constexpr size_t sha3_256_hash_size = 32;
static constexpr size_t sha3_384_hash_size = 48;
static constexpr size_t sha3_512_hash_size = 64;
static constexpr size_t sha3_max_permutation_size = 25;
static constexpr size_t sha3_max_rate_in_qwords = 24;

// OBJECTS
// -------

/**
 *  \brief SHA3 context.
 */
struct sha3_context
{
    uint64_t hash[sha3_max_permutation_size];
    uint64_t message[sha3_max_rate_in_qwords];
    uint32_t rest;
    uint32_t block_size;
};

// FUNCTIONS
// ---------


static void keccak_init(sha3_context* ctx, uint32_t bits)
{
    /* NB: The Keccak capacity parameter = bits * 2 */
    uint32_t rate = 1600 - bits * 2;

    secure_zero(ctx, sizeof(sha3_context));
    ctx->block_size = rate / 8;
    assert(rate <= 1600 && (rate % 64) == 0);
}


void sha3_224_init(sha3_context* ctx)
{
    keccak_init(ctx, 224);
}


void sha3_256_init(sha3_context* ctx)
{
    keccak_init(ctx, 256);
}


void sha3_384_init(sha3_context* ctx)
{
    keccak_init(ctx, 384);
}


void sha3_512_init(sha3_context* ctx)
{
    keccak_init(ctx, 512);
}
