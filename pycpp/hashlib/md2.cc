//  :copyright: (c) 2012-2015 Brad Conte.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  [reference] https://github.com/B-Con/crypto-algorithms
 */

#include <pycpp/hashlib.h>
#include <pycpp/processor.h>
#include <cstring>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr size_t MD2_HASH_SIZE = 16;
static constexpr uint8_t ENCODE[] = {41,46,67,201,162,216,124,1,61,54,84,161,236,240,6,19,98,167,5,243,192,199,115,140,152,147,43,217,188,76,130,202,30,155,87,60,253,212,224,22,103,66,111,24,138,23,229,18,190,78,196,214,218,158,222,73,160,251,245,142,187,47,238,122,169,104,121,145,21,178,7,63,148,194,16,137,11,34,95,33,128,127,93,154,90,144,50,39,53,62,204,231,191,247,151,3,255,25,48,179,72,165,181,209,215,94,146,42,172,86,170,198,79,184,56,210,150,164,125,182,118,252,107,226,156,116,4,241,69,157,112,89,100,113,135,32,134,91,207,101,230,45,168,2,27,96,37,173,174,176,185,246,28,70,97,105,52,64,126,15,85,71,163,35,221,81,175,58,195,92,249,206,186,197,234,38,44,83,13,110,133,40,132,9,211,223,205,244,65,129,77,82,106,220,55,200,108,193,171,250,36,225,123,8,12,189,177,74,120,136,149,139,227,99,232,109,233,203,213,254,59,0,29,57,242,239,183,14,102,88,208,228,166,119,114,248,235,117,75,10,49,68,80,180,143,237,31,26,219,153,141,51,159,17,131,20};

// OBJECTS
// -------

/** \brief MD2 context.
 */
struct md2_context
{
    int len;
    uint8_t data[16];
    uint8_t state[48];
    uint8_t checksum[16];
};

// FUNCTIONS
// ---------


static void md2_transform(md2_context *ctx, uint8_t* data)
{
    int j,k,t;

    for (j=0; j < 16; ++j) {
        ctx->state[j + 16] = data[j];
        ctx->state[j + 32] = (ctx->state[j+16] ^ ctx->state[j]);
    }

    t = 0;
    for (j = 0; j < 18; ++j) {
        for (k = 0; k < 48; ++k) {
            ctx->state[k] ^= ENCODE[t];
            t = ctx->state[k];
        }
        t = (t+j) & 0xFF;
    }

    t = ctx->checksum[15];
    for (j=0; j < 16; ++j) {
        ctx->checksum[j] ^= ENCODE[data[j] ^ t];
        t = ctx->checksum[j];
    }
}


static void md2_init(md2_context *ctx)
{
    secure_zero(ctx->state, sizeof(ctx->state));
    secure_zero(ctx->checksum, sizeof(ctx->checksum));
    ctx->len = 0;
}


static void md2_update(void *ptr, const void* buf, size_t len)
{
    auto* ctx = (md2_context*) ptr;
    auto* data = (const uint8_t*) buf;

    for (size_t i = 0; i < len; ++i) {
        ctx->data[ctx->len] = data[i];
        ctx->len++;
        if (ctx->len == MD2_HASH_SIZE) {
            md2_transform(ctx, ctx->data);
            ctx->len = 0;
        }
    }
}


static void md2_final(void* ptr, void* buf)
{
    auto* ctx = (md2_context*) ptr;
    auto* hash = (uint8_t*) buf;

    int pad = MD2_HASH_SIZE - ctx->len;
    while (ctx->len < MD2_HASH_SIZE) {
        ctx->data[ctx->len++] = pad;
    }

    md2_transform(ctx, ctx->data);
    md2_transform(ctx, ctx->checksum);

    memcpy(hash, ctx->state, MD2_HASH_SIZE);

    secure_zero(&pad, sizeof(pad));
    secure_zero(ctx, sizeof(*ctx));
}


// OBJECTS
// -------


md2_hash::md2_hash()
{
    ctx = new md2_context;
    md2_init(ctx);
}


md2_hash::md2_hash(const void* src, size_t srclen)
{
    ctx = new md2_context;
    md2_init(ctx);
    update(src, srclen);
}


md2_hash::md2_hash(const secure_string_view& str)
{
    ctx = new md2_context;
    md2_init(ctx);
    update(str);
}


md2_hash::~md2_hash()
{
    secure_zero(ctx, sizeof(*ctx));
    delete ctx;
}


void md2_hash::update(const void* src, size_t srclen)
{
    hash_update(ctx, src, srclen, md2_update);
}


void md2_hash::update(const secure_string_view& str)
{
    update(str.data(), str.size());
}


void md2_hash::digest(void*& dst, size_t dstlen) const
{
    md2_context copy = *ctx;
    hash_digest(&copy, dst, dstlen, MD2_HASH_SIZE, md2_final);
}


void md2_hash::hexdigest(void*& dst, size_t dstlen) const
{
    md2_context copy = *ctx;
    hash_hexdigest(&copy, dst, dstlen, MD2_HASH_SIZE, md2_final);
}


secure_string md2_hash::digest() const
{
    md2_context copy = *ctx;
    return hash_digest(&copy, MD2_HASH_SIZE, md2_final);
}


secure_string md2_hash::hexdigest() const
{
    md2_context copy = *ctx;
    return hash_hexdigest(&copy, MD2_HASH_SIZE, md2_final);
}

PYCPP_END_NAMESPACE
