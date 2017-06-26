//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "filesystem.h"

#include <errno.h>
#include <sys/stat.h>
#include <stdexcept>


// HELPERS
// -------


template <typename Path, typename ToString>
static void handle_error(const Path& path, int code, ToString tostring)
{
    switch (code) {
        case 0:
            return;
        case ENOENT:
            throw std::runtime_error("File not found: " + tostring(path));
         case EINVAL:
            throw std::runtime_error("Invalid parameter to _stat: " + tostring(path));
        case ENOMEM:
            throw std::runtime_error("Out of memory: " + tostring(path));
         default:
            throw std::runtime_error("Unexpected error in stat: " + tostring(path));
    }
}


template <typename Path>
static void handle_error(const Path& path, int code)
{
    return handle_error(path, code, [](const Path& path) {
        return path;
    });
}


#if defined(OS_WINDOWS)         // WINDOWS


static void copy_native(const struct _stat& src, stat_t& dst)
{
    dst.st_dev = src.st_dev;
    dst.st_ino = src.st_ino;
    dst.st_mode = src.st_mode;
    dst.st_nlink = src.st_nlink;
    dst.st_uid = src.st_uid;
    dst.st_gid = src.st_gid;
    dst.st_rdev = src.st_rdev;
    dst.st_size = src.st_size;
    dst.st_atim = {src.st_atime, 0};
    dst.st_mtim = {src.st_mtime, 0};
    dst.st_ctim = {src.st_ctime, 0};
}


stat_t stat(const path_t& path)
{
    struct _stat sb;
    stat_t data;
    int code;

    auto buffer = reinterpret_cast<const wchar_t*>(path.data());
    code = ::_wstat(buffer, &sb);
    handle_error(path, code, codec_utf16_utf8);
    copy_native(sb, data);

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
    handle_error(path, code);
    copy_native(sb, data);

    return data;
}

#endif

#if defined(backup_path_t)          // BACKUP PATH

stat_t stat(const backup_path_t& path)
{
    struct _stat sb;
    stat_t data;
    int code;

    code = ::_stat(path.data(), &sb);
    handle_error(path, code);
    copy_native(sb, data);

    return data;
}

#endif

// FUNCTIONS
// ---------


time_t getatime(const path_t& path)
{
    return stat(path).st_atim.tv_sec;
}


time_t getmtime(const path_t& path)
{
    return stat(path).st_mtim.tv_sec;
}


time_t getctime(const path_t& path)
{
    return stat(path).st_ctim.tv_sec;
}


off_t getsize(const path_t& path)
{
    return stat(path).st_size;
}


#if defined(backup_path_t)          // BACKUP PATH


time_t getatime(const backup_path_t& path)
{
    return stat(path).st_atim.tv_sec;
}


time_t getmtime(const backup_path_t& path)
{
    return stat(path).st_mtim.tv_sec;
}


time_t getctime(const backup_path_t& path)
{
    return stat(path).st_ctim.tv_sec;
}


off_t getsize(const backup_path_t& path)
{
    return stat(path).st_size;
}

#endif
