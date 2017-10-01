//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Windows-specific path normalization routines.
 */

#include <pycpp/config.h>
#include <pycpp/preprocessor/os.h>

#if defined(OS_WINDOWS)

#if _WIN32_WINNT < _WIN32_WINNT_VISTA
#   undef _WIN32_WINNT
#   define _WIN32_WINNT _WIN32_WINNT_VISTA
#endif

#include <pycpp/filesystem.h>
#include <pycpp/filesystem/exception.h>
#include <pycpp/string/casemap.h>
#include <errno.h>
#include <io.h>
#include <windows.h>
#include <sys/stat.h>
#include <algorithm>

// MACROS
// ------

#ifndef S_IRUSR
#   define S_IRUSR 00400
#endif

#ifndef S_IWUSR
#   define S_IWUSR 00200
#endif

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

// ERRNO

static void handle_open_error(int code)
{
    switch (code) {
        case 0:
            return;
        // TODO: implement...
        default:
            break;
    }
}


static void handle_read_error(int code)
{
    switch (code) {
        case 0:
            return;
        // TODO: implement...
        default:
            break;
    }
}


static void handle_write_error(int code)
{
    switch (code) {
        case 0:
            return;
        // TODO: implement...
        default:
            break;
    }
}


static void handle_seek_error(int code)
{
    switch (code) {
        case 0:
            return;
        // TODO: implement...
        default:
            break;
    }
}


static void handle_close_error(int code)
{
    // ignore the error: close errors are difficult
}

// PATH

/**
 *  \brief Get iterator where last directory separator occurs.
 *  \warning splitdrive **must** be called prior to this.
 */
template <typename Path>
static typename Path::const_iterator stem_impl(const Path& path)
{
    typedef typename Path::value_type char_type;

    return std::find_if(path.rbegin(), path.rend(), [](char_type c) {
        return path_separators.find(c) != path_separators.npos;
    }).base();
}


/**
 *  \brief Convert separators to preferred separators..
 */
template <typename Path>
static Path make_preferred(const Path& path)
{
    Path output;
    output.reserve(path.size());
    for (auto c: path) {
        if (path_separators.find(c) != path_separators.npos) {
            output.push_back(path_separator);
        } else {
            output.push_back(c);
        }
    }

    return output;
}

// RUNTIME

/** Has support for multiple drives and UNC paths. A windows path is
 *  comprised of 2 parts: a drive, and a path from the root.
 *  Any absolute paths from the drive will replace previous roots,
 *  and any new drives will replace the root and the path.
 */
template <typename List, typename ToPath>
static typename List::value_type join_impl(const List &paths, ToPath topath)
{
    typedef typename List::value_type Path;

    Path drive, path;
    for (auto &item: paths) {
        auto split = splitdrive(item);
        if (split[0].size()) {
            // new drive
            drive = split[0];
            path = split[1];
            if (path.size()) {
                // add only if non-empty, so join("D:", "temp") -> "D:temp"
                path += topath(path_separator);
            }
        } else if (split[1].size()) {
            // skip empty elements
            auto &root = split[1];
            if (path_separators.find(root[0]) != path_separators.npos) {
                // new root
                path = root;
            } else {
                path += root;
            }
            path += topath(path_separator);
        }
    }

    // clean up trailing separator
    if (path.size()) {
        path.erase(path.length() - 1);
    }

    return drive + path;
}

// SPLIT

template <typename Path>
static std::deque<Path> split_impl(const Path& path)
{
    auto list = splitdrive(path);
    Path &tail = list.back();
    auto it = stem_impl(tail);
    Path basename(it, tail.cend());
    Path dir(tail.cbegin(), it);
    if (dir.size() > 1 && path_separators.find(dir.back()) != path_separators.npos) {
        dir.erase(dir.length() - 1);
    }

    return {list.front() + dir, std::move(basename)};
}


