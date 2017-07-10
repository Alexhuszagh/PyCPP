//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Base16 unittests.
 */

#include <pycpp/base16.h>
#include <gtest/gtest.h>
#include <random>
#include <utility>
#include <vector>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(base16, encode_decode)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {
            "LOWER",
            "4C4F574552"
        },
        {
            "lower-/",
            "6C6F7765722D2F"
        },
        {
            "aaaAA0aa",
            "6161614141306161"
        },
        {
            "This is a long message",
            "546869732069732061206C6F6E67206D657373616765"
        },
        {
            std::string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "ED959CEAB5ADEC96B4",
        },
        {
            std::string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "72C3A46B736DC3B67267C3A573",
        },
        {
            std::string {77, -61, -86, 109, 101},
            "4DC3AA6D65",
        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(base16_encode(pair.first), pair.second);
        EXPECT_EQ(base16_decode(pair.second), pair.first);
    }
}


TEST(base16, fuzz)
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
        auto encoded = base16_encode(input);
        EXPECT_EQ(base16_decode(encoded), input);
    }
}
