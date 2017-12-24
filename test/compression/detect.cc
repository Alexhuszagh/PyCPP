//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Compression detection unittests.
 */

#include <pycpp/compression/detect.h>
#include <pycpp/stl/sstream.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(detect_compression, is_relocatable)
{
    static_assert(is_relocatable<is_bz2>::value, "");
    static_assert(is_relocatable<is_zlib>::value, "");
    static_assert(is_relocatable<is_gzip>::value, "");
    static_assert(is_relocatable<is_lzma>::value, "");
    static_assert(is_relocatable<is_blosc>::value, "");
}


TEST(detect_compression, is_bz2)
{
    std::string buffer("\x42\x5a\x68\x39\x31\x41\x59\x26\x53\x59", 10);
    istringstream stream(buffer);

    EXPECT_TRUE(is_bz2::header(buffer));
    EXPECT_TRUE(is_bz2::stream(stream));
}


TEST(detect_compression, is_zlib)
{
    std::string buffer("\x78\x9c\xf3H\xcd\xc9\xc9\x07\x00\x05\x8c", 10);
    istringstream stream(buffer);

    EXPECT_TRUE(is_zlib::header(buffer));
    EXPECT_TRUE(is_zlib::stream(stream));
}


TEST(detect_compression, is_gzip)
{
    std::string buffer("\x1f\x8b\x08\x08\x6d\xd9\xdc\x56\x00\x03", 10);
    istringstream stream(buffer);

    EXPECT_TRUE(is_gzip::header(buffer));
    EXPECT_TRUE(is_gzip::stream(stream));
}


TEST(detect_compression, is_lzma)
{
    std::string buffer("\xfd\x37\x7a\x58\x5a\x00\x00\x04\xe6\xd6", 10);
    istringstream stream(buffer);

    EXPECT_TRUE(is_lzma::header(buffer));
    EXPECT_TRUE(is_lzma::stream(stream));
}


TEST(detect_compression, is_blosc)
{
    std::string buffer("\x02\x01\x13\x08\x05\x00\x00\x00\x01\x00", 10);
    istringstream stream(buffer);

    EXPECT_TRUE(is_blosc::header(buffer));
    EXPECT_TRUE(is_blosc::stream(stream));
}
