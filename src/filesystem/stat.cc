//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "filesystem.h"
#include "filesystem/exception.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(OS_WINDOWS)
#   include "windows.h"
#endif

// MACROS
// -------

#ifndef S_IFLNK
#   define S_IFLNK 0120000
#endif

#ifndef S_ISREG
#   define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif

#ifndef S_ISDIR
#   define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

#ifndef S_ISCHR
#   define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR)
#endif

#ifndef S_ISBLK
#   define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK)
#endif

#ifndef S_ISFIFO
#   define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#endif

#ifndef S_ISLNK
#   define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
#endif

#ifndef S_ISSOCK
#   define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
#endif

#ifndef S_ISDOOR
#   define S_ISDOOR(m) (0)
#endif

#ifndef S_ISPORT
#   define S_ISPORT(m) (0)
#endif

#ifndef S_ISWHT
#   define S_ISWHT(m) (0)
#endif

// HELPERS
// -------

// STAT


static void handle_error(int code)
{
    switch (code) {
        case 0:
            return;
        case ENOENT:
            throw filesystem_error(filesystem_file_not_found);
         case EINVAL:
            throw filesystem_error(filesystem_invalid_parameter);
        case ENOMEM:
            throw filesystem_error(filesystem_out_of_memory);
         default:
            throw filesystem_error(filesystem_unexpected_error);
    }
}


#if defined(OS_WINDOWS)         // WINDOWS


//HANDLE get_file_handle(const std::string &path,
//    DWORD dwDesiredAccess = 0,
//    DWORD dwShareMode = SHARE_ALL,
//    LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr,
//    DWORD dwCreationDisposition = OPEN_EXISTING,
//    DWORD dwFlagsAndAttributes = FILE_FLAG_BACKUP_SEMANTICS,
//    HANDLE hTemplateFile = nullptr);
//HANDLE get_file_handle(const std::string &path,
//    DWORD dwDesiredAccess,
//    DWORD dwShareMode,
//    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
//    DWORD dwCreationDisposition,
//    DWORD dwFlagsAndAttributes,
//    HANDLE hTemplateFile)
//{
//    return CreateFileW(WIDE(path).data(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
//}
//
///** Create file handle with attribute-only permissions and read
// *  device information.
// */
//BY_HANDLE_FILE_INFORMATION get_file_info(const std::string &path)
//{
//    BY_HANDLE_FILE_INFORMATION info;
//    auto handle = get_file_handle(path);
//    if(handle != INVALID_HANDLE_VALUE) {
//        GetFileInformationByHandle(handle, &info);
//    }
//
//    return info;
//}



static int stat(const char* path, stat_t* buffer, bool do_lstat)
{
    // TODO: here..
}


static int wstat(const wchar_t* path, stat_t* buffer, bool do_lstat)
{
    // TODO: here..
}

    // GetFileInformationByHandle
    // TODO: here...
//    code = stat(path.data(), &sb);
//    handle_error(code);
//    copy_native(sb, data);

// TODO: need to check here...
// https://msdn.microsoft.com/en-us/library/windows/desktop/gg258117(v=vs.85).aspx
// https://github.com/joyent/libuv/blob/1dc2709b999a84520ab1b3c56c0e082bf8617c1f/src/win/fs.c#L971


//INLINE static void fs__stat_impl(uv_fs_t* req, int do_lstat) {
//  HANDLE handle;
//  DWORD flags;
//
//  flags = FILE_FLAG_BACKUP_SEMANTICS;
//  if (do_lstat) {
//    flags |= FILE_FLAG_OPEN_REPARSE_POINT;
//  }
//
//  handle = CreateFileW(req->pathw,
//                       FILE_READ_ATTRIBUTES,
//                       FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
//                       NULL,
//                       OPEN_EXISTING,
//                       flags,
//                       NULL);
//  if (handle == INVALID_HANDLE_VALUE) {
//    SET_REQ_WIN32_ERROR(req, GetLastError());
//    return;
//  }
//
//  if (fs__stat_handle(handle, &req->statbuf) != 0) {
//    DWORD error = GetLastError();
//    if (do_lstat && error == ERROR_SYMLINK_NOT_SUPPORTED) {
//      /* We opened a reparse point but it was not a symlink. Try again. */
//      fs__stat_impl(req, 0);
//
//    } else {
//      /* Stat failed. */
//      SET_REQ_WIN32_ERROR(req, GetLastError());
//    }
//
//    CloseHandle(handle);
//    return;
//  }
//
//  req->ptr = &req->statbuf;
//  req->result = 0;
//  CloseHandle(handle);
//}


stat_t stat(const path_t& path)
{
    stat_t data;
    int code;

    auto buffer = reinterpret_cast<const wchar_t*>(path.data());
    code = wstat(buffer, &data, false);
    handle_error(code);

    return data;
}


