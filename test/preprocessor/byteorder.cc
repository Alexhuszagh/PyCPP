//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Endian unittests.
 */

#include <pycpp/preprocessor/byteorder.h>
#include <gtest/gtest.h>

// TESTS
// -----


TEST(endian, macros)
{
#ifndef bswap16
#   error "bswap16 should have been defined."
#endif

#ifndef bswap32
#   error "bswap32 should have been defined."
#endif

#ifndef bswap64
#   error "bswap64 should have been defined."
#endif

#ifndef htobe
#   error "htobe should have been defined."
#endif

#ifndef htole
#   error "htole should have been defined."
#endif

#ifndef betoh
#   error "betoh should have been defined."
#endif

#ifndef letoh
#   error "letoh should have been defined."
#endif

#ifndef BYTE_ORDER
#   error "BYTE_ORDER should have been defined."
#endif

#ifndef FLOAT_WORD_ORDER
#   error "FLOAT_WORD_ORDER should have been defined."
#endif

#ifndef htobe16
#   error "htobe16 should have been defined."
#endif

#ifndef htole16
#   error "htole16 should have been defined."
#endif

#ifndef be16toh
#   error "be16toh should have been defined."
#endif

#ifndef le16toh
#   error "le16toh should have been defined."
#endif

#ifndef htobe32
#   error "htobe32 should have been defined."
#endif

#ifndef htole32
#   error "htole32 should have been defined."
#endif

#ifndef be32toh
#   error "be32toh should have been defined."
#endif

#ifndef le32toh
#   error "le32toh should have been defined."
#endif

#ifndef htobe64
#   error "htobe64 should have been defined."
#endif

#ifndef htole64
#   error "htole64 should have been defined."
#endif

#ifndef be64toh
#   error "be64toh should have been defined."
#endif

#ifndef le64toh
#   error "le64toh should have been defined."
#endif
}


TEST(endian, bswapnn)
{
    // 16-bit
    uint16_t i16_1 = 0x100ULL;
    uint16_t i16_2 = 1;
    uint16_t i16_3;
    bswap(&i16_3, &i16_1, 2);
    EXPECT_EQ(i16_3, i16_2);

    bswap(&i16_3, &i16_2, 2);
    EXPECT_EQ(i16_3, i16_1);

    htobe(&i16_3, 2);
    htole(&i16_3, 2);
    EXPECT_EQ(i16_3, i16_2);

    betoh(&i16_3, 2);
    letoh(&i16_3, 2);
    EXPECT_EQ(i16_3, i16_1);

    // 32-bit
    uint32_t i32_1 = 0x1000000ULL;
    uint32_t i32_2 = 1;
    uint32_t i32_3;
    bswap(&i32_3, &i32_1, 4);
    EXPECT_EQ(i32_3, i32_2);

    bswap(&i32_3, &i32_2, 4);
    EXPECT_EQ(i32_3, i32_1);

    htobe(&i32_3, 4);
    htole(&i32_3, 4);
    EXPECT_EQ(i32_3, i32_2);

    betoh(&i32_3, 4);
    letoh(&i32_3, 4);
    EXPECT_EQ(i32_3, i32_1);

    // 64-bit
    uint64_t i64_1 = 0x100000000000000ULL;
    uint64_t i64_2 = 1;
    uint64_t i64_3;
    bswap(&i64_3, &i64_1, 8);
    EXPECT_EQ(i64_3, i64_2);

    bswap(&i64_3, &i64_2, 8);
    EXPECT_EQ(i64_3, i64_1);

    htobe(&i64_3, 8);
    htole(&i64_3, 8);
    EXPECT_EQ(i64_3, i64_2);

    betoh(&i64_3, 8);
    letoh(&i64_3, 8);
    EXPECT_EQ(i64_3, i64_1);
}



TEST(endian, bswap16)
{
    auto htobe = htobe16(1);
    auto htole = htole16(0x100ULL);
    EXPECT_EQ(htobe, htole);

    auto betoh = be16toh(1);
    auto letoh = le16toh(0x100ULL);
    EXPECT_EQ(betoh, letoh);

    EXPECT_EQ(bswap16(0x100ULL), 1);
    EXPECT_EQ(bswap16(1), 0x100ULL);
}


TEST(endian, bswap32)
{
    auto htobe = htobe32(1);
    auto htole = htole32(0x1000000ULL);
    EXPECT_EQ(htobe, htole);

    auto betoh = be32toh(1);
    auto letoh = le32toh(0x1000000ULL);
    EXPECT_EQ(betoh, letoh);

    EXPECT_EQ(bswap32(0x1000000ULL), 1);
    EXPECT_EQ(bswap32(1), 0x1000000ULL);
}


TEST(endian, bswap64)
{
    auto htobe = htobe64(1);
    auto htole = htole64(0x100000000000000ULL);
    EXPECT_EQ(htobe, htole);

    auto betoh = be64toh(1);
    auto letoh = le64toh(0x100000000000000ULL);
    EXPECT_EQ(betoh, letoh);

    EXPECT_EQ(bswap64(0x100000000000000ULL), 1);
    EXPECT_EQ(bswap64(1), 0x100000000000000ULL);
}


TEST(endian, memcpy_bswapnn)
{
    uint16_t h[1];
    uint16_t s[1];

    h[0] = 0x100ULL;
    memcpy_bswap(s, h, 2, 2);
    EXPECT_EQ(s[0], 1);
}


TEST(endian, memcpy_bswap16)
{
    uint16_t h[1];
    uint16_t s[1];

    h[0] = 0x100ULL;
    memcpy_bswap16(s, h, 2);
    EXPECT_EQ(s[0], 1);
}


TEST(endian, memcpy_bswap32)
{
    uint32_t h[1];
    uint32_t s[1];

    h[0] = 0x1000000ULL;
    memcpy_bswap32(s, h, 4);
    EXPECT_EQ(s[0], 1);
}


TEST(endian, memcpy_bswap64)
{
    uint64_t h[1];
    uint64_t s[1];

    h[0] = 0x100000000000000ULL;
    memcpy_bswap64(s, h, 8);
    EXPECT_EQ(s[0], 1);
}
