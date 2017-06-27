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

#if defined(OS_WINDOWS) || defined(OS_CYGWIN)
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


TEST(stat, exists)
{
    EXPECT_TRUE(exists("test/files"));
    EXPECT_TRUE(exists("test/files/file"));
    EXPECT_FALSE(exists("test/files/missing"));
}


TEST(stat, samestat)
{
    auto s1 = stat("test/files");
    auto s2 = stat("test/files");
    auto s3 = stat("test/files/file");
    auto s4 = stat("test/files/file");
    EXPECT_TRUE(samestat(s1, s2));
    EXPECT_TRUE(samestat(s3, s4));
    EXPECT_FALSE(samestat(s1, s3));
}


TEST(path, isabs)
{
    EXPECT_TRUE(isabs("/usr"));
    EXPECT_FALSE(isabs("test/files"));
    EXPECT_FALSE(isabs("test/files/file"));
}


TEST(path, samefile)
{
    EXPECT_TRUE(samefile("test/files", "test/files"));
    EXPECT_TRUE(samefile("test/files/file", "test/files/file"));
    EXPECT_FALSE(samefile("test/files", "test/files/file"));
}


TEST(path, split)
{
    EXPECT_EQ(split("test").front(), "");
    EXPECT_EQ(split("test").back(), "test");
    EXPECT_EQ(split("test/").front(), "test");
    EXPECT_EQ(split("test/").back(), "");
    EXPECT_EQ(split("test/files").front(), "test");
    EXPECT_EQ(split("test/files").back(), "files");
    EXPECT_EQ(split("/usr/lib").front(), "/usr");
    EXPECT_EQ(split("/usr/lib").back(), "lib");
}


TEST(path, splitdrive)
{
    EXPECT_EQ(splitdrive("test/files").front(), "");
    EXPECT_EQ(splitdrive("test/files").back(), "test/files");
    EXPECT_EQ(splitdrive("/usr/lib").front(), "");
    EXPECT_EQ(splitdrive("/usr/lib").back(), "/usr/lib");

#if defined(OS_WINDOWS)
    EXPECT_EQ(splitdrive("c:/users").front(), "c:");
    EXPECT_EQ(splitdrive("c:/users").back(), "/users");
    EXPECT_EQ(splitdrive("c:..").front(), "c:");
    EXPECT_EQ(splitdrive("c:..").back(), "..");
    EXPECT_EQ(splitdrive("\\\\localhost").front(), "");
    EXPECT_EQ(splitdrive("\\\\localhost").back(), "\\\\localhost");
    EXPECT_EQ(splitdrive("\\\\localhost\\x").front(), "\\\\localhost\\x");
    EXPECT_EQ(splitdrive("\\\\localhost\\x").back(), "");
#endif
}


TEST(path, splitext)
{
    EXPECT_EQ(splitext(".").front(), ".");
    EXPECT_EQ(splitext(".").back(), "");
    EXPECT_EQ(splitext(".dat").front(), ".dat");
    EXPECT_EQ(splitext(".dat").back(), "");
    EXPECT_EQ(splitext("file.").front(), "file");
    EXPECT_EQ(splitext("file.").back(), ".");
    EXPECT_EQ(splitext("test/files").front(), "test/files");
    EXPECT_EQ(splitext("test/files").back(), "");
    EXPECT_EQ(splitext("/usr/lib").front(), "/usr/lib");
    EXPECT_EQ(splitext("/usr/lib").back(), "");
    EXPECT_EQ(splitext("test/files.dat").front(), "test/files");
    EXPECT_EQ(splitext("test/files.dat").back(), ".dat");
    EXPECT_EQ(splitext("test/.dat").front(), "test/.dat");
    EXPECT_EQ(splitext("test/.dat").back(), "");
}


TEST(path, splitunc)
{
#if defined(OS_WINDOWS)
    EXPECT_EQ(splitunc("c:/users").front(), "");
    EXPECT_EQ(splitunc("c:/users").back(), "c:/users");
    EXPECT_EQ(splitunc("\\\\localhost").front(), "");
    EXPECT_EQ(splitunc("\\\\localhost").back(), "\\\\localhost");
    EXPECT_EQ(splitunc("\\\\localhost\\x").front(), "\\\\localhost\\x");
    EXPECT_EQ(splitunc("\\\\localhost\\x").back(), "");
#endif
}


TEST(path, abspath)
{
    EXPECT_EQ(abspath("/usr/lib"), "/usr/lib");
    EXPECT_NE(abspath("usr/lib"), "usr/lib");
}


TEST(path, base_name)
{
    EXPECT_EQ(base_name("/usr/lib"), "lib");
    EXPECT_EQ(base_name("/usr/lib.dat"), "lib.dat");
    EXPECT_EQ(base_name("/usr/lib/"), "");
}


TEST(path, dir_name)
{
    EXPECT_EQ(dir_name("/usr/lib"), "/usr");
    EXPECT_EQ(dir_name("/usr/lib.dat"), "/usr");
    EXPECT_EQ(dir_name("/usr/lib/"), "/usr/lib");
}


TEST(path, expanduser)
{
    EXPECT_NE(expanduser("~/"), "~/");
    EXPECT_EQ(expanduser("~."), "~.");
    EXPECT_EQ(expanduser("~file"), "~file");
}


TEST(path, expandvars)
{
#if defined(OS_WINDOWS)             // WINDOWS
    EXPECT_NE(expandvars("%SYSTEMROOT%/path"), "%SYSTEMROOT%/path");
    EXPECT_EQ(expandvars("%UNSET%/path"), "%UNSET%/path");
#else                               // POSIX
    EXPECT_NE(expandvars("${HOME}/path"), "${HOME}/path");
    EXPECT_NE(expandvars("${UNSET}/path"), "${UNSET}/path");
#endif
}


TEST(path, normcase)
{
#if defined(OS_WINDOWS)             // WINDOWS
    EXPECT_EQ(normcase("c:/users"), "c:\\users");
    EXPECT_EQ(normcase("C:/Users"), "c:\\users");
#else                               // POSIX
    EXPECT_EQ(normcase("/usr/lib"), "/usr/lib");
    EXPECT_EQ(normcase("/usr/Lib"), "/usr/Lib");
#endif
}


TEST(path, normpath)
{
#if defined(OS_WINDOWS)             // WINDOWS
    EXPECT_EQ(normpath("C:\\.."), "C:\\");
    EXPECT_EQ(normpath("C:.."), "C:..");
    EXPECT_EQ(normpath("C:\\Users\\.."), "C:\\");
#else                               // POSIX
    EXPECT_EQ(normpath("/.."), "/");
    EXPECT_EQ(normpath(".."), "..");
    EXPECT_EQ(normpath("../."), "..");
    EXPECT_EQ(normpath("."), ".");
    EXPECT_EQ(normpath("./."), ".");
    EXPECT_EQ(normpath("/usr/lib"), "/usr/lib");
    EXPECT_EQ(normpath("/usr/lib/.."), "/usr");
    EXPECT_EQ(normpath("/usr/./lib/.."), "/usr");
    EXPECT_EQ(normpath("test/.."), ".");
    EXPECT_EQ(normpath("test/../.."), "..");
#endif
}


TEST(path, relpath)
{
    EXPECT_EQ(relpath("/usr/lib", "/usr/"), "lib");
    EXPECT_EQ(relpath("/usr/lib", "/usr"), "lib");
}
