//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator^()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_bit_xor.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_bit_xor, has_bit_xor)
{
    using p1 = pair<int, int>;

    static_assert(has_bit_xor<int>::value, "");
    static_assert(has_bit_xor<int, int>::value, "");
    static_assert(!has_bit_xor<p1>::value, "");
    static_assert(!has_bit_xor<p1, p1>::value, "");
    static_assert(!has_bit_xor<p1, int>::value, "");
    static_assert(!has_bit_xor<int, p1>::value, "");
}
