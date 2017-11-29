//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator^()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_bit_xor.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_bit_xor, has_bit_xor)
{
    using pair = std::pair<int, int>;

    static_assert(has_bit_xor<int>::value, "");
    static_assert(has_bit_xor<int, int>::value, "");
    static_assert(!has_bit_xor<pair>::value, "");
    static_assert(!has_bit_xor<pair, pair>::value, "");
    static_assert(!has_bit_xor<pair, int>::value, "");
    static_assert(!has_bit_xor<int, pair>::value, "");
}
