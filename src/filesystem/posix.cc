//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief POSIX-specific path normalization routines.
 */

#include "os.h"

#if defined(OS_POSIX)                           // POSIX & MACOS
#include "filesystem.h"
#include "filesystem/exception.h"

#include <unistd.h>
#include <algorithm>

// HELPERS
// -------


/**
 *  \brief Get path to home directory.
 */
template <typename Path, typename ToPath>
static Path home_impl(ToPath topath)
{
    char *dir = std::getenv("HOME");
    if (dir != nullptr) {
        return topath(dir);
    } else {
        // home not defined, root user
        return topath("/");
    }
}


/**
 *  \brief Get path to temporary directory.
 */
template <typename Path, typename ToPath>
static Path tmpdir_impl(ToPath topath)
{
    char *dir = std::getenv("TMPDIR");
    if (dir != nullptr) {
        return topath(dir);
    }

    dir = std::getenv("TEMP");
    if (dir != nullptr) {
        return topath(dir);
    }

    dir = std::getenv("TMP");
    if (dir != nullptr) {
        return topath(dir);
    } else {
        // temp directory not defined, return root
        return topath("/");
    }
}


/**
 *  \brief Get iterator where last directory separator occurs.
 */
template <typename Path>
static typename Path::const_iterator stem_impl(const Path& path)
{
    typedef typename Path::value_type char_type;

    return std::find_if(path.rbegin(), path.rend(), [](char_type c) {
        return path_separators.find(c) != path_separators.npos;
    }).base();
}

// SPLIT

template <typename Path>
static std::deque<Path> split_impl(const Path& path)
{
    auto it = stem_impl(path);
    Path basename(it, path.cend());
    Path dir(path.cbegin(), it);
    if (dir.size() > 1 && path_separators.find(dir.back()) != path_separators.npos) {
        dir.erase(dir.length() - 1);
    }

    return {std::move(dir), std::move(basename)};
}


template <typename Path>
std::deque<Path> splitdrive_impl(const Path& path)
{
    return {Path(), path};
}


template <typename Path>
std::deque<Path> splitunc_impl(const Path& path)
{
    return {Path(), path};
}

// NORMALIZATION


template <typename Path>
static Path basename_impl(const Path& path)
{
    return Path(stem_impl(path), path.cend());
}


template <typename Path>
static Path dirname_impl(const Path& path)
{
    auto it = stem_impl(path);
    Path dir(path.cbegin(), it);
    if (dir.size() > 1 && path_separators.find(dir.back()) != path_separators.npos) {
        dir.erase(dir.length() - 1);
    }

    return dir;
}


template <typename Path, typename ToPath>
static Path expanduser_impl(const Path& path, ToPath topath)
{
    switch (path.size()) {
        case 0:
            return path;
        case 1:
            return path[0] == '~' ? home_impl<Path>(topath) : path;
        default: {
            if (path[0] == '~' && path_separators.find(path[1]) != path_separators.npos) {
                return home_impl<Path>(topath) + path.substr(1);
            }
            return path;
        }
    }
}


template <typename Path>
bool isabs_impl(const Path& path)
{
    if (path.empty()) {
        return false;
    } else if (path_separators.find(path.front()) != path_separators.npos) {
        return true;
    }

    return false;
}


// FUNCTIONS
// ---------

// RUNTIME

path_t getcwd()
{
    char* buf = new char[PATH_MAX];
    if (!::getcwd(buf, MAX_PATH)) {
        throw filesystem_error(filesystem_unexpected_error);
    }

    path_t output(buf);
    delete[] buf;
    return output;
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


path_t basename(const path_t& path)
{
    return basename_impl(path);
}


path_t dirname(const path_t& path)
{
    return dirname_impl(path);
}


path_t expanduser(const path_t& path)
{
    return expanduser_impl(path, [](const path_t& p) {
        return p;
    });
}


path_t normcase(const path_t& path)
{
    return path;
}

#endif


#if defined(OS_POSIX) && !defined(OS_MACOS)     // POSIX without MACOS

// TODO: here...
#endif
