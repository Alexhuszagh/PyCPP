//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Path normalization routines.
 */

#include "filesystem.h"

#include <algorithm>

// HELPERS
// -------


// SPLIT

template <typename Path>
static std::deque<Path> splitext_impl(const Path& path)
{
    typedef typename Path::value_type char_type;

    auto list = split(path);
    auto it = std::find_if(list.back().cbegin(), list.back().cend(), [](char_type c) {
        return c == path_extension;
    });

    Path root(list.back().cbegin(), it);
    Path ext(it, list.back().cend());
    if (root.size() > 1 && root.back() == path_extension) {
        root.erase(root.length() - 1);
    }

    return {list.front() + root, std::move(ext)};
}


// NORMALIZATION


template <typename Path, typename ToPath>
static Path abspath_impl(const Path& path, ToPath topath)
{
    typedef typename Path::value_type char_type;
    static constexpr char_type separator(path_separator);

    if (isabs(path)) {
        return path;
    }

    auto list = splitdrive(path);
    return list.front() + topath(getcwd()) + separator + normpath(path);
}


template <typename Path>
static Path realpath_impl(const Path& path)
{
    if (islink(path)) {
        auto link = readlink(path);
        if (isabs(link)) {
            return link;
        } else {
            return abspath(link);
        }
    }

    return path;
}


template <typename Path>
static Path normpath_impl(const Path& path)
{
    // TODO: implement
    return path;
}


template <typename Path>
Path relpath_impl(const Path& path, const Path& start)
{
    typedef typename Path::value_type char_type;
    typedef typename Path::iterator iterator;

    auto length = std::min(path.size(), start.size());
    auto f1 = path.begin();
    auto l1 = f1 + length;
    auto f2 = start.begin();
    auto it = std::mismatch(f1, l1, f2).first;

    return Path(it, path.end());
}


template <typename Path, typename ToPath>
Path relpath_impl(const Path& path, ToPath topath)
{
    return relpath_impl(path, topath(getcwd()));
}


// FUNCTIONS
// ---------

// SPLIT

path_list_t splitext(const path_t& path)
{
    return splitext_impl(path);
}


// NORMALIZATION


path_t abspath(const path_t& path)
{
    return abspath_impl(path, [](const path_t& p) {
        return p;
    });
}


path_t realpath(const path_t& path)
{
    return realpath_impl(path);
}


path_t normpath(const path_t& path)
{
    return normpath_impl(path);
}


path_t relpath(const path_t& path)
{
    return relpath_impl(path, [](const path_t& p) {
        return p;
    });
}


path_t relpath(const path_t& path, const path_t& start)
{
    return relpath_impl(path, start);
}

#if defined(backup_path_t)          // BACKUP PATH

// SPLIT

backup_path_list_t splitext(const backup_path_t& path)
{
    return splitext_impl(path);
}

// NORMALIZATION

backup_path_t abspath(const backup_path_t& path)
{
    return abspath_impl(path, [](const path_t& p) {
        return path_to_backup_path(p);
    });
}


backup_path_t realpath(const backup_path_t& path)
{
    return realpath_impl(path);
}


backup_path_t normpath(const backup_path_t& path)
{
    return normpath_impl(path);
}


backup_path_t relpath(const backup_path_t& path)
{
    return relpath_impl(path, [](const path_t& p) {
        return path_to_backup_path(p);
    });
}


backup_path_t relpath(const backup_path_t& path, const backup_path_t& start)
{
    return relpath_impl(path, start);
}

#endif
