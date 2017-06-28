//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Hashlib unittests.
 */

#include "hashlib.h"
#include <gtest/gtest.h>

#include <random>
#include <utility>
#include <vector>

// TESTS
// -----


TEST(md2, digest)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {
            "LOWER",
            "F6A2EBAF32C712B834BC2955EF65B575"
        },
        {
            "lower-/",
            "51E0883600D9C96A25238478EE119005"
        },
        {
            "012345",
            "D1E2CA033B157BE9B293AB08E8D3F7E0"
        },
        {
            "aaaAA0aa",
            "A6CF9BF6277C5E167AC738A259C0521B"
        },
        {
            "This is a long message",
            "710C78446AF936E31300A865F544E069"
        },
        {
            std::string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "A61F3E492D7E68ABF93527A684D3BDC1",
        },
        {
            std::string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "9AB540CC64A3FEEEAB112B342BFEF141",
        },
        {
            std::string {77, -61, -86, 109, 101},
            "E6DD2B5A82E3395EE57E630CB88EB99C",
        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(md2_hash(pair.first).hexdigest(), pair.second);
    }
}


TEST(md2, update)
{
    md2_hash hash("x");
    EXPECT_EQ(hash.hexdigest(), "A0365D9BF982AAAD3526A01DB8A7206D");

    hash.update("x");
    EXPECT_EQ(hash.hexdigest(), "FC770B692CAA57F6DC13FC2D1CC391A9");
}


TEST(md2, fuzz)
{
    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<> dist(-128, 127);
    for (size_t i = 0; i < 50; i++) {
        const size_t length = rand() % 1000;
        std::string input;
        input.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            input.push_back(static_cast<char>(dist(engine)));
        }
        EXPECT_EQ(md2_hash(input).hexdigest().size(), 32);
    }
}


TEST(md4, digest)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {
            "LOWER",
            "0A5F25A7ADAE4B5FC3CD6F2EC94B4D18"
        },
        {
            "lower-/",
            "430AA12ED12FE599647AF395B9B3D105"
        },
        {
            "012345",
            "9C79EAE82A4F7E7792FD5CF07A1CA21B"
        },
        {
            "aaaAA0aa",
            "0A385B1967C792C2C011ADBE4E96DD38"
        },
        {
            "This is a long message",
            "F04E9006AB9676ED48935BAEE9E67DDF"
        },
        {
            std::string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "40D58CB3B6E382886247C08F56D1DD11",
        },
        {
            std::string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "9FBBA5A7D9392EC44F6A60990B28F787",
        },
        {
            std::string {77, -61, -86, 109, 101},
            "29BF61E97761AB9C9DCC7B700BCF1E7B",
        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(md4_hash(pair.first).hexdigest(), pair.second);
    }
}


TEST(md4, update)
{
    md4_hash hash("x");
    EXPECT_EQ(hash.hexdigest(), "51B834B7C1EF0B59EA50888FCB39ACE2");

    hash.update("x");
    EXPECT_EQ(hash.hexdigest(), "33E0B062039C10F3F92BADEFED4A5C0B");
}


TEST(md4, fuzz)
{
    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<> dist(-128, 127);
    for (size_t i = 0; i < 50; i++) {
        const size_t length = rand() % 1000;
        std::string input;
        input.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            input.push_back(static_cast<char>(dist(engine)));
        }
        EXPECT_EQ(md4_hash(input).hexdigest().size(), 32);
    }
}


TEST(md5, digest)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {
            "LOWER",
            "A7C48BA367E019D004BFB0239B85F2B3"
        },
        {
            "lower-/",
            "72C45A43715F5D9ADFC0FCAA2F59387D"
        },
        {
            "012345",
            "D6A9A933C8AAFC51E55AC0662B6E4D4A"
        },
        {
            "aaaAA0aa",
            "6B141DFD1512CF9FC18A8A04C9323F52"
        },
        {
            "This is a long message",
            "876BDBA2E3B24196AF5AE34219316593"
        },
        {
            std::string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "D6D014C612BE422818CA8A960063D051",
        },
        {
            std::string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "E462805DCF84413D5EDDCA45A4B88A5E",
        },
        {
            std::string {77, -61, -86, 109, 101},
            "89AAF570E626AC6BF001699DF83AF681",
        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(md5_hash(pair.first).hexdigest(), pair.second);
    }
}


