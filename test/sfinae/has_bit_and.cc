//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator&()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_bit_and.h>
#include <gtest/gtest.h>
#include <utility>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_bit_and, has_bit_and)
{
    using pair = std::pair<int, int>;

    static_assert(has_bit_and<int>::value, "");
    static_assert(has_bit_and<int, int>::value, "");
    static_assert(!has_bit_and<pair>::value, "");
    static_assert(!has_bit_and<pair, pair>::value, "");
    static_assert(!has_bit_and<pair, int>::value, "");
    static_assert(!has_bit_and<int, pair>::value, "");
}
