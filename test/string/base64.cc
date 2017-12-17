//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Base64 unittests.
 */

#include <pycpp/stl/random.h>
#include <pycpp/stl/utility.h>
#include <pycpp/stl/vector.h>
#include <pycpp/string/base64.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(base64, encode_decode)
{
    vector<pair<std::string, std::string>> tests = {
        {
            "LOWER",
            "TE9XRVI="
        },
        {
            "lower-/",
            "bG93ZXItLw=="
        },
        {
            "aaaAA0aa",
            "YWFhQUEwYWE="
        },
        {
            "This is a long message",
            "VGhpcyBpcyBhIGxvbmcgbWVzc2FnZQ=="
        },
        {
            std::string {-19, -107, -100, -22, -75, -83, -20, -106, -76},
            "7ZWc6rWt7Ja0",
        },
        {
            std::string {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115},
            "csOka3Ntw7ZyZ8Olcw==",
        },
        {
            std::string {77, -61, -86, 109, 101},
            "TcOqbWU=",
        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(base64_encode(pair.first), pair.second);
        EXPECT_EQ(base64_decode(pair.second), pair.first);
    }
}


TEST(base64, fuzz)
{
    random_device device;
    default_random_engine engine(device());
    uniform_int_distribution<> dist(-128, 127);
    for (size_t i = 0; i < 50; i++) {
        const size_t length = rand() % 1000;
        std::string input;
        input.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            input.push_back(static_cast<char>(dist(engine)));
        }
        auto encoded = base64_encode(input);
        EXPECT_EQ(base64_decode(encoded), input);
    }
}
