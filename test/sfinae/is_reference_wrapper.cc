//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `std::reference_wrapper` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/is_reference_wrapper.h>
#include <gtest/gtest.h>
#include <tuple>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(is_reference_wrapper, is_reference_wrapper)
{
    using ref = std::reference_wrapper<int>;
    using t1 = std::tuple<int>;
    using t2 = std::tuple<int, int>;
    using t3 = std::tuple<int, int, int>;

    static_assert(is_reference_wrapper<ref>::value, "");
    static_assert(!is_reference_wrapper<t1>::value, "");
    static_assert(!is_reference_wrapper<t2>::value, "");
    static_assert(!is_reference_wrapper<t3>::value, "");
    static_assert(!is_reference_wrapper<int>::value, "");
    static_assert(!is_reference_wrapper<char>::value, "");
}