/**
 *  See [here](1) for information on Windows paths and labels.
 *  1. https://msdn.microsoft.com/en-us/library/windows/desktop/aa365247(v=vs.85).aspx
 *
 *  \code
 *      splitunc("\\\\localhost")  => {"", "\\\\localhost"}
 *      splitunc("\\\\localhost\\x")  => {"\\\\localhost\\x", ""}
 */
template <typename Path>
static std::deque<Path> splitunc_impl(const Path& path)
{
    // sanity checks
    if (path.size() < 2) {
        return {Path(), path};
    }

    if (path[1] == ':') {
        // have a drive letter
        return {Path(), path};
    }
    auto p0 = path_separators.find(path[0]) != path_separators.npos;
    auto p1 = path_separators.find(path[1]) != path_separators.npos;
    if (p0 && p1) {
        // have a UNC path
        auto norm = normcase(path);
        auto index = norm.find(path_separator, 2);
        if (index == norm.npos) {
            return {Path(), path};
        }
        index = norm.find(path_separator, index + 1);
        if (index == norm.npos) {
            return {path, Path()};
        }
        return {path.substr(0, index), path.substr(index)};
    }

    return {Path(), path};
}


/**
 *  See [here](1) for information on Windows paths and labels.
 *  1. https://msdn.microsoft.com/en-us/library/windows/desktop/aa365247(v=vs.85).aspx
 *
 *  \code
 *      splitdrive("\\\\localhost")  => {"", "\\\\localhost"}
 *      splitdrive("\\\\localhost\\x")  => {"\\\\localhost\\x", ""}
 *      splitdrive("\\\\localhost\\x\\y")  => {"\\\\localhost\\x", "\\y"}
 *      "\\\\?\\D:\\very long path" => {"\\\\?\\D:", "\\very long path"}
 */
template <typename Path>
static std::deque<Path> splitdrive_impl(const Path& path)
{
    if (path.size() < 2) {
        return {Path(), path};
    } else if (path[1] == ':') {
        return {path.substr(0, 2), path.substr(2)};
    }

    return splitunc_impl(path);
}

// NORMALIZATION


template <typename Path>
static Path base_name_impl(const Path& path)
{
    auto tail = splitdrive(path).back();
    return Path(stem_impl(tail), tail.cend());
}


template <typename Path>
static Path dir_name_impl(const Path& path)
{
    auto tail = splitdrive(path).back();
    auto it = stem_impl(tail);
    Path dir(tail.cbegin(), it);
    if (dir.size() > 1 && path_separators.find(dir.back()) != path_separators.npos) {
        dir.erase(dir.length() - 1);
    }

    return dir;
}


template <typename Path, typename HomeFunc>
static Path expanduser_impl(const Path& path, HomeFunc homefunc)
{
    switch (path.size()) {
        case 0:
            return path;
        case 1:
            return path[0] == '~' ? homefunc() : path;
        default: {
            if (path[0] == '~' && path_separators.find(path[1]) != path_separators.npos) {
                return homefunc() + path.substr(1);
            }
            return path;
        }
    }
}


template <typename Path, typename FromPath, typename ToPath, typename Function>
static Path expandvars_impl(const Path& path, FromPath frompath, ToPath topath, Function function)
{
    wchar_t* buf = new wchar_t[MAX_PATH];
    auto wide = frompath(path);
    auto data = reinterpret_cast<const wchar_t*>(wide.data());
    DWORD length = function(data, buf, MAX_PATH);
    if (length == 0) {
        throw filesystem_error(filesystem_unexpected_error);
    }

    Path output(topath(buf, length-1));
    delete[] buf;
    return output;
}


template <typename Path>
static bool isabs_impl(const Path& path)
{
    auto tail = splitdrive_impl(path).back();
    if (tail.empty()) {
        return false;
    } else if (path_separators.find(tail.front()) != path_separators.npos) {
        return true;
    }

    return false;
}


template <typename Path, typename NormCase>
static Path normcase_impl(const Path& path, NormCase normcase)
{
    return normcase(make_preferred(path));
}


