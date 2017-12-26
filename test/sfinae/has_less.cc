//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `operator<()` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/has_less.h>
#include <pycpp/stl/tuple.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(has_less, has_less)
{
    using p1 = pair<int, int>;
    using t1 = tuple<int>;
    using t2 = tuple<int, int>;
    using t3 = tuple<int, int, int>;

    static_assert(has_less<p1>::value, "");
    static_assert(has_less<int>::value, "");
    static_assert(has_less<char>::value, "");
    static_assert(!has_less<p1, t1>::value, "");
    static_assert(!has_less<p1, t2>::value, "");
    static_assert(!has_less<p1, t3>::value, "");
    static_assert(!has_less<p1, int>::value, "");
    static_assert(!has_less<p1, char>::value, "");

#if !defined(HAVE_MSVC)
    static_assert(has_less<t1>::value, "");
    static_assert(has_less<t2>::value, "");
    static_assert(has_less<t3>::value, "");
#endif      // HAVE_MSVC
}
