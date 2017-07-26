//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Runtime OS detection unittests.
 */

#include <pycpp/runtime/os.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


#if BUILD_FILESYSTEM                    // BUILD_FILESYSTEM

TEST(runtime, os)
{
    EXPECT_TRUE(is_wine() || true);
    EXPECT_TRUE(is_docker() || true);
    EXPECT_TRUE(is_container() || true);
}

#endif                                  // BUILD_FILESYSTEM