TEST(md5, update)
{
    md5_hash hash("x");
    EXPECT_EQ(hash.hexdigest(), "9DD4E461268C8034F5C8564E155C67A6");

    hash.update("x");
    EXPECT_EQ(hash.hexdigest(), "9336EBF25087D91C818EE6E9EC29F8C1");
}


TEST(md5, fuzz)
{
    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<> dist(-128, 127);
    for (size_t i = 0; i < 50; i++) {
        const size_t length = rand() % 1000;
        std::string input;
        input.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            input.push_back(static_cast<char>(dist(engine)));
        }
        EXPECT_EQ(md5_hash(input).hexdigest().size(), 32);
    }
}


TEST(sha1, digest)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {
            "LOWER",
            "E2B5C793D8E006E471A5275E68ADFB98FE059DC4"
        },
        {
            "lower-/",
            "E0EF66C0EFC8DEC254491C7FC6D0D63D272094A4"
        },
        {
            "012345",
            "FDF8BC5814536F66012884E146A8887A44709A56"
        },
        {
            "aaaAA0aa",
            "B5E88447D8D3C41D1A02CC717E2595F08A83BB29"
        },
        {
            "This is a long message",
            "57846FFA4ABAD11215011F5B5D1452F949123762"
        },
        {
            std::string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "6E081B5948EFD1A97CECEE33A5186C2D9195A93A",
        },
        {
            std::string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "95E2FEA39A4E4BB3A73B7CFBF82C817E91F7F5DB",
        },
        {
            std::string {77, -61, -86, 109, 101},
            "35F7E32CF0651E8E38CCDFCECE594D4DFA34EE2B",
        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(sha1_hash(pair.first).hexdigest(), pair.second);
    }
}


TEST(sha1, update)
{
    sha1_hash hash("x");
    EXPECT_EQ(hash.hexdigest(), "11F6AD8EC52A2984ABAAFD7C3B516503785C2072");

    hash.update("x");
    EXPECT_EQ(hash.hexdigest(), "DD7B7B74EA160E049DD128478E074CE47254BDE8");
}


TEST(sha1, fuzz)
{
    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<> dist(-128, 127);
    for (size_t i = 0; i < 50; i++) {
        const size_t length = rand() % 1000;
        std::string input;
        input.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            input.push_back(static_cast<char>(dist(engine)));
        }
        EXPECT_EQ(sha1_hash(input).hexdigest().size(), 40);
    }
}


TEST(sha256, digest)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {
            "LOWER",
            "86543EDCC2F0F4CCD0057538969B1BFF79D03F4E3CE9807194398849359D5DB0"
        },
        {
            "lower-/",
            "C8C6F2A1034E2C30F2B5758411C6A84B700FC2A3B280996342A0EFB030840146"
        },
        {
            "012345",
            "2224512EF44A62E580BB1C0DCB33AFF688F4E7DA8A488AEB4E7CA402C5CACF45"
        },
        {
            "aaaAA0aa",
            "67B542863569F340BB3CF24F7ACAE07C0721DF3740C0E8394CE8FDAB146AC9BB"
        },
        {
            "This is a long message",
            "56514B402D7BF504C2EE559075D7CE290B4AC8EDD01CDBE4238F839048E4855D",
        },
        {
            std::string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "EA3252281BC3BCEC5672D33EC8C25DEE8ABD77235F85B8A049948F59DDF60B8D",
        },
        {
            std::string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "9992A572307E11690B104DB56E7689EFCDDD251A363B1588209FB907F27AFA31",
        },
        {
            std::string {77, -61, -86, 109, 101},
            "88A12C03ED7F6DE09626B834B45356DF89B1E716DBC05149C101EE14762589A4",
        }
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(sha256_hash(pair.first).hexdigest(), pair.second);
    }
}


TEST(sha256, update)
{
    sha256_hash hash("x");
    EXPECT_EQ(hash.hexdigest(), "2D711642B726B04401627CA9FBAC32F5C8530FB1903CC4DB02258717921A4881");

    hash.update("x");
    EXPECT_EQ(hash.hexdigest(), "5DDE896887F6754C9B15BFE3A441AE4806DF2FDE94001311E08BF110622E0BBE");
}


TEST(sha256, fuzz)
{
    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<> dist(-128, 127);
    for (size_t i = 0; i < 50; i++) {
        const size_t length = rand() % 1000;
        std::string input;
        input.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            input.push_back(static_cast<char>(dist(engine)));
        }
        EXPECT_EQ(sha256_hash(input).hexdigest().size(), 64);
    }
}
