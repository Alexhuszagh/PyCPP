//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `std::complex` SFINAE-detection unittests.
 */

#include <pycpp/sfinae/is_complex.h>
#include <gtest/gtest.h>
#include <tuple>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(is_complex, is_complex)
{
    using num = std::complex<int>;
    using t1 = std::tuple<int>;
    using t2 = std::tuple<int, int>;
    using t3 = std::tuple<int, int, int>;

    static_assert(is_complex<num>::value, "");
    static_assert(!is_complex<t1>::value, "");
    static_assert(!is_complex<t2>::value, "");
    static_assert(!is_complex<t3>::value, "");
    static_assert(!is_complex<int>::value, "");
    static_assert(!is_complex<char>::value, "");
}
