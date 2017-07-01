//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Path normalization routines.
 */

#include <filesystem.h>
#include <stringlib.h>
#include <algorithm>

// HELPERS
// -------


// SPLIT

template <typename Path>
static std::deque<Path> splitext_impl(const Path& path)
{
    typedef typename Path::value_type char_type;

    auto list = split(path);
    auto &tail = list.back();
    size_t i = tail.rfind(path_extension);
    if (i == 0 || i == tail.npos) {
        return {path, Path()};
    }

    auto index = path.size() - tail.size() + i;
    return {path.substr(0, index), path.substr(index)};
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
        auto link = read_link(path);
        if (isabs(link)) {
            return link;
        } else {
            return abspath(link);
        }
    }

    return path;
}


template <typename Path, typename ToPath, typename FromPath>
static Path normpath_impl(const Path& path, ToPath topath, FromPath frompath)
{
    // get drive/root components
    Path root;
    auto list = splitdrive(path);
    auto &drive = list.front();
    auto &tail = list.back();
    if (!tail.empty() && path_separators.find(tail.front()) != path_separators.npos) {
        root += path_separator;
        tail = tail.substr(1);
    }

    // get directory components
    auto dirs = split(frompath(tail), path_to_string(path_separators));
    std::vector<std::string> buffer;
    for (auto it = dirs.begin(); it != dirs.end(); ++it) {
        if (*it == current_directory) {
            if (root.empty() && buffer.empty() && std::distance(it, dirs.end()) == 1) {
                buffer.push_back(*it);
            }
        } else if (*it == parent_directory) {
            // Erase if the buffer is not empty and the last element is not ..
            // otherwise, we have a relative path, and need to keep the item
            // If the path is "./..", we just want "..".
            // If the buffer is empty and it has a root path, we're already
            // at the root, so skip the item. Otherwise, we have a relative
            // path, so add the element.
            if (!buffer.empty()) {
                auto &parent = buffer.back();
                if (parent == current_directory) {
                    buffer.erase(buffer.end()-1);
                    buffer.push_back(*it);
                } else if (parent == parent_directory) {
                    buffer.emplace_back(*it);
                } else {
                    buffer.erase(buffer.end()-1);
                }
            } else if (root.empty()) {
                buffer.emplace_back(*it);
            }
        } else {
            buffer.emplace_back(*it);
        }
    }

    // create output
    Path output = drive + root;
    for (auto &item: buffer) {
        output += topath(item);
        output += path_separator;
    }
    if (!buffer.empty()) {
        output.erase(output.size() - 1);
    }
    if (output.empty()) {
        return topath(".");
    }

    return output;
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

    if (it == path.end()) {
        return Path();
    } else if (path_separators.find(*it) != path_separators.npos) {
        ++it;
    }

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
    auto topath = [](const std::string& str)
    {
        return string_to_path(str);
    };
    auto frompath = [](const path_t& p)
    {
        return path_to_string(p);
    };

    return normpath_impl(path, topath, frompath);
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
    auto topath = [](const std::string& str)
    {
        return string_to_backup_path(str);
    };
    auto frompath = [](const backup_path_t& p)
    {
        return backup_path_to_string(p);
    };

    return normpath_impl(path, topath, frompath);
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
