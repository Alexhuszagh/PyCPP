//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


template <typename Path>
bool move_path_impl(const Path& src, const Path& dst, bool replace)
{
    auto src_lstat = lstat(src);
    if (islink(src_lstat)) {
        return move_link(src, dst, replace);
    } else if (isfile(src_lstat)) {
        return move_file(src, dst, replace);
    } else if (isdir(src_lstat)) {
        return move_dir(src, dst, replace);
    }
}


template <typename Path>
bool copy_link_impl(const Path& src, const Path& dst, bool replace)
{
    auto target = read_link(src);
    return mklink(target, dst, replace);
}


template <typename Path>
bool copy_path_impl(const Path& src, const Path& dst, bool recursive, bool replace)
{
    auto src_lstat = lstat(src);
    if (islink(src_lstat)) {
        return copy_link(src, dst, replace);
    } else if (isfile(src_lstat)) {
        return copy_file(src, dst, replace);
    } else if (isdir(src_lstat)) {
        return copy_dir(src, dst, recursive, replace);
    }
}


template <typename Path>
bool remove_path_impl(const Path& path, bool recursive)
{
    auto path_lstat = lstat(path);
    if (islink(path_lstat)) {
        return remove_link(path);
    } else if (isfile(path_lstat)) {
        return remove_file(path);
    } else if (isdir(path_lstat)) {
        return remove_dir(path, recursive);
    }
}


template <typename Path, typename ToPath>
std::deque<Path> listdir_impl(const Path& path, ToPath topath)
{
    std::deque<Path> list;
    directory_iterator first(path);
    directory_iterator last;
    for (; first != last; ++first) {
        list.emplace_back(topath(first->basename()));
    }

    return list;
}


// FUNCTIONS
// ---------

bool move_path(const path_t& src, const path_t& dst, bool replace)
{
    return move_path_impl(src, dst, replace);
}


bool copy_link(const path_t& src, const path_t& dst, bool replace)
{
    return copy_link_impl(src, dst, replace);
}


bool copy_path(const path_t& src, const path_t& dst, bool recursive, bool replace)
{
    return copy_path_impl(src, dst, recursive, replace);
}


bool remove_path(const path_t& path, bool recursive)
{
    return remove_path_impl(path, recursive);
}


path_list_t listdir(const path_t& path)
{
    return listdir_impl(path, [](path_t&& path) {
        return std::forward<path_t>(path);
    });
}

#if defined(OS_WINDOWS)          // BACKUP PATH


bool move_path(const backup_path_t& src, const backup_path_t& dst, bool replace)
{
    return move_path_impl(src, dst, replace);
}


bool copy_link(const backup_path_t& src, const backup_path_t& dst, bool replace)
{
    return copy_link_impl(src, dst, replace);
}


bool copy_path(const backup_path_t& src, const backup_path_t& dst, bool recursive, bool replace)
{
    return copy_path_impl(src, dst, recursive, replace);
}


bool remove_path(const backup_path_t& path, bool recursive)
{
    return remove_path_impl(path, recursive);
}


backup_path_list_t listdir(const backup_path_t& path)
{
    return listdir_impl(path, [](path_t&& path) {
        return path_to_backup_path(path);
    });
}

#endif

PYCPP_END_NAMESPACE