// MANIPULATION

template <typename Path, typename MoveFile>
static bool move_file_impl(const Path& src, const Path& dst, bool replace, MoveFile move)
{
    auto dst_dir = dir_name(dst);

    // ensure we have a file and a dest directory
    auto src_stat = stat(src);
    auto dst_stat = stat(dst_dir);
    if (!isfile(src)) {
        throw filesystem_error(filesystem_not_a_file);
    } else if (!exists(dst_stat)) {
        throw filesystem_error(filesystem_no_such_directory);
    }

    if (exists(dst)) {
        if (replace) {
            remove_file(dst);
        } else {
            throw filesystem_error(filesystem_destination_exists);
        }
    }

    // Windows MoveFileW can handle different filesystems
    // don't worry about st_dev.
    return move(src, dst);
}


template <typename Path>
static bool is_empty_directory_impl(const Path& path)
{
    directory_iterator it(path);
    return it == directory_iterator();
}


template <typename Path, typename MoveDir>
static bool move_dir_impl(const Path& src, const Path& dst, bool replace, MoveDir move)
{
    // get stats
    auto src_stat = stat(src);
    auto dst_stat = stat(dst);

    // check if we can move
    if (!isdir(src)) {
        throw filesystem_error(filesystem_no_such_directory);
    } else if (exists(dst_stat)) {
        // destination exists, can we overwrite?
        if (replace) {
            if (!remove_path(dst)) {
                return false;
            }
        } else if (!isdir(dst_stat) || !is_empty_directory_impl(dst)) {
            throw filesystem_error(filesystem_destination_exists);
        }
    }

    // no guaranteed atomicity
    if (src_stat.st_dev == dst_stat.st_dev) {
        // same volume, call MoveFile[AW]
        return move(src, dst);
    } else {
        // different filesystems, remove and copy
        if (!copy_dir(src, dst, true)) {
            throw filesystem_error(filesystem_unexpected_error);
        }
        if (!remove_dir(src)) {
            throw filesystem_error(filesystem_unexpected_error);
        }
    }
    return false;
}


template <typename Path, typename Mklink>
static bool mklink_impl(const Path& target, const Path& dst, bool replace, Mklink linker)
{
    if (replace && exists(dst)) {
        remove_file(dst);
    }

    DWORD flags = isdir(target);
    return linker(target, dst, flags);
}


template <typename Path, typename CopyFile>
static bool copy_file_impl(const Path& src, const Path& dst, bool replace, CopyFile copy)
{
    auto dst_dir = dir_name(dst);

    // ensure we have a file and a dest directory
    auto src_stat = stat(src);
    auto dst_stat = stat(dst_dir);
    if (!isfile(src)) {
        throw filesystem_error(filesystem_not_a_file);
    } else if (!exists(dst_stat)) {
        throw filesystem_error(filesystem_no_such_directory);
    }

    return copy(src, dst, replace);
}


static bool copy_dir_shallow_impl(const path_t&src, const path_t& dst)
{
    auto s = reinterpret_cast<const wchar_t*>(src.data());
    auto d = reinterpret_cast<const wchar_t*>(dst.data());
    return CreateDirectoryExW(s, d, 0);
}


static bool copy_dir_shallow_impl(const backup_path_t&src, const backup_path_t& dst)
{
    return CreateDirectoryExA(src.data(), dst.data(), 0);
}


static bool copy_dir_recursive_impl(const path_t& src, const path_t& dst)
{
    if (!copy_dir_shallow_impl(src, dst)) {
        return false;
    }

    directory_iterator first(src);
    directory_iterator last;
    for (; first != last; ++first) {
        path_list_t dst_list = {dst, first->basename()};
        if (first->isfile()) {
            if (!copy_file(first->path(), join(dst_list))) {
                return false;
            }
        } else if (first->islink()) {
            if (!copy_link(first->path(), join(dst_list))) {
                return false;
            }
        } else if (first->isdir()) {
            if (!copy_dir_recursive_impl(first->path(), join(dst_list))) {
                return false;
            }
        }
    }

    return true;
}


