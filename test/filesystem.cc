//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Filesystem unittests.
 */

#include <pycpp/filesystem.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// CONSTANTS
// ---------

#if defined(OS_WINDOWS) || defined(OS_CYGWIN)
#   define DIRECTORY_SIZE 0
#else
#   define DIRECTORY_SIZE 8
#endif

// HELPERS
// -------


static void test_fd(fd_t fd)
{
    size_t size = 11;
    std::string in("Single Line");
    std::vector<char> out(size);

    EXPECT_NE(fd, INVALID_FD_VALUE);

    // fd_write
    EXPECT_EQ((size_t) fd_write(fd, in.data(), in.size()), size);
    EXPECT_EQ((size_t) fd_tell(fd), size);

    // fd_seek
    EXPECT_EQ((size_t) fd_seek(fd, 0, std::ios_base::beg), 0);

    // fd_read
    EXPECT_EQ((size_t) fd_read(fd, out.data(), out.size()), size);
    EXPECT_EQ(in, std::string(out.data(), out.size()));

    // fd_close
    EXPECT_EQ(fd_close(fd), 0);
}

// TESTS
// -----


TEST(directory_iterator, directory_iterator)
{

    directory_iterator first(path_t(path_prefix("test/files")));
    directory_iterator last;
    EXPECT_NE(first, last);
    for (; first != last; ++first) {
        EXPECT_TRUE(first->isfile());
        EXPECT_FALSE(first->isdir());
        EXPECT_FALSE(first->islink());
        EXPECT_TRUE(first->exists());
#if defined(OS_WINDOWS)                 // WINDOWS
        EXPECT_EQ(first->path(), path_t(path_prefix("test/files\\file")));
#else                                   // POSIX
        EXPECT_EQ(first->path(), path_t(path_prefix("test/files/file")));
#endif                                  // WINDOWS
        EXPECT_EQ(first->basename(), path_t(path_prefix("file")));
        EXPECT_EQ(first->dirname(), path_t(path_prefix("test/files")));
    }
}


TEST(directory_iterator, directory_iterator_shallow)
{
    path_list_t list;
    directory_iterator first(path_t(path_prefix("test/directory")));
    directory_iterator last;
    EXPECT_NE(first, last);
    for (; first != last; ++first) {
        list.emplace_back(first->path());
    }
    EXPECT_EQ(list.size(), 2);
}


TEST(directory_iterator, recursive_directory_iterator)
{
    path_list_t list;
    recursive_directory_iterator first(path_t(path_prefix("test/directory")));
    recursive_directory_iterator last;
    EXPECT_NE(first, last);
    for (; first != last; ++first) {
        EXPECT_TRUE(first->exists());
        list.emplace_back(first->path());
    }
    EXPECT_EQ(list.size(), 3);
}


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


