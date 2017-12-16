//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL typeindex hash unittests.
 */

#include <pycpp/stl/typeindex.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(typeindex, hash)
{
    using hash_type = hash<type_index>;
    using result_type = decltype(hash_type()(declval<type_index>()));
    static_assert(is_same<result_type, size_t>::value, "");
}
