//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/itoa.h>
#include <pycpp/lexical/table.h>
#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/stdexcept.h>
#include <assert.h>
#include <math.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

// RANGE

template <typename Int>
inline uint32_t digits(Int value, uint8_t base)
{
    if (value == 0) {
        return 1;
    }
    return static_cast<uint32_t>(floor(log(value) / log(base)) + 1);
}

// GENERIC

/**
 *  \brief Naive implementation for base-N numbers.
 */
template <typename Int>
static void itoa_naive(Int value, char* first, char*& last, uint8_t base)
{
    if (base < 2 || base > 36) {
        throw invalid_argument("Numerical base must be from 2-36");
    }

    uint32_t rem;
    char* p = first;
    while (value >= base) {
        rem = value % base;
        value /= base;
        *p++ = BASEN[rem];
    }

    rem = value % base;
    *p++ = BASEN[rem];

    reverse(first, p);
    last = p;
}

// SPECIFIC

/**
 *  \brief Optimized implementation for base10 numbers.
 */
template <typename Int, int base>
static void itoa_optimized(Int value, char* first, char*& last, const char* table)
{
    static int base2 = base * base;

    if (value == 0) {
        *first = '0';
        last = first+1;
        return;
    }

    uint32_t rem;
    char* p = first;
    while (value >= base2) {
        rem = 2 * (value % base2);
        value /= base2;
        *p++ = table[rem+1];
        *p++ = table[rem];
    }

    while (value > 0) {
        rem = value % base;
        *p++ = BASEN[rem];
        value /= base;
    }

    reverse(first, p);
    last = p;
}


template <typename Int, typename Uint>
void itoa_(Int value, char* first, char*& last, uint8_t base)
{
    // disable this check in release builds, since it's a logic
    // error and extraordinarily expensive
    assert(first <= last);
    assert(last - first >= digits(value, base) && "Need a larger buffer.");

    // handle negative numbers, use an unsigned type to avoid overflow
    char* tmp = last - 1;
    Uint v;
    if (value < 0) {
        first[0]= '-';
        v = -value;
        first++;
    } else {
        v = value;
    }

    // use optimized functions if possible
    switch (base) {
        // Ugly hack to convert runtime value to compile-time optimization
        case 2:     itoa_optimized<Uint, 2>(v, first, tmp, BASE2);   break;
        case 3:     itoa_optimized<Uint, 3>(v, first, tmp, BASE3);   break;
        case 4:     itoa_optimized<Uint, 4>(v, first, tmp, BASE4);   break;
        case 5:     itoa_optimized<Uint, 5>(v, first, tmp, BASE5);   break;
        case 6:     itoa_optimized<Uint, 6>(v, first, tmp, BASE6);   break;
        case 7:     itoa_optimized<Uint, 7>(v, first, tmp, BASE7);   break;
        case 8:     itoa_optimized<Uint, 8>(v, first, tmp, BASE8);   break;
        case 9:     itoa_optimized<Uint, 9>(v, first, tmp, BASE9);   break;
        case 10:    itoa_optimized<Uint, 10>(v, first, tmp, BASE10); break;
        case 11:    itoa_optimized<Uint, 11>(v, first, tmp, BASE11); break;
        case 12:    itoa_optimized<Uint, 12>(v, first, tmp, BASE12); break;
        case 13:    itoa_optimized<Uint, 13>(v, first, tmp, BASE13); break;
        case 14:    itoa_optimized<Uint, 14>(v, first, tmp, BASE14); break;
        case 15:    itoa_optimized<Uint, 15>(v, first, tmp, BASE15); break;
        case 16:    itoa_optimized<Uint, 16>(v, first, tmp, BASE16); break;
        case 17:    itoa_optimized<Uint, 17>(v, first, tmp, BASE17); break;
        case 18:    itoa_optimized<Uint, 18>(v, first, tmp, BASE18); break;
        case 19:    itoa_optimized<Uint, 19>(v, first, tmp, BASE19); break;
        case 20:    itoa_optimized<Uint, 20>(v, first, tmp, BASE20); break;
        case 21:    itoa_optimized<Uint, 21>(v, first, tmp, BASE21); break;
        case 22:    itoa_optimized<Uint, 22>(v, first, tmp, BASE22); break;
        case 23:    itoa_optimized<Uint, 23>(v, first, tmp, BASE23); break;
        case 24:    itoa_optimized<Uint, 24>(v, first, tmp, BASE24); break;
        case 25:    itoa_optimized<Uint, 25>(v, first, tmp, BASE25); break;
        case 26:    itoa_optimized<Uint, 26>(v, first, tmp, BASE26); break;
        case 27:    itoa_optimized<Uint, 27>(v, first, tmp, BASE27); break;
        case 28:    itoa_optimized<Uint, 28>(v, first, tmp, BASE28); break;
        case 29:    itoa_optimized<Uint, 29>(v, first, tmp, BASE29); break;
        case 30:    itoa_optimized<Uint, 30>(v, first, tmp, BASE30); break;
        case 31:    itoa_optimized<Uint, 31>(v, first, tmp, BASE31); break;
        case 32:    itoa_optimized<Uint, 32>(v, first, tmp, BASE32); break;
        case 33:    itoa_optimized<Uint, 33>(v, first, tmp, BASE33); break;
        case 34:    itoa_optimized<Uint, 34>(v, first, tmp, BASE34); break;
        case 35:    itoa_optimized<Uint, 35>(v, first, tmp, BASE35); break;
        case 36:    itoa_optimized<Uint, 36>(v, first, tmp, BASE36); break;
        default:    itoa_naive(v, first, tmp, base);                 break;
    }

    // add a trailing null character
    *tmp = '\0';
    last = tmp;
}

