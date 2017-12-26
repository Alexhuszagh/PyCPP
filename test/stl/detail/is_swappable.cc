//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `is_swappable` unittests.
 */

#include <pycpp/stl/detail/is_swappable.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

struct s1 { void swap(s1&) noexcept; };
struct s2 { void swap(s1&) noexcept; };
struct s3 { void swap(s3&) noexcept; };
struct s4 { void swap(s1&); };
struct s5 { void swap(s5&); };

void swap(s1&, s1&) noexcept;
void swap(s2&, s1&) noexcept;
void swap(s3&, s3&) noexcept;
void swap(s4&, s1&);
void swap(s5&, s5&);

// TESTS
// -----

TEST(is_swappable, is_swappable_with)
{
    static_assert(is_swappable_with<s1, s1>::value, "");
    static_assert(is_swappable_with<s2, s1>::value, "");
    static_assert(is_swappable_with<s3, s3>::value, "");
    static_assert(is_swappable_with<s4, s1>::value, "");
    static_assert(is_swappable_with<s5, s5>::value, "");
    static_assert(!is_swappable_with<s2, s2>::value, "");
    static_assert(!is_swappable_with<s4, s4>::value, "");
}


TEST(is_nothrow_swappable_with, is_nothrow_swappable_with)
{
    static_assert(is_nothrow_swappable_with<s1, s1>::value, "");
    static_assert(is_nothrow_swappable_with<s2, s1>::value, "");
    static_assert(is_nothrow_swappable_with<s3, s3>::value, "");
    static_assert(!is_nothrow_swappable_with<s4, s1>::value, "");
    static_assert(!is_nothrow_swappable_with<s5, s5>::value, "");
    static_assert(!is_nothrow_swappable_with<s2, s2>::value, "");
    static_assert(!is_nothrow_swappable_with<s4, s4>::value, "");
}


TEST(is_swappable, is_swappable)
{
    static_assert(is_swappable<s1>::value, "");
    static_assert(!is_swappable<s2>::value, "");
    static_assert(is_swappable<s3>::value, "");
    static_assert(!is_swappable<s4>::value, "");
    static_assert(is_swappable<s5>::value, "");
}


TEST(is_nothrow_swappable, is_nothrow_swappable)
{
    static_assert(is_nothrow_swappable<s1>::value, "");
    static_assert(!is_nothrow_swappable<s2>::value, "");
    static_assert(is_nothrow_swappable<s3>::value, "");
    static_assert(!is_nothrow_swappable<s4>::value, "");
    static_assert(!is_nothrow_swappable<s5>::value, "");
}
