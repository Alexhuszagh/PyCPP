//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL thread hash unittests.
 */

#include <pycpp/stl/thread.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(thread, hash)
{
    using hash_type = hash<thread::id>;
    using result_type = decltype(hash_type()(declval<thread::id>()));
    static_assert(is_same<result_type, size_t>::value, "");
}
