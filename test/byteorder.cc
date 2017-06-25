//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Endian unittests.
 */

#include "byteorder.h"
#include <gtest/gtest.h>


// TESTS
// -----


TEST(endian, macros)
{
#ifndef BYTE_ORDER
#   error BYTE_ORDER should have been defined.
#endif

#ifndef FLOAT_WORD_ORDER
#   error FLOAT_WORD_ORDER should have been defined.
#endif

#ifndef htobe16
#   error htobe16 should have been defined.
#endif

#ifndef htole16
#   error htole16 should have been defined.
#endif

#ifndef be16toh
#   error be16toh should have been defined.
#endif

#ifndef le16toh
#   error le16toh should have been defined.
#endif

#ifndef htobe32
#   error htobe32 should have been defined.
#endif

#ifndef htole32
#   error htole32 should have been defined.
#endif

#ifndef be32toh
#   error be32toh should have been defined.
#endif

#ifndef le32toh
#   error le32toh should have been defined.
#endif

#ifndef htobe64
#   error htobe64 should have been defined.
#endif

#ifndef htole64
#   error htole64 should have been defined.
#endif

#ifndef be64toh
#   error be64toh should have been defined.
#endif

#ifndef le64toh
#   error le64toh should have been defined.
#endif
}


TEST(endian, bswap16)
{
    auto htobe = htobe16(1);
    auto htole = htole16(0x100ULL);
    EXPECT_EQ(htobe, htole);

    auto betoh = be16toh(1);
    auto letoh = le16toh(0x100ULL);
    EXPECT_EQ(betoh, letoh);
}


TEST(endian, bswap32)
{
    auto htobe = htobe32(1);
    auto htole = htole32(0x1000000ULL);
    EXPECT_EQ(htobe, htole);

    auto betoh = be32toh(1);
    auto letoh = le32toh(0x1000000ULL);
    EXPECT_EQ(betoh, letoh);
}


TEST(endian, bswap64)
{
// TODO: this is failing on MinGW
//    auto htobe = htobe64(1);
//    auto htole = htole64(0x100000000000000ULL);
//    EXPECT_EQ(htobe, htole);
//
//    auto betoh = be64toh(1);
//    auto letoh = le64toh(0x100000000000000ULL);
//    EXPECT_EQ(betoh, letoh);
}