static bool copy_dir_recursive_impl(const backup_path_t& src, const backup_path_t& dst)
{
    return copy_dir_recursive_impl(backup_path_to_path(src), backup_path_to_path(dst));
}


template <typename Path>
static bool copy_dir_impl(const Path&src, const Path& dst, bool recursive, bool replace)
{
    if (replace && exists(dst)) {
        if (!remove_path(dst)) {
            throw filesystem_error(filesystem_destination_exists);
        }
    }

    if (recursive) {
        return copy_dir_recursive_impl(src, dst);
    } else {
        return copy_dir_shallow_impl(src, dst);
    }
}


template <typename Path>
static bool remove_link_impl(const Path& path)
{
    if (!islink(path)) {
        throw filesystem_error(filesystem_not_a_symlink);
    }

    auto path_stat = stat(path);
    if (isdir(path_stat)) {
        return remove_dir(path, false);
    } else {
        return remove_file(path);
    }
}


static bool remove_dir_shallow_impl(const path_t& path)
{
    auto p = reinterpret_cast<const wchar_t*>(path.data());
    return RemoveDirectoryW(p);
}


static bool remove_dir_shallow_impl(const backup_path_t& path)
{
    return RemoveDirectoryA(path.data());
}


template <typename Path>
static bool remove_dir_recursive_impl(const Path& path)
{
    directory_iterator first(path);
    directory_iterator last;
    for (; first != last; ++first) {
        if (first->isfile()) {
            if (!remove_file(first->path())) {
                return false;
            }
        } else if (first->islink()) {
            if (!remove_link(first->path())) {
                return false;
            }
        } else if (first->isdir()) {
            if (!remove_dir_recursive_impl(first->path())) {
                return false;
            }
        }
    }

    return true;
}


template <typename Path>
static bool remove_dir_impl(const Path& path, bool recursive)
{
    if (recursive) {
        return remove_dir_recursive_impl(path);
    } else {
        return remove_dir_shallow_impl(path);
    }
}

// FILE UTILS


static DWORD convert_access_mode(std::ios_base::openmode mode)
{
    if ((mode & std::ios_base::in) && (mode & std::ios_base::out)) {
        return GENERIC_READ | GENERIC_WRITE;
    } else if (mode & std::ios_base::in) {
        return GENERIC_READ;
    } else if (mode & std::ios_base::out) {
        return GENERIC_WRITE;
    }

    return 0;
}


static DWORD convert_create_mode(std::ios_base::openmode mode)
{
    if (mode & std::ios_base::trunc) {
        return CREATE_ALWAYS;
    } else if (mode & std::ios_base::out) {
        return OPEN_ALWAYS;
    } else {
        return OPEN_EXISTING;
    }
}


template <typename Pointer, typename Function>
static HANDLE fd_open_impl(const Pointer &path, std::ios_base::openmode mode, Function function)
{
    DWORD access = convert_access_mode(mode);
    DWORD share = 0;
    LPSECURITY_ATTRIBUTES security = nullptr;
    DWORD create = convert_create_mode(mode);
    DWORD flags = 0;
    HANDLE file = nullptr;

    return function(path, access, share, security, create, flags, file);
}


template <typename Path>
static int fd_allocate_impl(const Path& path, std::streamsize size)
{
    fd_t fd = fd_open(path, std::ios_base::out);
    if (fd == INVALID_HANDLE_VALUE) {
        return false;
    }
    int status = fd_allocate(fd, size);
    fd_close(fd);

    return status;
}


template <typename Path>
static int fd_truncate_impl(const Path& path, std::streamsize size)
{
    fd_t fd = fd_open(path, std::ios_base::out);
    if (fd == INVALID_HANDLE_VALUE) {
        return false;
    }
    int status = fd_truncate(fd, size);
    fd_close(fd);

    return status;
}


