//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `reference_wrapper` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/is_reference_wrapper.h>
#include <pycpp/stl/tuple.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(is_reference_wrapper, is_reference_wrapper)
{
    using ref = reference_wrapper<int>;
    using t1 = tuple<int>;
    using t2 = tuple<int, int>;
    using t3 = tuple<int, int, int>;

    static_assert(is_reference_wrapper<ref>::value, "");
    static_assert(!is_reference_wrapper<t1>::value, "");
    static_assert(!is_reference_wrapper<t2>::value, "");
    static_assert(!is_reference_wrapper<t3>::value, "");
    static_assert(!is_reference_wrapper<int>::value, "");
    static_assert(!is_reference_wrapper<char>::value, "");
}
