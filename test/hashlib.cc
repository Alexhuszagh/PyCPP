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
