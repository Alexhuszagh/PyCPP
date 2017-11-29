//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator^=()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_bit_xor_assign.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_bit_xor_assign, has_bit_xor_assign)
{
    using pair = std::pair<int, int>;

    static_assert(has_bit_xor_assign<int>::value, "");
    static_assert(has_bit_xor_assign<int, int>::value, "");
    static_assert(!has_bit_xor_assign<pair>::value, "");
    static_assert(!has_bit_xor_assign<pair, pair>::value, "");
    static_assert(!has_bit_xor_assign<pair, int>::value, "");
    static_assert(!has_bit_xor_assign<int, pair>::value, "");
}
