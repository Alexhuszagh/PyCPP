//  :copyright: (c) 2017 Joyent, Inc. and other Node contributors.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "filesystem.h"
#include "filesystem/exception.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(OS_WINDOWS)
#   include "windows.h"
#   include "winerror.h"
#   include <tuple>
#endif

// MACROS
// -------

#ifndef S_IREAD
#   define S_IREAD _S_IREAD
#endif

#ifndef S_IWRITE
#   define S_IWRITE _S_IWRITE
#endif

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


#if defined(OS_WINDOWS)
#   ifndef ERROR_SYMLINK_NOT_SUPPORTED
#       define ERROR_SYMLINK_NOT_SUPPORTED 1464
#   endif
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


#define FILETIME_TO_UINT(filetime)                                          \
   (*((uint64_t*) &(filetime)) - 116444736000000000ULL)


#define FILETIME_TO_TIME_T(filetime)                                        \
   (FILETIME_TO_UINT(filetime) / 10000000ULL)

#define FILETIME_TO_TIME_NS(filetime, secs)                                 \
   ((FILETIME_TO_UINT(filetime) - (secs * 10000000ULL)) * 100)


#define FILETIME_TO_TIMESPEC(ts, filetime)                                  \
   do {                                                                     \
     (ts).tv_sec = (long) FILETIME_TO_TIME_T(filetime);                     \
     (ts).tv_nsec = (long) FILETIME_TO_TIME_NS(filetime, (ts).tv_sec);      \
   } while(0)


/**
 *  \brief Map GetLastError to errno.
 */
static int get_error_code()
{
    DWORD error = GetLastError();
    switch (error) {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
            return ENOENT;
        case ERROR_BAD_ARGUMENTS:
            return EINVAL;
        case ERROR_TOO_MANY_OPEN_FILES:
            return EMFILE;
        case ERROR_ACCESS_DENIED:
            return EACCES;
        case ERROR_NOT_ENOUGH_MEMORY:
        case ERROR_OUTOFMEMORY:
            return ENOMEM;
        default:
            return -1;
    }
}


static int copy_stat(HANDLE handle, stat_t* buffer)
{
    BY_HANDLE_FILE_INFORMATION info;
    if (!GetFileInformationByHandle(handle, &info)) {
        return get_error_code();
    }

    // set our information
    buffer->st_dev = info.dwVolumeSerialNumber;

    ULARGE_INTEGER ul;
    buffer->st_mode = 0;
    if (info.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
        buffer->st_mode |= S_IFLNK;
        buffer->st_size = 0;
    } else if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        buffer->st_mode |= S_IFDIR;
        buffer->st_size = 0;
    } else {
        ul.HighPart = info.nFileSizeHigh;
        ul.LowPart = info.nFileSizeLow;
        buffer->st_mode |= S_IFREG;
        buffer->st_size = ul.QuadPart;
    }

    // get permissions
    int mode;
    if (info.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
        mode = S_IREAD;
    } else {
        mode = S_IREAD | S_IWRITE;
    }
    buffer->st_mode |= mode | (mode >> 3) | (mode >> 6);

    // get filetimes
    FILETIME_TO_TIMESPEC(buffer->st_atim, info.ftLastAccessTime);
    FILETIME_TO_TIMESPEC(buffer->st_mtim, info.ftLastWriteTime);

    // set a reasonable default last changed time
    auto atim = std::tie(buffer->st_atim.tv_sec, buffer->st_atim.tv_nsec);
    auto mtim = std::tie(buffer->st_mtim.tv_sec, buffer->st_mtim.tv_nsec);
    if (atim > mtim) {
        buffer->st_ctim = buffer->st_atim;
    } else {
        buffer->st_ctim = buffer->st_mtim;
    }

    ul.HighPart = info.nFileIndexHigh;
    ul.LowPart = info.nFileIndexLow;
    buffer->st_ino = ul.QuadPart;
    buffer->st_nlink = info.nNumberOfLinks;

    // Windows doesn't define these values
    buffer->st_gid = 0;
    buffer->st_uid = 0;
    buffer->st_rdev = 0;

    return 0;
}


template <typename Pointer, typename Function>
static int stat_impl(Pointer path, stat_t* buffer, bool use_lstat, Function function)
{
    HANDLE handle;
    DWORD access = FILE_READ_ATTRIBUTES;
    DWORD share = 0;
    LPSECURITY_ATTRIBUTES security = nullptr;
    DWORD create = OPEN_EXISTING;
    DWORD flags = FILE_FLAG_BACKUP_SEMANTICS;
    HANDLE file = nullptr;
    if (use_lstat) {
        flags |= FILE_FLAG_OPEN_REPARSE_POINT;
    }

    handle = function(path, access, share, security, create, flags, file);
    if (handle == INVALID_HANDLE_VALUE) {
        if (use_lstat && GetLastError() == ERROR_SYMLINK_NOT_SUPPORTED) {
            return stat_impl(path, buffer, false, function);
        }
        return get_error_code();
    }
    int status = copy_stat(handle, buffer);
    CloseHandle(handle);
    if (status) {
        return get_error_code();
    }

    return 0;
}


static int stat(const char* path, stat_t* buffer, bool use_lstat)
{
    return stat_impl(path, buffer, use_lstat, CreateFile);
}


static int wstat(const wchar_t* path, stat_t* buffer, bool use_lstat)
{
    return stat_impl(path, buffer, use_lstat, CreateFileW);
}


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

    code = stat(path.data(), &data, true);
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
static bool check_impl(const Path& path, Function function, bool use_lstat = false)
{
    try {
        if (use_lstat) {
            return function(lstat(path));
        }
        return function(stat(path));
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
    return check_impl(path, islink_stat, true);
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