// FUNCTIONS
// ---------

// RUNTIME

path_t getcwd()
{
    wchar_t* buf = new wchar_t[MAX_PATH];
    DWORD length = GetCurrentDirectoryW(MAX_PATH, buf);
    if (length == 0) {
        throw filesystem_error(filesystem_unexpected_error);
    }

    path_t output(reinterpret_cast<char16_t*>(buf), length);
    delete[] buf;
    return output;
}


path_t join(const path_list_t &paths)
{
    return join_impl(paths, [](char16_t c) {
        return c;
    });
}

// SPLIT

path_list_t split(const path_t& path)
{
    return split_impl(path);
}


path_list_t splitdrive(const path_t& path)
{
    return splitdrive_impl(path);
}


path_list_t splitunc(const path_t& path)
{
    return splitunc_impl(path);
}

// NORMALIZATION


bool isabs(const path_t& path)
{
    return isabs_impl(path);
}


path_t base_name(const path_t& path)
{
    return base_name_impl(path);
}


path_t dir_name(const path_t& path)
{
    return dir_name_impl(path);
}


path_t expanduser(const path_t& path)
{
    return expanduser_impl(path, gettempdirw);
}


path_t expandvars(const path_t& path)
{
    auto frompath = [](const path_t& path) {
        return path;
    };
    auto topath = [](wchar_t* str, size_t l) {
        return path_t(reinterpret_cast<char16_t*>(str), l);
    };
    return expandvars_impl(path, frompath, topath, ExpandEnvironmentStringsW);
}


path_t normcase(const path_t& path)
{
    return normcase_impl(path, [](const path_t& p) {
        return utf16_tolower(p);
    });
}

// MANIPULATION


bool move_link(const path_t& src, const path_t& dst, bool replace)
{
    // same as move_file
    return move_file(src, dst, replace);
}


bool move_file(const path_t& src, const path_t& dst, bool replace)
{
    return move_file_impl(src, dst, replace, [](const path_t& src, const path_t& dst) {
        auto s = reinterpret_cast<const wchar_t*>(src.data());
        auto d = reinterpret_cast<const wchar_t*>(dst.data());
        return MoveFileW(s, d);
    });
}


bool move_dir(const path_t& src, const path_t& dst, bool replace)
{
    return move_dir_impl(src, dst, replace, [](const path_t& src, const path_t& dst) {
        auto s = reinterpret_cast<const wchar_t*>(src.data());
        auto d = reinterpret_cast<const wchar_t*>(dst.data());
        return MoveFileW(s, d);
    });
}


bool mklink(const path_t& target, const path_t& dst, bool replace)
{
    return mklink_impl(target, dst, replace, [](const path_t& tar, const path_t& dst, DWORD f) {
        auto t = reinterpret_cast<const wchar_t*>(tar.data());
        auto d = reinterpret_cast<const wchar_t*>(dst.data());
        return CreateSymbolicLinkW(d, t, f);
    });
}


bool copy_file(const path_t& src, const path_t& dst, bool replace)
{
    return copy_file_impl(src, dst, replace, [](const path_t& src, const path_t& dst, bool replace) {
        auto s = reinterpret_cast<const wchar_t*>(src.data());
        auto d = reinterpret_cast<const wchar_t*>(dst.data());
        return CopyFileW(s, d, replace);
    });
}


bool copy_dir(const path_t& src, const path_t& dst, bool recursive, bool replace)
{
    return copy_dir_impl(src, dst, recursive, replace);
}


bool remove_link(const path_t& path)
{
    return remove_link_impl(path);
}


bool remove_file(const path_t& path)
{
    return DeleteFileW(reinterpret_cast<const wchar_t*>(path.data()));
}


bool remove_dir(const path_t& path, bool recursive)
{
    return remove_dir_impl(path, recursive);
}


