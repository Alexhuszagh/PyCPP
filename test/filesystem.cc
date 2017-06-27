//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Filesystem unittests.
 */

#include "filesystem.h"
#include <gtest/gtest.h>

// CONSTANTS
// ---------

#if defined(OS_WINDOWS)
#   define DIRECTORY_SIZE 0
#else
#   define DIRECTORY_SIZE 8
#endif

// TESTS
// -----


TEST(stat, stat)
{
    auto s = stat("test/files");
    EXPECT_GE(s.st_mode, 0);

    s = stat("test/files/file");
    EXPECT_GE(s.st_mode, 0);
}


TEST(stat, lstat)
{
    // lstat works, but isn't able to work with intgration
    // testing. On WINE, symbolic links are automatically resolved,
    // and on Tea-CI, symlinks cannot be created.
    auto s = lstat("test/files");
    EXPECT_GE(s.st_mode, 0);

    s = lstat("test/files/file");
    EXPECT_GE(s.st_mode, 0);
}


TEST(stat, getatime)
{
    EXPECT_GE(getatime("test/files"), 1498529140);
    EXPECT_GE(getatime("test/files/file"), 1498520203);
}


TEST(stat, getmtime)
{
    EXPECT_GE(getmtime("test/files"), 1498529140);
    EXPECT_GE(getmtime("test/files/file"), 1498520203);
}


TEST(stat, getctime)
{
    EXPECT_GE(getmtime("test/files"), 1498529140);
    EXPECT_GE(getmtime("test/files/file"), 1498520203);
}


TEST(stat, getsize)
{
    EXPECT_GE(getsize("test/files"), DIRECTORY_SIZE);
    EXPECT_EQ(getsize("test/files/file"), 0);
}


TEST(stat, isfile)
{
    EXPECT_FALSE(isfile("test/files"));
    EXPECT_TRUE(isfile("test/files/file"));
}


TEST(stat, isdir)
{
    EXPECT_TRUE(isdir("test/files"));
    EXPECT_FALSE(isdir("test/files/file"));
}


TEST(stat, islink)
{
    // symbolic link works, but isn't able to work with intgration
    // testing. On WINE, symbolic links are automatically resolved,
    // and on Tea-CI, symlinks cannot be created.
    EXPECT_FALSE(islink("test/files"));
    EXPECT_FALSE(islink("test/files/file"));
}


TEST(stat, ismount)
{}


TEST(stat, exists)
{
    EXPECT_TRUE(exists("test/files"));
    EXPECT_TRUE(exists("test/files/file"));
    EXPECT_FALSE(exists("test/files/missing"));
}