// FUNCTIONS
// ---------


void u8toa(uint8_t value, char* first, char*& last, uint8_t base)
{
    itoa_<uint8_t, uint8_t>(value, first, last, base);
}


std::string u8toa(uint8_t value, uint8_t base)
{
    char buffer[9];
    char* last = buffer + 9;
    u8toa(value, buffer, last, base);
    return std::string(buffer, last);
}


void i8toa(int8_t value, char* first, char*& last, uint8_t base)
{
    itoa_<int8_t, uint8_t>(value, first, last, base);
}


std::string i8toa(int8_t value, uint8_t base)
{
    char buffer[9];
    char* last = buffer + 9;
    i8toa(value, buffer, last, base);
    return std::string(buffer, last);
}


void u16toa(uint16_t value, char* first, char*& last, uint8_t base)
{
    itoa_<uint16_t, uint16_t>(value, first, last, base);
}


std::string u16toa(uint16_t value, uint8_t base)
{
    char buffer[17];
    char* last = buffer + 17;
    u16toa(value, buffer, last, base);
    return std::string(buffer, last);
}


void i16toa(int16_t value, char* first, char*& last, uint8_t base)
{
    itoa_<int16_t, uint16_t>(value, first, last, base);
}


std::string i16toa(int16_t value, uint8_t base)
{
    char buffer[17];
    char* last = buffer + 17;
    i16toa(value, buffer, last, base);
    return std::string(buffer, last);
}


void u32toa(uint32_t value, char* first, char*& last, uint8_t base)
{
    itoa_<uint32_t, uint32_t>(value, first, last, base);
}


std::string u32toa(uint32_t value, uint8_t base)
{
    char buffer[33];
    char* last = buffer + 33;
    u32toa(value, buffer, last, base);
    return std::string(buffer, last);
}


void i32toa(int32_t value, char* first, char*& last, uint8_t base)
{
    itoa_<int32_t, uint32_t>(value, first, last, base);
}


std::string i32toa(int32_t value, uint8_t base)
{
    char buffer[33];
    char* last = buffer + 33;
    i32toa(value, buffer, last, base);
    return std::string(buffer, last);
}


void u64toa(uint64_t value, char* first, char*& last, uint8_t base)
{
    itoa_<uint64_t, uint64_t>(value, first, last, base);
}


std::string u64toa(uint64_t value, uint8_t base)
{
    char buffer[65];
    char* last = buffer + 65;
    u64toa(value, buffer, last, base);
    return std::string(buffer, last);
}


void i64toa(int64_t value, char* first, char*& last, uint8_t base)
{
    itoa_<int64_t, uint64_t>(value, first, last, base);
}


std::string i64toa(int64_t value, uint8_t base)
{
    char buffer[65];
    char* last = buffer + 65;
    i64toa(value, buffer, last, base);
    return std::string(buffer, last);
}

PYCPP_END_NAMESPACE