TEST(path, join_path)
{
    path_list_t list;
    EXPECT_EQ(join_path(list), path_prefix(""));

#if defined(OS_WINDOWS)             // WINDOWS

    list = {u"\\tmp", u"path", u"to"};
    EXPECT_EQ(join_path(list), u"\\tmp\\path\\to");

    list = {u"\\tmp", u"\\path", u"to"};
    EXPECT_EQ(join_path(list), u"\\path\\to");

    list = {u"\\\\?\\D:", u"long"};
    EXPECT_EQ(join_path(list), u"\\\\?\\D:long");

    list = {u"\\\\?\\D:", u"\\long", u"\\x"};
    EXPECT_EQ(join_path(list), u"\\\\?\\D:\\x");

    list = {u"\\\\?\\D:", u"C:\\long", u"\\x"};
    EXPECT_EQ(join_path(list), u"C:\\x");

    list = {u"D:source", u"C:\\long", u"\\x"};
    EXPECT_EQ(join_path(list), u"C:\\x");

#else                               // POSIX

    list = {"/path", "to", "file"};
    EXPECT_EQ(join_path(list), "/path/to/file");

    list = {"/path", "/to", "file"};
    EXPECT_EQ(join_path(list), "/to/file");

#endif
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


TEST(directory, iterdir)
{
    auto r = iterdir("test/directory");
    ASSERT_EQ(r.distance(), 2);
}


TEST(directory, listdir)
{
    auto list = listdir("test/directory");
    ASSERT_EQ(list.size(), 2);
}


TEST(path, path_split)
{
    EXPECT_EQ(path_split("test").front(), "");
    EXPECT_EQ(path_split("test").back(), "test");
    EXPECT_EQ(path_split("test/").front(), "test");
    EXPECT_EQ(path_split("test/").back(), "");
    EXPECT_EQ(path_split("test/files").front(), "test");
    EXPECT_EQ(path_split("test/files").back(), "files");
    EXPECT_EQ(path_split("/usr/lib").front(), "/usr");
    EXPECT_EQ(path_split("/usr/lib").back(), "lib");
}


TEST(path, path_splitdrive)
{
    EXPECT_EQ(path_splitdrive("test/files").front(), "");
    EXPECT_EQ(path_splitdrive("test/files").back(), "test/files");
    EXPECT_EQ(path_splitdrive("/usr/lib").front(), "");
    EXPECT_EQ(path_splitdrive("/usr/lib").back(), "/usr/lib");

#if defined(OS_WINDOWS)
    EXPECT_EQ(path_splitdrive("c:/users").front(), "c:");
    EXPECT_EQ(path_splitdrive("c:/users").back(), "/users");
    EXPECT_EQ(path_splitdrive("c:..").front(), "c:");
    EXPECT_EQ(path_splitdrive("c:..").back(), "..");
    EXPECT_EQ(path_splitdrive("\\\\localhost").front(), "");
    EXPECT_EQ(path_splitdrive("\\\\localhost").back(), "\\\\localhost");
    EXPECT_EQ(path_splitdrive("\\\\localhost\\x").front(), "\\\\localhost\\x");
    EXPECT_EQ(path_splitdrive("\\\\localhost\\x").back(), "");
#endif
}


TEST(path, path_splitext)
{
    EXPECT_EQ(path_splitext(".").front(), ".");
    EXPECT_EQ(path_splitext(".").back(), "");
    EXPECT_EQ(path_splitext(".dat").front(), ".dat");
    EXPECT_EQ(path_splitext(".dat").back(), "");
    EXPECT_EQ(path_splitext("file.").front(), "file");
    EXPECT_EQ(path_splitext("file.").back(), ".");
    EXPECT_EQ(path_splitext("test/files").front(), "test/files");
    EXPECT_EQ(path_splitext("test/files").back(), "");
    EXPECT_EQ(path_splitext("/usr/lib").front(), "/usr/lib");
    EXPECT_EQ(path_splitext("/usr/lib").back(), "");
    EXPECT_EQ(path_splitext("test/files.dat").front(), "test/files");
    EXPECT_EQ(path_splitext("test/files.dat").back(), ".dat");
    EXPECT_EQ(path_splitext("test/.dat").front(), "test/.dat");
    EXPECT_EQ(path_splitext("test/.dat").back(), "");
}


TEST(path, path_splitunc)
{
#if defined(OS_WINDOWS)
    EXPECT_EQ(path_splitunc("c:/users").front(), "");
    EXPECT_EQ(path_splitunc("c:/users").back(), "c:/users");
    EXPECT_EQ(path_splitunc("\\\\localhost").front(), "");
    EXPECT_EQ(path_splitunc("\\\\localhost").back(), "\\\\localhost");
    EXPECT_EQ(path_splitunc("\\\\localhost\\x").front(), "\\\\localhost\\x");
    EXPECT_EQ(path_splitunc("\\\\localhost\\x").back(), "");
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


TEST(path, gethomedir)
{
    EXPECT_GE(gethomedir().size(), 1);
    EXPECT_EQ(gethomedir(), homedir);
    homedir = path_prefix("/home/user");
    EXPECT_EQ(gethomedir(), homedir);
}


TEST(path, gettempdir)
{
    EXPECT_GE(gettempdir().size(), 1);
    EXPECT_EQ(gettempdir(), tempdir);
    tempdir = path_prefix("/tmp");
    EXPECT_EQ(gettempdir(), tempdir);
}


TEST(path, gettempprefix)
{
    EXPECT_GE(gettempprefix().size(), 1);
    EXPECT_EQ(gettempprefix(), tempprefix);
    tempprefix = path_prefix("tmp");
    EXPECT_EQ(gettempprefix(), tempprefix);
}


TEST(path, gettempnam)
{
    path_t path = gettempnam();
    auto suffix = base_name(path).substr(tempprefix.size());
    EXPECT_EQ(base_name(path).size(), tempprefix.size() + TMP_SUFFIX_LENGTH);
    EXPECT_EQ(dir_name(path), gettempdir());
    EXPECT_TRUE(std::all_of(suffix.begin(), suffix.end(), [](native_char_type c) {
        return TMP_SUFFIX_CHARACTERS.find(c) != std::string::npos;
    }));
}


TEST(fd, fd_utils)
{
    std::string path("sample_path");
    std::ios_base::openmode iomode = std::ios_base::in | std::ios_base::out;
    {
        // generic
        fd_t fd = fd_open(path, iomode);
        test_fd(fd);
        EXPECT_TRUE(remove_file(path));
    }
    {
        // permissions
        fd_t fd = fd_open(path, iomode, S_IWR_USR_GRP);
        test_fd(fd);
        EXPECT_TRUE(remove_file(path));
    }
    {
        // normal
        fd_t fd = fd_open(path, iomode, S_IWR_USR_GRP, access_normal);
        test_fd(fd);
        EXPECT_TRUE(remove_file(path));
    }
    {
        // sequential
        fd_t fd = fd_open(path, iomode, S_IWR_USR_GRP, access_sequential);
        test_fd(fd);
        EXPECT_TRUE(remove_file(path));
    }
    {
        // random-access
        fd_t fd = fd_open(path, iomode, S_IWR_USR_GRP, access_random);
        test_fd(fd);
        EXPECT_TRUE(remove_file(path));
    }
}
