//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL system_error hash unittests.
 */

#include <pycpp/stl/system_error.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(error_code, hash)
{
    using hash_type = hash<error_code>;
    using result_type = decltype(hash_type()(declval<error_code>()));
    static_assert(is_same<result_type, size_t>::value, "");
}


#if defined(HAVE_CPP17)

TEST(error_condition, hash)
{
    using hash_type = hash<error_condition>;
    using result_type = decltype(hash_type()(declval<error_condition>()));
    static_assert(is_same<result_type, size_t>::value, "");
}

#endif          // HAVE_CPP17
