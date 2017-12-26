//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `is_safe_overload` unittests.
 */

#include <pycpp/stl/detail/is_safe_overload.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

struct base {};
struct derived: base {};

// TESTS
// -----

TEST(is_safe_overload, is_safe_overload)
{
    // 2 args
    static_assert(is_safe_overload<true, base, int>::value, "");
    static_assert(!is_safe_overload<true, base, base>::value, "");
    static_assert(!is_safe_overload<true, base, derived>::value, "");
    static_assert(is_safe_overload<true, derived, base>::value, "");
    static_assert(!is_safe_overload<true, derived, derived>::value, "");

    // 3 args, only check a few
    static_assert(is_safe_overload<true, base, int, int>::value, "");
    static_assert(!is_safe_overload<true, base, base, int>::value, "");
    static_assert(!is_safe_overload<true, base, derived, int>::value, "");
    static_assert(is_safe_overload<true, base, int, base>::value, "");
}
