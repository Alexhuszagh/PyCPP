//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief BZIP2 compression and decompression unittests.
 */

#include <pycpp/compression/bzip2.h>
#include <gtest/gtest.h>
#include <sstream>

PYCPP_USING_NAMESPACE

// DATA
// ----

static std::string COMPRESSED("\x42\x5a\x68\x39\x31\x41\x59\x26\x53\x59\xa3\x79\xdd\x84\x00\x00\x00\x04\x00\x00\x40\x20\x00\x21\x18\x46\x82\xee\x48\xa7\x0a\x12\x14\x6f\x3b\xb0\x80", 37);
static std::string DECOMPRESSED("H", 1);

// TESTS
// -----


TEST(bzip2, bz2_compressor)
{
    char* buffer = new char[4096];
    std::string bz2 = DECOMPRESSED;
    const void* src;
    void* dst;

    try {
        // first example
        bz2_compressor ctx;
        src = bz2.data();
        dst = buffer;
        EXPECT_EQ(ctx.compress(src, bz2.size(), dst, 0), compression_need_output);
        EXPECT_EQ(ctx.compress(src, bz2.size(), dst, 37), compression_need_input);
        EXPECT_TRUE(ctx.flush(dst, 37));
        EXPECT_EQ(std::distance(buffer, (char*) dst), 37);
        EXPECT_EQ(strncmp(buffer, COMPRESSED.data(), COMPRESSED.size()), 0);

        // second example
        ctx = bz2_compressor();
        src = bz2.data();
        dst = buffer;
        EXPECT_EQ(ctx.compress(src, bz2.size(), dst, 4096), compression_need_input);
        EXPECT_TRUE(ctx.flush(dst, 4096));
        EXPECT_EQ(std::distance(buffer, (char*) dst), 37);
        EXPECT_EQ(strncmp(buffer, COMPRESSED.data(), COMPRESSED.size()), 0);

    } catch(...) {
        EXPECT_TRUE(false);
    }

    delete[] buffer;
}


TEST(bzip2, bz2_decompressor)
{
    char* buffer = new char[4096];
    std::string bz2 = COMPRESSED;
    const void* src;
    void* dst;

    try {
        // first example
        bz2_decompressor ctx;
        src = bz2.data();
        dst = buffer;
        EXPECT_EQ(ctx.decompress(src, bz2.size(), dst, 0), compression_need_output);
        EXPECT_EQ(ctx.decompress(src, bz2.size(), dst, 1), compression_eof);
        EXPECT_EQ(std::distance(buffer, (char*) dst), 1);
        EXPECT_EQ(strncmp(buffer, DECOMPRESSED.data(), DECOMPRESSED.size()), 0);

        // second example
        ctx = bz2_decompressor();
        src = bz2.data();
        dst = buffer;
        EXPECT_EQ(ctx.decompress(src, bz2.size(), dst, 4096), compression_eof);
        EXPECT_EQ(std::distance(buffer, (char*) dst), 1);
        EXPECT_EQ(strncmp(buffer, DECOMPRESSED.data(), DECOMPRESSED.size()), 0);

    } catch(...) {
        EXPECT_TRUE(false);
    }

    delete[] buffer;
}


TEST(bzip2, bzip2_compress)
{
    EXPECT_EQ(bzip2_compress(DECOMPRESSED), COMPRESSED);
}


TEST(bzip2, bzip2_decompress)
{
    EXPECT_EQ(bzip2_decompress(COMPRESSED, DECOMPRESSED.size()), DECOMPRESSED);
}
