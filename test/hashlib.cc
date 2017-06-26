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
        EXPECT_EQ(md5_digest(pair.first), pair.second);
    }
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
        EXPECT_EQ(md5_digest(input).size(), 32);
    }
}
