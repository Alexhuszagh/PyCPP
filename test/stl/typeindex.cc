//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL typeindex hash unittests.
 */

#include <pycpp/stl/typeindex.h>
#include <gtest/gtest.h>
#include <type_traits>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(typeindex, hash)
{
    using hash_type = hash<std::type_index>;
    using result_type = decltype(hash_type()(std::declval<std::type_index>()));
    static_assert(std::is_same<result_type, size_t>::value, "");
}
