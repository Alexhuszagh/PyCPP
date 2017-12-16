//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/stl/utility.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


template <typename Path>
static bool move_path_impl(const Path& src, const Path& dst, bool replace)
{
    auto src_lstat = lstat(src);
    if (islink(src_lstat)) {
        return move_link(src, dst, replace);
    } else if (isfile(src_lstat)) {
        return move_file(src, dst, replace);
    } else if (isdir(src_lstat)) {
        return move_dir(src, dst, replace);
    }
    return false;
}


template <typename Path>
static bool copy_link_impl(const Path& src, const Path& dst, bool replace)
{
    auto target = read_link(src);
    return mklink(target, dst, replace);
}


template <typename Path>
static bool copy_path_impl(const Path& src, const Path& dst, bool recursive, bool replace)
{
    auto src_lstat = lstat(src);
    if (islink(src_lstat)) {
        return copy_link(src, dst, replace);
    } else if (isfile(src_lstat)) {
        return copy_file(src, dst, replace);
    } else if (isdir(src_lstat)) {
        return copy_dir(src, dst, recursive, replace);
    }
    return false;
}


template <typename Path>
static bool remove_path_impl(const Path& path, bool recursive)
{
    auto path_lstat = lstat(path);
    if (islink(path_lstat)) {
        return remove_link(path);
    } else if (isfile(path_lstat)) {
        return remove_file(path);
    } else if (isdir(path_lstat)) {
        return remove_dir(path, recursive);
    }
    return false;
}


template <typename Path>
static range<directory_iterator> iterdir_impl(const Path& path)
{
    return range<directory_iterator>(directory_iterator(path));
}



template <typename Path>
struct listdir_impl
{
    template <typename View, typename ToPath>
    deque<Path> operator()(const View& path, ToPath topath)
    {
        deque<Path> list;
        for (auto entry: iterdir_impl(path)) {
            list.emplace_back(topath(entry.basename()));
        }

        return list;
    }
};


// FUNCTIONS
// ---------


path_t join_path(initializer_list<path_view_t> paths)
{
    path_view_list_t list(paths.begin(), paths.end());
    return join_path(list);
}


bool move_path(const path_view_t& src, const path_view_t& dst, bool replace)
{
    return move_path_impl(src, dst, replace);
}


bool copy_link(const path_view_t& src, const path_view_t& dst, bool replace)
{
    return copy_link_impl(src, dst, replace);
}


bool copy_path(const path_view_t& src, const path_view_t& dst, bool recursive, bool replace)
{
    return copy_path_impl(src, dst, recursive, replace);
}


bool remove_path(const path_view_t& path, bool recursive)
{
    return remove_path_impl(path, recursive);
}


range<directory_iterator> iterdir(const path_view_t& path)
{
    return iterdir_impl(path);
}


path_list_t listdir(const path_view_t& path)
{
    return listdir_impl<path_t>()(path, [](path_t&& path) {
        return forward<path_t>(path);
    });
}


streampos fd_tell(fd_t fd)
{
    return fd_seek(fd, 0, ios_base::cur);
}

#if defined(OS_WINDOWS)          // BACKUP PATH


backup_path_t join_path(initializer_list<backup_path_view_t> paths)
{
    backup_path_view_list_t list(paths.begin(), paths.end());
    return join_path(list);
}


bool move_path(const backup_path_view_t& src, const backup_path_view_t& dst, bool replace)
{
    return move_path_impl(src, dst, replace);
}


bool copy_link(const backup_path_view_t& src, const backup_path_view_t& dst, bool replace)
{
    return copy_link_impl(src, dst, replace);
}


bool copy_path(const backup_path_view_t& src, const backup_path_view_t& dst, bool recursive, bool replace)
{
    return copy_path_impl(src, dst, recursive, replace);
}


bool remove_path(const backup_path_view_t& path, bool recursive)
{
    return remove_path_impl(path, recursive);
}


range<directory_iterator> iterdir(const backup_path_view_t& path)
{
    return iterdir_impl(path);
}


backup_path_list_t listdir(const backup_path_view_t& path)
{
    return listdir_impl<backup_path_t>()(path, [](path_t&& path) {
        return path_to_backup_path(path);
    });
}

#endif

PYCPP_END_NAMESPACE
