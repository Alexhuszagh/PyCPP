//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "base64.h"

#include <cmath>
#include <iterator>

// CONSTANTS
// ---------

static constexpr size_t INPUT_INTERVAL = 3;
static constexpr size_t OUTPUT_INTERVAL = 4;
static constexpr char ENCODING[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static constexpr char DECODING[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

// HELPERS
// -------

// LENGTH

static size_t encoded_byte_count(const size_t length)
{
    static constexpr double input = static_cast<double>(INPUT_INTERVAL);
    static constexpr double output = static_cast<double>(OUTPUT_INTERVAL);
    return std::ceil(length * output / input);
}


static size_t decoded_byte_count(const size_t length)
{
    static constexpr double input = static_cast<double>(INPUT_INTERVAL);
    static constexpr double output = static_cast<double>(OUTPUT_INTERVAL);
    return std::floor(length * input / output);
}

/**
 *  Calculate upper bound onencoded message length, for buffer allocation
 *  to avoid reallocating.
 *
 *  The message size for each encoded message must be a multiple of 24,
 *  and each bit is transformed into 6-bit character groups to be
 *  transformed into the base64 dictionary (2^6 == 64).
 */
static size_t encoded_size(size_t length)
{
    return ((4 * length / 3) + 3) & ~3;
}


static size_t decoded_size(size_t length)
{
    static constexpr double input = static_cast<double>(INPUT_INTERVAL);
    static constexpr double output = static_cast<double>(OUTPUT_INTERVAL);
    return std::ceil(length * input) / output;
}


/**
 *  \brief Pad buffer to a fixed length.
 */
template <typename Iter, typename Array>
static size_t pad_buffer(Iter& first, Iter last,
    Array& array,
    size_t size,
    char pad = '\0')
{
    // fill
    size_t index = 0;
    for (; index < size && first < last; ++index, ++first) {
        array[index] = *first;
    }
    size_t filled = index;
    for (; index < size; ++index) {
        array[index] = pad;
    }

    return filled;
}


/**
 *  \brief Pad buffer to a fixed length.
 */
template <typename Iter, typename Array, typename Function>
static size_t pad_buffer(Iter& first, Iter last,
    Array& array,
    size_t size,
    Function function,
    char pad = '=')
{
    // fill
    size_t index = 0;
    size_t filled = SIZE_MAX;
    for (; index < size && first < last; ++index, ++first) {
        array[index] = function(*first);
        if (filled == SIZE_MAX) {
            filled = *first == pad ? index : SIZE_MAX;
        }
    }
    filled = filled == SIZE_MAX ? index : filled;
    for (; index < size; ++index) {
        array[index] = function(pad);
    }

    return filled;
}


// CHARACTER

/**
 *  \brief Encode 3 bytes to base64.
 */
template <typename Iter1, typename Iter2>
static void encode_base64_message(Iter1& src_first, Iter1 src_last, Iter2 &dst)
{
    char *b1 = new char[INPUT_INTERVAL];
    char *b2 = new char[OUTPUT_INTERVAL];
    size_t filled = encoded_byte_count(pad_buffer(src_first, src_last, b1, INPUT_INTERVAL));

    // First: 11111100
    b2[0] = ENCODING[(b1[0] & 0xfc) >> 2];
    // First: 00000011, Second: 11110000
    b2[1] = ENCODING[((b1[0] & 0x03) << 4) + ((b1[1] & 0xf0) >> 4)];
    // First: 00001111, Second: 11000000
    b2[2] = ENCODING[((b1[1] & 0x0f) << 2) + ((b1[2] & 0xc0) >> 6)];
    // First: 00111111
    b2[3] = ENCODING[b1[2] & 0x3f];

    for (size_t i = 0; i < filled; ++i) {
        *dst++ = b2[i];
    }
    for (size_t i = filled; i < OUTPUT_INTERVAL; ++i) {
        *dst++ = '=';
    }

    delete[] b1;
    delete[] b2;
}


/**
 *  \brief Decode 4 bytes from base64.
 */
template <typename Iter1, typename Iter2>
static void decode_base64_message(Iter1 &src_first, Iter1 src_last, Iter2 &dst)
{
    char* b1 = new char[OUTPUT_INTERVAL];
    char* b2 = new char[INPUT_INTERVAL];
    size_t filled = decoded_byte_count(pad_buffer(src_first, src_last, b1, OUTPUT_INTERVAL, [](char c) {
        return DECODING[static_cast<int>(c)];
    }));

    // First: 11111111, Second: 00110000
    b2[0] = static_cast<char>((b1[0] << 2) + ((b1[1] & 0x30) >> 4));
    // First: 00001111, Second: 00111100
    b2[1] = static_cast<char>(((b1[1] & 0x0f) << 4) + ((b1[2] & 0x3c) >> 2));
    // First: 00000011, Second: 11111111
    b2[2] = static_cast<char>(((b1[2] & 0x03) << 6) + b1[3]);

    for (size_t i = 0; i < filled; ++i) {
        *dst++ = b2[i];
    }

    delete[] b1;
    delete[] b2;
}


// FUNCTIONS
// ---------


size_t base64_encode(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    auto src_first = reinterpret_cast<const char*>(src);
    auto src_last = src_first + srclen;
    auto dst_first = reinterpret_cast<char*>(dst);
    auto dst_last = dst_first + dstlen;

    for (; src_first < src_last && dst_first + OUTPUT_INTERVAL <= dst_last; ) {
        encode_base64_message(src_first, src_last, dst_first);
    }

    return dst_first - reinterpret_cast<char*>(dst);
}


std::string base64_encode(const std::string& str)
{
    std::string base64;
    base64.reserve(encoded_size(str.size()));
    auto first = str.begin();
    auto last = str.end();
    auto dst = std::back_inserter(base64);
    for (; first < last; ) {
        encode_base64_message(first, last, dst);
    }

    return base64;
}


size_t base64_decode(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    auto src_first = reinterpret_cast<const char*>(src);
    auto src_last = src_first + srclen;
    auto dst_first = reinterpret_cast<char*>(dst);
    auto dst_last = dst_first + dstlen;

    for (; src_first < src_last && dst_first + INPUT_INTERVAL <= dst_last; ) {
        decode_base64_message(src_first, src_last, dst_first);
    }

    return dst_first - reinterpret_cast<char*>(dst);
}


std::string base64_decode(const std::string& str)
{
    std::string base64;
    base64.reserve(decoded_size(str.size()));
    auto first = str.begin();
    auto last = str.end();
    auto dst = std::back_inserter(base64);
    for (; first < last; ) {
        decode_base64_message(first, last, dst);
    }

    return base64;
}