stat_t lstat(const path_t& path)
{
    stat_t data;
    int code;

    auto buffer = reinterpret_cast<const wchar_t*>(path.data());
    code = wstat(buffer, &data, true);
    handle_error(code);

    return data;
}

#else                           // POSIX


static void copy_native(const struct stat& src, stat_t& dst)
{
    dst.st_dev = src.st_dev;
    dst.st_ino = src.st_ino;
    dst.st_mode = src.st_mode;
    dst.st_nlink = src.st_nlink;
    dst.st_uid = src.st_uid;
    dst.st_gid = src.st_gid;
    dst.st_rdev = src.st_rdev;
    dst.st_size = src.st_size;

    // ignore the missing precision for backwards compatibility
    dst.st_atim = {src.st_atime, 0};
    dst.st_mtim = {src.st_mtime, 0};
    dst.st_ctim = {src.st_ctime, 0};
}


stat_t stat(const path_t& path)
{
    struct stat sb;
    stat_t data;
    int code;

    code = ::stat(path.c_str(), &sb);
    handle_error(code);
    copy_native(sb, data);

    return data;
}


stat_t lstat(const path_t& path)
{
    struct stat sb;
    stat_t data;
    int code;

    code = ::lstat(path.c_str(), &sb);
    handle_error(code);
    copy_native(sb, data);

    return data;
}

#endif

#if defined(backup_path_t)          // BACKUP PATH

stat_t stat(const backup_path_t& path)
{
    stat_t data;
    int code;

    code = stat(path.data(), &data, false);
    handle_error(code);

    return data;
}


stat_t lstat(const backup_path_t& path)
{
    stat_t data;
    int code;

    code = stat(path.data(), &data, false);
    handle_error(code);

    return data;
}

#endif

// STAT PROPERTIES

static time_t getatime(const stat_t& s)
{
    return s.st_atim.tv_sec;
}


static time_t getmtime(const stat_t& s)
{
    return s.st_mtim.tv_sec;
}


static time_t getctime(const stat_t& s)
{
    return s.st_ctim.tv_sec;
}


static off_t getsize(const stat_t& s)
{
    return s.st_size;
}


static bool exists_stat(const stat_t& s)
{
    return true;
}


static bool isfile_stat(const stat_t& s)
{
    return S_ISREG(s.st_mode);
}


static bool isdir_stat(const stat_t& s)
{
    return S_ISDIR(s.st_mode);
}


static bool islink_stat(const stat_t& s)
{
    return S_ISLNK(s.st_mode);
}

// PATH PROPERTIES


template <typename Path, typename Function>
static bool check_impl(const Path& path, Function function)
{
    try {
        auto s = stat(path);
        return function(s);
    } catch (filesystem_error& error) {
        if (error.code() == filesystem_file_not_found) {
            return false;
        }
        throw;
    }
}


template <typename Path>
static bool exists_impl(const Path& path)
{
    return check_impl(path, exists_stat);
}


template <typename Path>
static bool isfile_impl(const Path& path)
{
    return check_impl(path, isfile_stat);
}


template <typename Path>
static bool isdir_impl(const Path& path)
{
    return check_impl(path, isdir_stat);
}


template <typename Path>
static bool islink_impl(const Path& path)
{
    return check_impl(path, islink_stat);
}

// FUNCTIONS
// ---------


time_t getatime(const path_t& path)
{
    return getatime(stat(path));
}


time_t getmtime(const path_t& path)
{
    return getmtime(stat(path));
}


time_t getctime(const path_t& path)
{
    return getctime(stat(path));
}


off_t getsize(const path_t& path)
{
    return getsize(stat(path));
}


bool isfile(const path_t& path)
{
    return isfile_impl(path);
}


bool isdir(const path_t& path)
{
    return isdir_impl(path);
}


bool islink(const path_t& path)
{
    return islink_impl(path);
}

////bool ismount(const path_t& path);

bool exists(const path_t& path)
{
    return exists_impl(path);
}


#if defined(backup_path_t)          // BACKUP PATH


time_t getatime(const backup_path_t& path)
{
    return getatime(stat(path));
}


time_t getmtime(const backup_path_t& path)
{
    return getmtime(stat(path));
}


time_t getctime(const backup_path_t& path)
{
    return getctime(stat(path));
}


off_t getsize(const backup_path_t& path)
{
    return getmtime(stat(path));
}


bool isfile(const backup_path_t& path)
{
    return isfile_impl(path);
}


bool isdir(const backup_path_t& path)
{
    return isdir_impl(path);
}


bool islink(const backup_path_t& path)
{
    return islink_impl(path);
}

////bool ismount(const backup_path_t& path);

bool exists(const backup_path_t& path)
{
    return exists_impl(path);
}

#endif