bool mkdir(const path_t& path, int mode)
{
    auto data = reinterpret_cast<const wchar_t*>(path.data());
    if (CreateDirectoryW(data, nullptr)) {
        int mask = 0;
        if (mode & S_IRUSR) {
            mask |= _S_IREAD;
        }
        if (mode & S_IWUSR) {
            mask |= _S_IWRITE;
        }
        return _wchmod(data, mask) == 0;
    }

    return false;
}


bool makedirs(const path_t& path, int mode)
{
    if (!exists(path)) {
        makedirs(dir_name(path), mode);
        return mkdir(path, mode);
    }

    return false;
}

// FILE UTILS


fd_t fd_open(const path_t& path, std::ios_base::openmode mode)
{
    const wchar_t* p = (const wchar_t*) path.data();
    fd_t fd = fd_open_impl(p, mode, CreateFileW);
    if (fd == INVALID_HANDLE_VALUE) {
        handle_open_error(GetLastError());
    }
    return fd;
}

std::streamsize fd_read(fd_t fd, void* buf, std::streamsize count)
{
    DWORD read;
    if (!ReadFile(fd, buf, count, &read, nullptr)) {
        handle_read_error(GetLastError());
        return -1;
    }

    return read;
}


std::streamsize fd_write(fd_t fd, void* buf, std::streamsize count)
{
    DWORD wrote;
    if (!WriteFile(fd, buf, count, &wrote, nullptr)) {
        handle_write_error(GetLastError());
        return -1;
    }

    return wrote;
}


std::streampos fd_seek(fd_t fd, std::streamoff off, std::ios_base::seekdir way)
{
    DWORD method;
    switch (way) {
        case std::ios_base::beg:
            method = FILE_BEGIN;
            break;
        case std::ios_base::cur:
            method = FILE_CURRENT;
            break;
        case std::ios_base::end:
            method = FILE_END;
            break;
        default:
            return std::streampos(std::streamoff(-1));
    }

    LARGE_INTEGER bytes;
    bytes.QuadPart = off;
    DWORD status = SetFilePointer(fd, bytes.LowPart, &bytes.HighPart, FILE_BEGIN);
    if (status == INVALID_SET_FILE_POINTER) {
        handle_seek_error(GetLastError());
        return -1;          // force POSIX-like behavior
    }
    return status;
}


int fd_close(fd_t fd)
{
    if (!CloseHandle(fd)) {
        handle_close_error(GetLastError());
        return -1;          // force POSIX-like behavior
    }
    return 0;
}


