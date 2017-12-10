//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL basic_string alias unittests.
 */

#include <pycpp/stl/hash.h>
#include <pycpp/stl/string.h>
#include <gtest/gtest.h>
#include <type_traits>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(basic_string, basic_string)
{
    using basic_string_type = basic_string<char>;
    basic_string_type str;
}


TEST(basic_string, hash)
{
    using basic_string_type = basic_string<char>;
    using hash_type = hash<basic_string_type>;
    using result_type = decltype(hash_type()(std::declval<basic_string_type>()));
    static_assert(std::is_same<result_type, size_t>::value, "");
}
