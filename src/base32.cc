//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <base32.h>
#include <cmath>
#include <iterator>

// CONSTANTS
// ---------

static constexpr size_t INPUT_INTERVAL = 5;
static constexpr size_t OUTPUT_INTERVAL = 8;
static constexpr char ENCODING[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
static constexpr char DECODING[] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

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

/** The message size for each encoded message must be a multiple of 40,
 *  and each bit is transformed into 5-bit character groups to be
 *  transformed into the base32 dictionary (2^5 == 32).
 */
static size_t encoded_size(size_t length)
{
    static constexpr double input = static_cast<double>(INPUT_INTERVAL);
    return std::ceil(length / input) * OUTPUT_INTERVAL;
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
 *  \brief Encode 5 bytes to base32.
 */
template <typename Iter1, typename Iter2>
static void encode_base32_message(Iter1& src_first, Iter1 src_last, Iter2 &dst)
{
    char *b1 = new char[INPUT_INTERVAL];
    char *b2 = new char[OUTPUT_INTERVAL];
    size_t filled = encoded_byte_count(pad_buffer(src_first, src_last, b1, INPUT_INTERVAL));

    // First: 11111000
    b2[0] = ENCODING[(b1[0] & 0xf8) >> 3];
    // First: 00000111, Second: 11000000
    b2[1] = ENCODING[((b1[0] & 0x07) << 2) + ((b1[1] & 0xc0) >> 6)];
    // First: 00111110
    b2[2] = ENCODING[((b1[1] & 0x3e) >> 1)];
    // First: 00000001, Second: 11110000
    b2[3] = ENCODING[((b1[1] & 0x01) << 4) + ((b1[2] & 0xf0) >> 4)];
    // First: 00001111, Second: 10000000
    b2[4] = ENCODING[((b1[2] & 0x0f) << 1) + ((b1[3] & 0x80) >> 7)];
    // First: 01111100
    b2[5] = ENCODING[((b1[3] & 0x7c) >> 2)];
    // First: 00000011, Second: 11100000
    b2[6] = ENCODING[((b1[3] & 0x03) << 3) + ((b1[4] & 0xe0) >> 5)];
    // First: 00011111
    b2[7] = ENCODING[((b1[4] & 0x1f))];

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
 *  \brief Decode 8 bytes from base32.
 */
template <typename Iter1, typename Iter2>
static void decode_base32_message(Iter1 &src_first, Iter1 src_last, Iter2 &dst)
{
    char* b1 = new char[OUTPUT_INTERVAL];
    char* b2 = new char[INPUT_INTERVAL];
    size_t filled = decoded_byte_count(pad_buffer(src_first, src_last, b1, OUTPUT_INTERVAL, [](char c) {
        return DECODING[static_cast<int>(c)];
    }));

    // First: 11111111, Second: 00011100
    b2[0] = static_cast<char>((b1[0] << 3) + ((b1[1] & 0x1c) >> 2));
    // First: 00000011, Second: 00011111, Third: 00010000
    b2[1] = static_cast<char>(((b1[1] & 0x03) << 6) + (b1[2] << 1) + ((b1[3] & 0x10) >> 4));
    // First: 00001111, Second: 00011110
    b2[2] = static_cast<char>(((b1[3] & 0x0f) << 4) + ((b1[4] & 0x1e) >> 1));
    // First: 00000001, Second: 00011111, Third: 00011000
    b2[3] = static_cast<char>(((b1[4] & 0x01) << 7) + (b1[5] << 2) + ((b1[6] & 0x18) >> 3));
    // First: 00000111, Second: 00011111
    b2[4] = static_cast<char>(((b1[6] & 0x07) << 5) + (b1[7]));

    for (size_t i = 0; i < filled; ++i) {
        *dst++ = b2[i];
    }

    delete[] b1;
    delete[] b2;
}

// FUNCTIONS
// ---------


size_t base32_encode(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    auto src_first = reinterpret_cast<const char*>(src);
    auto src_last = src_first + srclen;
    auto dst_first = reinterpret_cast<char*>(dst);
    auto dst_last = dst_first + dstlen;

    for (; src_first < src_last && dst_first + OUTPUT_INTERVAL <= dst_last; ) {
        encode_base32_message(src_first, src_last, dst_first);
    }

    return dst_first - reinterpret_cast<char*>(dst);
}


std::string base32_encode(const std::string& str)
{
    std::string base32;
    base32.reserve(encoded_size(str.size()));
    auto first = str.begin();
    auto last = str.end();
    auto dst = std::back_inserter(base32);
    for (; first < last; ) {
        encode_base32_message(first, last, dst);
    }

    return base32;
}


size_t base32_decode(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    auto src_first = reinterpret_cast<const char*>(src);
    auto src_last = src_first + srclen;
    auto dst_first = reinterpret_cast<char*>(dst);
    auto dst_last = dst_first + dstlen;

    for (; src_first < src_last && dst_first + INPUT_INTERVAL <= dst_last; ) {
        decode_base32_message(src_first, src_last, dst_first);
    }

    return dst_first - reinterpret_cast<char*>(dst);
}


std::string base32_decode(const std::string& str)
{
    std::string base32;
    base32.reserve(decoded_size(str.size()));
    auto first = str.begin();
    auto last = str.end();
    auto dst = std::back_inserter(base32);
    for (; first < last; ) {
        decode_base32_message(first, last, dst);
    }

    return base32;
}