int fd_allocate(fd_t fd, std::streamsize size)
{
    if (fd == INVALID_HANDLE_VALUE) {
        return EBADF;
    }

    LARGE_INTEGER bytes;
    bytes.QuadPart = size;
    if (!::SetFilePointerEx(fd, bytes, nullptr, FILE_BEGIN)) {
        // TODO: return the proper error value, do not set errno
        return -1;
    }
    if (!::SetEndOfFile(fd)) {
        // TODO: return the proper error value, do not set errno
        return -1;
    }
    if (::SetFilePointer(fd, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
        // TODO: return the proper error value, do not set errno
        return -1;
    }

    return 0;
}


int fd_truncate(fd_t fd, std::streamsize size)
{
    return fd_allocate(fd, size);
}


int fd_allocate(const path_t& path, std::streamsize size)
{
    return fd_allocate_impl(path, size);
}


int fd_truncate(const path_t& path, std::streamsize size)
{
    return fd_truncate_impl(path, size);
}

#endif


#if defined(OS_WINDOWS)         // BACKUP PATH

// RUNTIME

backup_path_t join(const backup_path_list_t &paths)
{
    return join_impl(paths, [](char16_t c) {
        return static_cast<char>(c);
    });
}

// STAT

bool isabs(const backup_path_t& path)
{
    return isabs_impl(path);
}

// SPLIT

backup_path_list_t split(const backup_path_t& path)
{
    return split_impl(path);
}


backup_path_list_t splitdrive(const backup_path_t& path)
{
    return splitdrive_impl(path);
}


backup_path_list_t splitunc(const backup_path_t& path)
{
    return splitunc_impl(path);
}

// NORMALIZATION

backup_path_t base_name(const backup_path_t& path)
{
    return base_name_impl(path);
}


backup_path_t dir_name(const backup_path_t& path)
{
    return dir_name_impl(path);
}


backup_path_t expanduser(const backup_path_t& path)
{
    return expanduser_impl(path, gettempdira);
}


backup_path_t expandvars(const backup_path_t& path)
{
    auto frompath = [](const backup_path_t& path) {
        return backup_path_to_path(path);
    };
    auto topath = [](wchar_t* str, size_t l) {
        return path_to_backup_path(path_t(reinterpret_cast<char16_t*>(str), l));
    };
    return expandvars_impl(path, frompath, topath, ExpandEnvironmentStringsW);
}


backup_path_t normcase(const backup_path_t& path)
{
    return normcase_impl(path, [](const backup_path_t& p) {
        return utf8_tolower(p);
    });
}

// MANIPULATION

bool move_link(const backup_path_t& src, const backup_path_t& dst, bool replace)
{
    // same as move_file
    return move_file(src, dst, replace);
}


bool move_file(const backup_path_t& src, const backup_path_t& dst, bool replace)
{
    return move_file_impl(src, dst, replace, [](const backup_path_t& src, const backup_path_t& dst) {
        return MoveFileA(src.data(), dst.data());
    });
}


bool move_dir(const backup_path_t& src, const backup_path_t& dst, bool replace)
{
    return move_dir_impl(src, dst, replace, [](const backup_path_t& src, const backup_path_t& dst) {
        return MoveFileA(src.data(), dst.data());
    });
}


bool mklink(const backup_path_t& target, const backup_path_t& dst, bool replace)
{
    return mklink_impl(target, dst, replace, [](const backup_path_t& t, const backup_path_t& d, DWORD f) {
        return CreateSymbolicLinkA(d.data(), t.data(), f);
    });
}


bool copy_file(const backup_path_t& src, const backup_path_t& dst, bool replace)
{
    return copy_file_impl(src, dst, replace, [](const backup_path_t& src, const backup_path_t& dst, bool replace) {
        return CopyFileA(src.data(), dst.data(), replace);
    });
}


bool copy_dir(const backup_path_t& src, const backup_path_t& dst, bool recursive, bool replace)
{
    return copy_dir_impl(src, dst, recursive, replace);
}


bool remove_link(const backup_path_t& path)
{
    return remove_link_impl(path);
}


bool remove_file(const backup_path_t& path)
{
    return DeleteFileA(path.data());
}


bool remove_dir(const backup_path_t& path, bool recursive)
{
    return remove_dir_impl(path, recursive);
}


bool mkdir(const backup_path_t& path, int mode)
{
    if (CreateDirectory(path.data(), nullptr)) {
        int mask = 0;
        if (mode & S_IRUSR) {
            mask |= _S_IREAD;
        }
        if (mode & S_IWUSR) {
            mask |= _S_IWRITE;
        }
        return _chmod(path.data(), mask) == 0;
    }

    return false;
}


bool makedirs(const backup_path_t& path, int mode)
{
    if (!exists(path)) {
        makedirs(dir_name(path), mode);
        return mkdir(path, mode);
    }

    return false;
}

// FILE UTILS

fd_t fd_open(const backup_path_t& path, std::ios_base::openmode mode)
{
    return fd_open_impl(path.data(), mode, CreateFileA);
}


int fd_allocate(const backup_path_t& path, std::streamsize size)
{
    return fd_allocate_impl(path, size);
}


int fd_truncate(const backup_path_t& path, std::streamsize size)
{
    return fd_truncate_impl(path, size);
}

PYCPP_END_NAMESPACE

#endif
