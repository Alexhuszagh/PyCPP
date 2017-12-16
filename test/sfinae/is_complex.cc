//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `complex` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/is_complex.h>
#include <pycpp/stl/complex.h>
#include <pycpp/stl/tuple.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(is_complex, is_complex)
{
    using num = complex<int>;
    using t1 = tuple<int>;
    using t2 = tuple<int, int>;
    using t3 = tuple<int, int, int>;

    static_assert(is_complex<num>::value, "");
    static_assert(!is_complex<t1>::value, "");
    static_assert(!is_complex<t2>::value, "");
    static_assert(!is_complex<t3>::value, "");
    static_assert(!is_complex<int>::value, "");
    static_assert(!is_complex<char>::value, "");
}
