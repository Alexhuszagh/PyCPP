//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `is_relocatable` unittests.
 */

#include <pycpp/stl/detail/is_relocatable.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

struct empty {};
struct pod { int x; };
struct copyable: pod { copyable(const copyable&) = default; };
struct noncopyable: pod { noncopyable(const noncopyable&) {} };
struct virt { virtual ~virt() = default; };

// TESTS
// -----

TEST(is_relocatable, is_relocatable)
{
    static_assert(is_relocatable<empty>::value, "");
    static_assert(is_relocatable<pod>::value, "");
    static_assert(is_relocatable<copyable>::value, "");
    static_assert(!is_relocatable<noncopyable>::value, "");
    static_assert(!is_relocatable<virt>::value, "");
}
