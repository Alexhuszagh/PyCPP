//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

template <typename Path>
bool copy_link_impl(const Path& src, const Path& dst, bool replace)
{
    auto target = read_link(src);
    return mklink(target, dst, replace);
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

bool copy_link(const path_t& src, const path_t& dst, bool replace)
{
    return copy_link_impl(src, dst, replace);
}


path_list_t listdir(const path_t& path)
{
    return listdir_impl(path, [](path_t&& path) {
        return std::forward<path_t>(path);
    });
}

#if defined(OS_WINDOWS)          // BACKUP PATH


bool copy_link(const backup_path_t& src, const backup_path_t& dst, bool replace)
{
    return copy_link_impl(src, dst, replace);
}


backup_path_list_t listdir(const backup_path_t& path)
{
    return listdir_impl(path, [](path_t&& path) {
        return path_to_backup_path(path);
    });
}

#endif

PYCPP_END_NAMESPACE
