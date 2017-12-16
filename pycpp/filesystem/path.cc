//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Path normalization routines.
 */

#include <pycpp/filesystem.h>
#include <pycpp/string/string.h>
#include <pycpp/stl/algorithm.h>
#if defined(OS_WINDOWS)
#   include <pycpp/windows/winapi.h>
#endif

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

// ¯\_(ツ)_/¯
#undef max
#undef min

// HELPERS
// -------


// SPLIT

template <typename Path>
static deque<Path> splitext_impl(const Path& path)
{
    using char_type = typename Path::value_type;

    auto list = path_split(path);
    auto &tail = list.back();
    size_t i = tail.rfind(path_extension);
    if (i == 0 || i == tail.npos) {
        return {path, Path()};
    }

    auto index = path.size() - tail.size() + i;
    return {path.substr(0, index), path.substr(index)};
}


// NORMALIZATION


template <typename Path>
struct abspath_impl
{
    template <typename View, typename ToPath>
    Path operator()(const View& path, ToPath topath)
    {
        using char_type = typename Path::value_type;
        static constexpr char_type separator(path_separator);

        if (isabs(path)) {
            return Path(path);
        }

        auto list = path_splitdrive(path);
        return Path(list.front()) + topath(getcwd()) + separator + normpath(path);
    }
};


template <typename Path>
struct realpath_impl
{
    template <typename View>
    Path operator()(const View& path)
    {
        if (islink(path)) {
            auto link = read_link(path);
            if (isabs(link)) {
                return link;
            } else {
                return abspath(link);
            }
        }

        return Path(path);
    }
};


template <typename Path>
struct normpath_impl
{
    template <typename View, typename ToPath, typename FromPath>
    Path operator()(const View& path, ToPath topath, FromPath frompath)
    {
        // get drive/root components
        Path root;
        auto list = path_splitdrive(path);
        auto &drive = list.front();
        auto &tail = list.back();
        if (!tail.empty() && path_separators.find(tail.front()) != path_separators.npos) {
            root += path_separator;
            tail = tail.substr(1);
        }

        // get directory components
        auto tail_string = frompath(tail);
        auto dirs = split(tail_string, path_to_string(path_separators));
        vector<std::string> buffer;
        for (auto it = dirs.begin(); it != dirs.end(); ++it) {
            if (*it == current_directory) {
                if (root.empty() && buffer.empty() && distance(it, dirs.end()) == 1) {
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
        Path output(drive);
        output += root;
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
};


template <typename Path>
struct relpath_impl
{
    template <typename View>
    Path operator()(const View& path, const View& start)
    {
        using char_type = typename Path::value_type;
        using iterator = typename Path::iterator;

        auto length = min(path.size(), start.size());
        auto f1 = path.begin();
        auto l1 = f1 + length;
        auto f2 = start.begin();
        auto it = mismatch(f1, l1, f2).first;

        if (it == path.end()) {
            return Path();
        } else if (path_separators.find(*it) != path_separators.npos) {
            ++it;
        }

        return Path(it, path.end());
    }

    template <typename View, typename ToPath>
    Path operator()(const View& path, ToPath topath)
    {
        return operator()(path, View(topath(getcwd())));
    }
};


// FUNCTIONS
// ---------

#if defined(OS_WINDOWS)          // WINDOWS

// CONVERSIONS


std::u16string ansi_to_utf16(const string_view& ansi)
{
    // parameters
    auto srclen = ansi.size();
    auto src = ansi.data();
    auto dstlen = srclen * 2;       // need 4 bytes per ANSI character
    wchar_t* dst = new wchar_t[dstlen];

    // conversion
    auto length = MultiByteToWideChar(CP_ACP, 0, src, srclen, dst, dstlen);
    if (length == 0) {
        delete[] dst;
        throw runtime_error("Cannot convert ansi to UTF-16.");
    }

    // create output
    std::u16string u16(reinterpret_cast<char16_t*>(dst), length);
    delete[] dst;

    return u16;
}


std::string utf16_to_ansi(const u16string_view& u16)
{
    // parameters
    auto srclen = u16.size();
    auto src = reinterpret_cast<const wchar_t*>(u16.data());
    auto dstlen = srclen * 3;       // need 1.5 bytes per UTF-16 character
    char* dst = new char[dstlen];

    // conversion
    auto length = WideCharToMultiByte(CP_ACP, 0, src, srclen, dst, dstlen, nullptr, nullptr);
    if (length == 0) {
        delete[] dst;
        throw runtime_error("Cannot convert UTF-16 to ANSI.");
    }

    // create output
    std::string ansi(dst, length);
    delete[] dst;

    return ansi;
}


#endif


// SPLIT

path_view_list_t path_splitext(const path_view_t& path)
{
    return splitext_impl(path);
}


// NORMALIZATION


path_t abspath(const path_view_t& path)
{
    return abspath_impl<path_t>()(path, [](const path_t& p) {
        return p;
    });
}


path_t realpath(const path_view_t& path)
{
    return realpath_impl<path_t>()(path);
}


path_t normpath(const path_view_t& path)
{
    auto topath = [](const string_view& str) -> path_t
    {
        return path_t(string_to_path(str));
    };
    auto frompath = [](const path_view_t& p) -> std::string
    {
        return std::string(path_to_string(p));
    };

    return normpath_impl<path_t>()(path, topath, frompath);
}


path_t relpath(const path_view_t& path)
{
    return relpath_impl<path_t>()(path, [](const path_view_t& p) {
        return p;
    });
}


path_t relpath(const path_view_t& path, const path_view_t& start)
{
    return relpath_impl<path_t>()(path, start);
}

#if defined(OS_WINDOWS)          // BACKUP PATH

// SPLIT

backup_path_view_list_t path_splitext(const backup_path_view_t& path)
{
    return splitext_impl(path);
}

// NORMALIZATION

backup_path_t abspath(const backup_path_view_t& path)
{
    return abspath_impl<backup_path_t>()(path, [](const path_t& p) {
        return path_to_backup_path(p);
    });
}


backup_path_t realpath(const backup_path_view_t& path)
{
    return realpath_impl<backup_path_t>()(path);
}


backup_path_t normpath(const backup_path_view_t& path)
{
    auto topath = [](const string_view& str) -> backup_path_t
    {
        return backup_path_t(string_to_backup_path(str));
    };
    auto frompath = [](const backup_path_view_t& p) -> std::string
    {
        return std::string(backup_path_to_string(p));
    };

    return normpath_impl<backup_path_t>()(path, topath, frompath);
}


backup_path_t relpath(const backup_path_view_t& path)
{
    return relpath_impl<backup_path_t>()(path, [](const path_view_t& p) {
        return path_to_backup_path(p);
    });
}


backup_path_t relpath(const backup_path_view_t& path, const backup_path_view_t& start)
{
    return relpath_impl<backup_path_t>()(path, start);
}

#endif

PYCPP_END_NAMESPACE
