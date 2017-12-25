//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/string/base16.h>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr size_t INPUT_INTERVAL = 1;
static constexpr size_t OUTPUT_INTERVAL = 2;
static constexpr char ENCODING[] = "0123456789ABCDEF";
static constexpr char DECODING[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

// HELPERS
// -------

// LENGTH

/**
*   The message size for each encoded message is exactly twice the
 *  unencoded size, since base16 == 2^4, which means two hexadecimal
 *  digits are required for a single byte (2^8).
 */
static size_t encoded_size(size_t length)
{
    return OUTPUT_INTERVAL * length;
}


static size_t decoded_size(size_t length)
{
    return length / OUTPUT_INTERVAL;
}

// CHARACTER

/**
 *  \brief Encode 1 byte to base16.
 */
template <typename Iter1, typename Iter2>
static void encode_base16_message(Iter1 &src, Iter2 &dst)
{
    *dst++ = ENCODING[(*src & 0xf0) >> 4];          // First: 11110000
    *dst++ = ENCODING[(*src & 0x0f)];               // First: 00001111
    ++src;
}

/**
 *  \brief Decode 2 bytes from base16.
 */
template <typename Iter1, typename Iter2>
static void decode_base16_message(Iter1 &src_first, Iter1 src_last, Iter2 &dst)
{
    char buffer[OUTPUT_INTERVAL];
    size_t i = 0;
    for (; i < OUTPUT_INTERVAL && src_first != src_last; i++, src_first++) {
        buffer[i] = DECODING[static_cast<int>(*src_first)];
    }
    for (; i < OUTPUT_INTERVAL; i++) {
        buffer[i] = DECODING[0];
    }

    // First, 11111111, Seconds, 11111111
    *dst++ = static_cast<char>((buffer[0] << 4) + (buffer[1]));
}


// FUNCTIONS
// ---------


size_t base16_encode(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    auto src_first = reinterpret_cast<const char*>(src);
    auto src_last = src_first + srclen;
    auto dst_first = reinterpret_cast<char*>(dst);
    auto dst_last = dst_first + dstlen;

    for (; src_first < src_last && dst_first + OUTPUT_INTERVAL <= dst_last; ) {
        encode_base16_message(src_first, dst_first);
    }

    return dst_first - reinterpret_cast<char*>(dst);
}


string base16_encode(const string_wrapper& str)
{
    string base16;
    base16.reserve(encoded_size(str.size()));
    auto first = str.begin();
    auto last = str.end();
    auto dst = back_inserter(base16);
    for (; first != last; ) {
        encode_base16_message(first, dst);
    }

    return base16;
}


size_t base16_decode(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    auto src_first = reinterpret_cast<const char*>(src);
    auto src_last = src_first + srclen;
    auto dst_first = reinterpret_cast<char*>(dst);
    auto dst_last = dst_first + dstlen;

    for (; src_first + OUTPUT_INTERVAL <= src_last && dst_first < dst_last; ) {
        decode_base16_message(src_first, src_last, dst_first);
    }

    return dst_first - reinterpret_cast<char*>(dst);
}


string base16_decode(const string_wrapper& str)
{
    string base16;
    base16.reserve(decoded_size(str.size()));
    auto first = str.begin();
    auto last = str.end();
    auto dst = back_inserter(base16);
    for (; first != last; ) {
        decode_base16_message(first, last, dst);
    }

    return base16;
}

PYCPP_END_NAMESPACE
