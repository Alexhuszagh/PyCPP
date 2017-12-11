//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL system_error hash unittests.
 */

#include <pycpp/stl/system_error.h>
#include <gtest/gtest.h>
#include <type_traits>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(error_code, hash)
{
    using hash_type = hash<std::error_code>;
    using result_type = decltype(hash_type()(std::declval<std::error_code>()));
    static_assert(std::is_same<result_type, size_t>::value, "");
}


#if defined(HAVE_CPP17)

TEST(error_condition, hash)
{
    using hash_type = hash<std::error_condition>;
    using result_type = decltype(hash_type()(std::declval<std::error_condition>()));
    static_assert(std::is_same<result_type, size_t>::value, "");
}

#endif          // HAVE_CPP17
