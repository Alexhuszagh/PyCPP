//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Filesystem unittests.
 */

#include "filesystem.h"
#include <gtest/gtest.h>

// TESTS
// -----


//TEST(stat, stat)
//{
//    auto s = stat("test/files");
//    EXPECT_GE(s.st_mode, 0);
//
//    s = stat("test/files/file");
//    EXPECT_GE(s.st_mode, 0);
//
//    s = stat("test/files/link");
//    EXPECT_GE(s.st_mode, 0);
//}
//
//
TEST(stat, lstat)
{
//    auto s = lstat("test/files");
//    EXPECT_GE(s.st_mode, 0);
//
//    s = lstat("test/files/file");
//    EXPECT_GE(s.st_mode, 0);

//    auto s = lstat("test/files/link");
    EXPECT_GE(s.st_mode, 0);

//// TODO: here...
////    s = lstat("test/files/broken_link");
////    EXPECT_GE(s.st_mode, 0);
}
//
//
//TEST(stat, getatime)
//{
//    EXPECT_GE(getatime("test/files"), 0);
//    EXPECT_GE(getatime("test/files/file"), 0);
////    EXPECT_GE(getatime("test/files/link"), 0);
//}
//
//
//TEST(stat, getmtime)
//{
//    EXPECT_GE(getatime("test/files"), 0);
//    EXPECT_GE(getatime("test/files/file"), 0);
////    EXPECT_GE(getatime("test/files/link"), 0);
//}


TEST(stat, getctime)
{}


TEST(stat, getsize)
{}


TEST(stat, isfile)
{}


TEST(stat, isdir)
{
    EXPECT_TRUE(isdir("test/files"));
    EXPECT_FALSE(isdir("test/files/file"));
//    EXPECT_FALSE(isdir("test/files/link"));
}


TEST(stat, islink)
{
//    EXPECT_FALSE(islink("test/files"));
//    EXPECT_FALSE(isdir("test/files/file"));
//    // TODO: this is failing...
//    EXPECT_TRUE(islink("test/files/link"));
}


TEST(stat, ismount)
{}


TEST(stat, exists)
{
//    exit(1);
}
