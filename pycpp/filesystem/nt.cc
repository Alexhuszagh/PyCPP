//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Windows-specific path normalization routines.
 */

#include <pycpp/config.h>
#include <pycpp/os.h>

PYCPP_BEGIN_NAMESPACE

#if defined(OS_WINDOWS)
#include <pycpp/casemap.h>
#include <pycpp/filesystem.h>
#include <pycpp/filesystem/exception.h>
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

// HELPERS
// -------


template <typename Path, typename ToPath>
static Path home_impl(ToPath topath)
{
    // check USERPROFILE
    wchar_t *home = _wgetenv(L"USERPROFILE");
    if (home != nullptr) {
        return topath(home);
    }

    // check HOME
    home = _wgetenv(L"HOME");
    if (home != nullptr) {
        return topath(home);
    }

    // combine HOMEDRIVE and HOMEPATH
    wchar_t *drive = _wgetenv(L"HOMEDRIVE");
    wchar_t *path = _wgetenv(L"HOMEPATH");
    if (drive != nullptr && path != nullptr) {
        // "c:", "\users\{user}"
        return topath(drive) + topath(path);
    }

    // try SystemDrive
    home = _wgetenv(L"SystemDrive");
    if (home != nullptr) {
        return topath(home);
    } else {
        return topath(L"c:");
    }
}


/**
 *  \brief Get path to temporary directory.
 */
template <typename Path, typename ToPath>
static Path tmpdir_impl(ToPath topath)
{
    wchar_t *dir = _wgetenv(L"TMPDIR");
    if (dir != nullptr) {
        return topath(dir);
    }

    dir = _wgetenv(L"TEMP");
    if (dir != nullptr) {
        return topath(dir);
    }

    dir = _wgetenv(L"TMP");
    if (dir != nullptr) {
        return topath(dir);
    } else {
        // temp directory not defined, return root
        return topath("/");
    }
}


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



template <typename Path, typename CopyFile>
static bool copy_file_impl(const Path& src, const Path& dst, bool replace, bool copystat, CopyFile copy)
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

    bool status = copy(src, dst, replace);
    if (status && copystat) {
        return PYCPP_NAMESPACE::copystat(src, dst);
    }
    return status;
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


template <typename Path>
static bool copy_dir_recursive_impl(const path_t&src, const path_t& dst)
{
    // TODO: implement...
    // TODO: need a directory iterator...
    return false;
}


template <typename Path>
static bool copy_dir_impl(const path_t&src, const path_t& dst, bool recursive)
{
    if (recursive) {
        return copy_dir_recursive_impl(src, dst);
    } else {
        return copy_dir_shallow_impl(src, dst);
    }
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
    return expanduser_impl(path, [](const std::wstring& str) {
        return path_t(reinterpret_cast<const char16_t*>(str.data()));
    });
}


path_t expandvars(const path_t& path)
{
    auto frompath = [](const path_t& path) {
        return path;
    };
    auto topath = [](wchar_t* str, size_t l) {
        return std::u16string(reinterpret_cast<char16_t*>(str), l);
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


bool move_file(const path_t& src, const path_t& dst, bool replace)
{
    return move_file_impl(src, dst, replace, [](const path_t& src, const path_t& dst) {
        auto s = reinterpret_cast<const wchar_t*>(src.data());
        auto d = reinterpret_cast<const wchar_t*>(dst.data());
        return MoveFileW(s, d);
    });
}


bool copy_file(const path_t& src, const path_t& dst, bool replace, bool copystat)
{
    return copy_file_impl(src, dst, replace, copystat, [](const path_t& src, const path_t& dst, bool replace) {
        auto s = reinterpret_cast<const wchar_t*>(src.data());
        auto d = reinterpret_cast<const wchar_t*>(dst.data());
        return CopyFileW(s, d, replace);
    });
}


bool remove_file(const path_t& path)
{
    return DeleteFileW(reinterpret_cast<const wchar_t*>(path.data()));
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
    return expanduser_impl(path, [](const std::wstring& str) {
        return path_to_backup_path(reinterpret_cast<const char16_t*>(str.data()));
    });
}


backup_path_t expandvars(const backup_path_t& path)
{
    auto frompath = [](const backup_path_t& path) {
        return backup_path_to_path(path);
    };
    auto topath = [](wchar_t* str, size_t l) {
        return path_to_backup_path(std::u16string(reinterpret_cast<char16_t*>(str), l));
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


bool move_file(const backup_path_t& src, const backup_path_t& dst, bool replace)
{
    return move_file_impl(src, dst, replace, [](const backup_path_t& src, const backup_path_t& dst) {
        return MoveFile(src.data(), dst.data());
    });
}


bool copy_file(const backup_path_t& src, const backup_path_t& dst, bool replace, bool copystat)
{
    return copy_file_impl(src, dst, replace, copystat, [](const backup_path_t& src, const backup_path_t& dst, bool replace) {
        return CopyFile(src.data(), dst.data(), replace);
    });
}


bool remove_file(const backup_path_t& path)
{
    return DeleteFile(path.data());
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


//// TODO: remove directory is going to be ugly
//
//int DeleteDirectory(const std::string &refcstrRootDirectory,
//                    bool              bDeleteSubdirectories = true)
//{
//  bool            bSubdirectory = false;       // Flag, indicating whether
//                                               // subdirectories have been found
//  HANDLE          hFile;                       // Handle to directory
//  std::string     strFilePath;                 // Filepath
//  std::string     strPattern;                  // Pattern
//  WIN32_FIND_DATA FileInformation;             // File information
//
//
//  strPattern = refcstrRootDirectory + "\\*.*";
//  hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
//  if(hFile != INVALID_HANDLE_VALUE)
//  {
//    do
//    {
//      if(FileInformation.cFileName[0] != '.')
//      {
//        strFilePath.erase();
//        strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;
//
//        if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//        {
//          if(bDeleteSubdirectories)
//          {
//            // Delete subdirectory
//            int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
//            if(iRC)
//              return iRC;
//          }
//          else
//            bSubdirectory = true;
//        }
//        else
//        {
//          // Set file attributes
//          if(::SetFileAttributes(strFilePath.c_str(),
//                                 FILE_ATTRIBUTE_NORMAL) == FALSE)
//            return ::GetLastError();
//
//          // Delete file
//          if(::DeleteFile(strFilePath.c_str()) == FALSE)
//            return ::GetLastError();
//        }
//      }
//    } while(::FindNextFile(hFile, &FileInformation) == TRUE);
//
//    // Close handle
//    ::FindClose(hFile);
//
//    DWORD dwError = ::GetLastError();
//    if(dwError != ERROR_NO_MORE_FILES)
//      return dwError;
//    else
//    {
//      if(!bSubdirectory)
//      {
//        // Set directory attributes
//        if(::SetFileAttributes(refcstrRootDirectory.c_str(),
//                               FILE_ATTRIBUTE_NORMAL) == FALSE)
//          return ::GetLastError();
//
//        // Delete directory
//        if(::RemoveDirectory(refcstrRootDirectory.c_str()) == FALSE)
//          return ::GetLastError();
//      }
//    }
//  }
//
//  return 0;
//}

#endif

PYCPP_END_NAMESPACE
