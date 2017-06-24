//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Architecture unittests.
 */

#include "architecture.h"
#include <gtest/gtest.h>


// TESTS
// -----


TEST(architecture, system_architecture)
{
    EXPECT_EQ(SYSTEM_ARCHITECTURE, SYSTEM_ARCHITECTURE);
}
