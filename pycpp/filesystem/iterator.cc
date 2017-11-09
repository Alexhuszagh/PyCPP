//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/filesystem/exception.h>
#include <pycpp/preprocessor/errno.h>
#include <string.h>
#include <algorithm>
#include <tuple>
#if defined(OS_WINDOWS)
#   include <pycpp/windows/winapi.h>
#else
#   include <dirent.h>
#endif

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

#ifndef NO_LARGEFILE_SOURCE
#   ifndef _LARGEFILE_SOURCE
#       define _LARGEFILE_SOURCE
#   endif
#   ifndef _FILE_OFFSET_BITS
#       define _FILE_OFFSET_BITS 64
#   endif
#endif

// TODO: these need to be moved as options later...
#define PYCPP_FOLLOW_DIRECTORY_SYMLINK false
#define PYCPP_SKIP_PERMISSIONS_ERROR false

// HELPERS
// -------


static void handle_error(int code)
{
    switch (code) {
        case 0:
            return;
        case EACCES:
            throw filesystem_error(filesystem_permissions_error);
        case EMFILE:
        case ENFILE:
            throw filesystem_error(filesystem_too_many_file_descriptors);
        case ENOENT:
        case ENOTDIR:
            throw filesystem_error(filesystem_no_such_directory);
        case ENOMEM:
            throw filesystem_error(filesystem_out_of_memory);
        default:
            throw filesystem_error(filesystem_unexpected_error);
    }
}


/**
 *  \brief Check if the file is "." or "..";
 */
static bool is_relative_dot(const char* name)
{
    return (strcmp(name, ".") == 0) || (strcmp(name, "..") == 0);
}


#if defined(OS_WINDOWS)                     // WINDOWS


/**
 *  \brief Map GetLastError to errno.
 */
static int get_error_code()
{
    DWORD error = GetLastError();
    switch (error) {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
            return ENOENT;
        case ERROR_BAD_ARGUMENTS:
            return EINVAL;
        case ERROR_TOO_MANY_OPEN_FILES:
            return EMFILE;
        case ERROR_ACCESS_DENIED:
            if (PYCPP_SKIP_PERMISSIONS_ERROR) {
                return 0;
            }
            return EACCES;
        case ERROR_NOT_ENOUGH_MEMORY:
        case ERROR_OUTOFMEMORY:
            return ENOMEM;
        default:
            return -1;
    }
}


static bool has_wildcards(const path_t& path)
{
    return std::any_of(path.begin(), path.end(), [](native_char_type c) {
        return c == path_prefix('*') || c == path_prefix('?');
    });
}


static bool has_wildcards(const backup_path_t& path)
{
    return std::any_of(path.begin(), path.end(), [](backup_char_type c) {
        return c == '*' || c == '?';
    });
}


/**
 *  \brief Check if the file is "." or "..";
 */
static bool is_relative_dot(const wchar_t* name)
{
    return (wcscmp(name, L".") == 0) || (wcscmp(name, L"..") == 0);
}


#else                                       // POSIX

#endif                                      // WINDOWS

// OBJECTS -- PLATFORM SPECIFIC
// ----------------------------

#if defined(OS_WINDOWS)                     // WINDOWS


/**
 *  \brief Generic base class for directory data.
 */
struct directory_data_impl
{
    stat_t *stat_ = nullptr;

    ~directory_data_impl();
    virtual path_t fullpath() const = 0;
    virtual path_t basename() const = 0;
    virtual const path_t& dirname() const = 0;
    const stat_t& stat();
    void open(HANDLE& handle, WIN32_FIND_DATAW*& data, const path_t& path);

    void increment(HANDLE& handle, WIN32_FIND_DATAW*& data);
};


directory_data_impl::~directory_data_impl()
{
    delete stat_;
}


void directory_data_impl::open(HANDLE& handle, WIN32_FIND_DATAW*& data, const path_t& path)
{
    // don't allow wildcards in the search
    if (has_wildcards(path)) {
        throw std::runtime_error("Cannot use wildcards in search.");
    }

    // get data
    auto find_data = new WIN32_FIND_DATAW;
    path_list_t paths = {path, path_prefix("*")};
    path_t joined = join(paths);
    auto str = reinterpret_cast<const wchar_t*>(joined.data());

    // create our handle
    handle = FindFirstFileW(str, find_data);
    if (handle == INVALID_HANDLE_VALUE) {
        delete find_data;
        handle_error(get_error_code());
    }

    // reset class data
    delete data;
    data = find_data;
}


template <typename FindNext, typename FindData>
static DWORD find_next_path(HANDLE handle, FindData* data, FindNext findnext)
{
    while (true) {
        if (findnext(handle, data) == 0) {
            DWORD error = GetLastError();
            if (PYCPP_SKIP_PERMISSIONS_ERROR && error == ERROR_ACCESS_DENIED) {
                continue;
            } else {
                return error;
            }
        } else if (!is_relative_dot(data->cFileName)) {
            return 0;
        }
    }
}


void directory_data_impl::increment(HANDLE& handle, WIN32_FIND_DATAW*& data)
{
    DWORD error = 0;
    error = find_next_path(handle, data, FindNextFileW);

    // handle any errors
    if (error == ERROR_NO_MORE_FILES) {
        delete data;
        data = nullptr;
    } else if (error != 0) {
        handle_error(get_error_code());
    }

    delete stat_;
    stat_ = nullptr;
}


/**
 *  \brief Data for a directory entry.
 */
struct directory_data: directory_data_impl
{
    HANDLE handle = INVALID_HANDLE_VALUE;
    path_t path;
    WIN32_FIND_DATAW* data = nullptr;

    ~directory_data();
    virtual path_t fullpath() const override;
    virtual path_t basename() const override;
    virtual const path_t& dirname() const override;

    void open(const path_t& path);
    void open(const backup_path_t& path);
    directory_data& operator++();
    directory_data operator++(int);
    bool operator==(const directory_data&) const;
    bool operator!=(const directory_data&) const;
    explicit operator bool() const;
};


directory_data::~directory_data()
{
    FindClose(handle);
    delete data;
}


path_t directory_data::basename() const
{
    return path_t(reinterpret_cast<char16_t*>(data->cFileName));
}


void directory_data::open(const path_t& p)
{
    path = p;
    directory_data_impl::open(handle, data, path);
}


void directory_data::open(const backup_path_t& p)
{
    open(ansi_to_utf16(p));
}


directory_data& directory_data::operator++()
{
    increment(handle, data);
    return *this;
}


bool directory_data::operator==(const directory_data& rhs) const
{
    return std::tie(handle, path, data) == std::tie(rhs.handle, rhs.path, rhs.data);
}


directory_data::operator bool() const
{
    return bool(data);
}


/**
 *  \brief Data for a recursive directory entry.
 */
struct recursive_directory_data: directory_data_impl
{
    std::deque<HANDLE> handle_list;
    path_list_t path_list;
    std::deque<WIN32_FIND_DATAW*> data_list;
    bool initialized = false;

    ~recursive_directory_data();
    virtual path_t fullpath() const override;
    virtual path_t basename() const override;
    virtual const path_t& dirname() const override;

    void open(const path_t& path);
    void open(const backup_path_t& path);
    recursive_directory_data& operator++();
    recursive_directory_data operator++(int);
    bool operator==(const recursive_directory_data&) const;
    bool operator!=(const recursive_directory_data&) const;
    explicit operator bool() const;
};


recursive_directory_data::~recursive_directory_data()
{
    std::for_each(handle_list.begin(), handle_list.end(), [](HANDLE handle) {
        FindClose(handle);
    });
    std::for_each(data_list.begin(), data_list.end(), [](WIN32_FIND_DATAW* data) {
        delete data;
    });
}


path_t recursive_directory_data::basename() const
{
    return path_t(reinterpret_cast<char16_t*>(data_list.back()->cFileName));
}


void recursive_directory_data::open(const path_t& p)
{
    path_list.emplace_back(p);
    handle_list.emplace_back(INVALID_HANDLE_VALUE);
    data_list.emplace_back(nullptr);
    directory_data_impl::open(handle_list.back(), data_list.back(), path_list.back());
}


void recursive_directory_data::open(const backup_path_t& p)
{
    open(ansi_to_utf16(p));
}


recursive_directory_data& recursive_directory_data::operator++()
{
    // directory start, add a level
    if (initialized && isdir(stat())) {
        path_list.emplace_back(fullpath());
        handle_list.emplace_back(INVALID_HANDLE_VALUE);
        data_list.emplace_back(nullptr);
        directory_data_impl::open(handle_list.back(), data_list.back(), path_list.back());
    }
    initialized = true;

    // increment until we don't lose a parent directory
    increment(handle_list.back(), data_list.back());
    while (!data_list.empty() && !data_list.back()) {
       // clean our values
        path_list.pop_back();
        handle_list.pop_back();
        data_list.pop_back();
        if (!data_list.empty()) {
            increment(handle_list.back(), data_list.back());
        }
    }

    return *this;
}


bool recursive_directory_data::operator==(const recursive_directory_data& rhs) const
{
    return std::tie(handle_list, path_list, data_list) == std::tie(rhs.handle_list, rhs.path_list, rhs.data_list);
}


recursive_directory_data::operator bool() const
{
    return data_list.empty() ? false : bool(data_list.front());
}


directory_iterator::directory_iterator(const path_t& path)
{
    entry_.ptr_.reset(new directory_data);
    entry_.ptr_->open(path);
    operator++();
}


directory_iterator::directory_iterator(const backup_path_t& path)
{
    entry_.ptr_.reset(new directory_data);
    entry_.ptr_->open(path);
    operator++();
}


recursive_directory_iterator::recursive_directory_iterator(const path_t& path)
{
    entry_.ptr_.reset(new recursive_directory_data);
    entry_.ptr_->open(path);
    operator++();
}


recursive_directory_iterator::recursive_directory_iterator(const backup_path_t& path)
{
    entry_.ptr_.reset(new recursive_directory_data);
    entry_.ptr_->open(path);
    operator++();
}


#else                                       // POSIX


/**
 *  \brief Generic base class for directory data.
 */
struct directory_data_impl
{
    dirent* entry = nullptr;
    stat_t *stat_ = nullptr;

    ~directory_data_impl();
    void reset();
    virtual path_t fullpath() const = 0;
    path_t basename() const;
    virtual const path_t& dirname() const = 0;
    const stat_t& stat();
    void open(DIR*& dir, const path_t& path);

    void increment(DIR*& dir);
    bool operator==(const directory_data_impl&) const;
    explicit operator bool() const;
};


directory_data_impl::~directory_data_impl()
{
    delete stat_;
}


path_t directory_data_impl::basename() const
{
    return path_t(entry->d_name);
}


void directory_data_impl::open(DIR*& dir, const path_t& path)
{
    dir = opendir(path.data());
    if (dir == nullptr) {
        handle_error(errno);
    }
}


void directory_data_impl::increment(DIR*& dir)
{
    errno = 0;
    do {
        entry = readdir(dir);
    } while (entry && is_relative_dot(entry->d_name));

    // check for any error handling
    int error = errno;
    if (!entry && errno != 0) {
        if (errno != EACCES || !PYCPP_SKIP_PERMISSIONS_ERROR) {
            handle_error(errno);
        }
    }

    delete stat_;
    stat_ = nullptr;
}


bool directory_data_impl::operator==(const directory_data_impl& rhs) const
{
    return entry == rhs.entry;
}


directory_data_impl::operator bool() const
{
    return bool(entry);
}


/**
 *  \brief Data for a directory entry.
 */
struct directory_data: directory_data_impl
{
    DIR* dir = nullptr;
    path_t path;

    ~directory_data();
    void open(const path_t& path);

    virtual path_t fullpath() const override;
    virtual const path_t& dirname() const override;

    directory_data& operator++();
    directory_data operator++(int);
    bool operator==(const directory_data&) const;
    bool operator!=(const directory_data&) const;
};


directory_data::~directory_data()
{
    closedir(dir);
}


void directory_data::open(const path_t& p)
{
    directory_data_impl::open(dir, p);
    path = p;
}


directory_data& directory_data::operator++()
{
    increment(dir);
    return *this;
}


bool directory_data::operator==(const directory_data& rhs) const
{
    return directory_data_impl::operator==(rhs) && std::tie(dir, path) == std::tie(rhs.dir, rhs.path);
}


/**
 *  \brief Data for a recursive directory entry.
 */
struct recursive_directory_data: directory_data_impl
{
    std::deque<DIR*> dir_list;
    path_list_t path_list;

    ~recursive_directory_data();
    virtual path_t fullpath() const override;
    virtual const path_t& dirname() const override;

    void open(const path_t& path);
    recursive_directory_data& operator++();
    recursive_directory_data operator++(int);
    bool operator==(const recursive_directory_data&) const;
    bool operator!=(const recursive_directory_data&) const;
};


recursive_directory_data::~recursive_directory_data()
{
    std::for_each(dir_list.begin(), dir_list.end(), [](DIR* dir) {
        closedir(dir);
    });
}


void recursive_directory_data::open(const path_t& p)
{
    path_list.emplace_back(p);
    dir_list.emplace_back(nullptr);
    directory_data_impl::open(dir_list.back(), p);
}


recursive_directory_data& recursive_directory_data::operator++()
{
    // directory start, add  a level
    if (entry && isdir(stat())) {
        path_list.emplace_back(fullpath());
        dir_list.emplace_back(nullptr);
        directory_data_impl::open(dir_list.back(), path_list.back());
    }

    // increment until we don't lose a parent directory
    increment(dir_list.back());
    if (!dir_list.empty() && !entry) {
        // clean our values
        path_list.pop_back();
        dir_list.pop_back();

        // if we still have parents, recurse
        if (!dir_list.empty()) {
            return operator++();
        }
    }

    return *this;
}


bool recursive_directory_data::operator==(const recursive_directory_data& rhs) const
{
    return directory_data_impl::operator==(rhs) && std::tie(dir_list, path_list) == std::tie(rhs.dir_list, rhs.path_list);
}


directory_iterator::directory_iterator(const path_t& path)
{
    entry_.ptr_.reset(new directory_data);
    entry_.ptr_->open(path);
    operator++();
}


recursive_directory_iterator::recursive_directory_iterator(const path_t& path)
{
    entry_.ptr_.reset(new recursive_directory_data);
    entry_.ptr_->open(path);
    operator++();
}


#endif                                      // WINDOWS


// OBJECTS -- GENERIC
// ------------------


const stat_t& directory_data_impl::stat()
{
    if (!stat_) {
        stat_ = new stat_t(lstat(fullpath()));
    }
    return *stat_;
}


path_t directory_data::fullpath() const
{
    path_list_t paths = {dirname(), basename()};
    return join(paths);
}


const path_t& directory_data::dirname() const
{
    return path;
}


auto directory_data::operator++(int) -> directory_data
{
    directory_data copy(*this);
    operator++();
    return copy;
}


bool directory_data::operator!=(const directory_data& rhs) const
{
    return !operator==(rhs);
}


path_t directory_entry::path() const
{
    return ptr_->fullpath();
}


path_t directory_entry::basename() const
{
    return ptr_->basename();
}


const path_t& directory_entry::dirname() const
{
    return ptr_->dirname();
}


const stat_t& directory_entry::stat() const
{
    return ptr_->stat();
}


bool directory_entry::isfile() const
{
    return PYCPP_NAMESPACE::isfile(stat());
}


bool directory_entry::isdir() const
{
    return PYCPP_NAMESPACE::isdir(stat());
}


bool directory_entry::islink() const
{
    return PYCPP_NAMESPACE::islink(stat());
}


bool directory_entry::exists() const
{
    return PYCPP_NAMESPACE::exists(stat());
}


void directory_entry::swap(self_t& rhs)
{
    std::swap(ptr_, rhs.ptr_);
}


bool directory_entry::operator==(const self_t& rhs) const
{
    if (ptr_ && rhs.ptr_) {
        return *ptr_ == *rhs.ptr_;
    }
    return ptr_ == rhs.ptr_;
}


bool directory_entry::operator!=(const self_t& rhs) const
{
    return !operator==(rhs);
}


directory_iterator::~directory_iterator()
{}


auto directory_iterator::operator++() -> self_t&
{
    entry_.ptr_->operator++();
    if (!*entry_.ptr_) {
        entry_.ptr_.reset();
    }
    return *this;
}


auto directory_iterator::operator++(int) -> self_t
{
    self_t copy(*this);
    operator++();
    return copy;
}


auto directory_iterator::operator->() -> pointer
{
    return &entry_;
}


auto directory_iterator::operator->() const -> const_pointer
{
    return &entry_;
}


auto directory_iterator::operator*() -> reference
{
    return entry_;
}


auto directory_iterator::operator*() const -> const_reference
{
    return entry_;
}


void directory_iterator::swap(self_t& rhs)
{
    std::swap(entry_, rhs.entry_);
}


bool directory_iterator::operator==(const self_t& rhs) const
{
    return entry_ == rhs.entry_;
}


bool directory_iterator::operator!=(const self_t& rhs) const
{
    return !operator==(rhs);
}


path_t recursive_directory_data::fullpath() const
{
    path_list_t paths = {dirname(), basename()};
    return join(paths);
}


const path_t& recursive_directory_data::dirname() const
{
    return path_list.back();
}


auto recursive_directory_data::operator++(int) -> recursive_directory_data
{
    recursive_directory_data copy(*this);
    operator++();
    return copy;
}


bool recursive_directory_data::operator!=(const recursive_directory_data& rhs) const
{
    return !operator==(rhs);
}


path_t recursive_directory_entry::path() const
{
    return ptr_->fullpath();
}


path_t recursive_directory_entry::basename() const
{
    return ptr_->basename();
}


const path_t& recursive_directory_entry::dirname() const
{
    return ptr_->dirname();
}


const stat_t& recursive_directory_entry::stat() const
{
    return ptr_->stat();
}


bool recursive_directory_entry::isfile() const
{
    return PYCPP_NAMESPACE::isfile(stat());
}


bool recursive_directory_entry::isdir() const
{
    return PYCPP_NAMESPACE::isdir(stat());
}


bool recursive_directory_entry::islink() const
{
    return PYCPP_NAMESPACE::islink(stat());
}


bool recursive_directory_entry::exists() const
{
    return PYCPP_NAMESPACE::exists(stat());
}


void recursive_directory_entry::swap(self_t& rhs)
{
    std::swap(ptr_, rhs.ptr_);
}


bool recursive_directory_entry::operator==(const self_t& rhs) const
{
    if (ptr_ && rhs.ptr_) {
        return *ptr_ == *rhs.ptr_;
    }
    return ptr_ == rhs.ptr_;
}


bool recursive_directory_entry::operator!=(const self_t& rhs) const
{
    return !operator==(rhs);
}


recursive_directory_iterator::~recursive_directory_iterator()
{}


auto recursive_directory_iterator::operator++() -> self_t&
{
    entry_.ptr_->operator++();
    if (!*entry_.ptr_) {
        entry_.ptr_.reset();
    }
    return *this;
}


auto recursive_directory_iterator::operator++(int) -> self_t
{
    self_t copy(*this);
    operator++();
    return copy;
}


auto recursive_directory_iterator::operator->() -> pointer
{
    return &entry_;
}


auto recursive_directory_iterator::operator->() const -> const_pointer
{
    return &entry_;
}


auto recursive_directory_iterator::operator*() -> reference
{
    return entry_;
}


auto recursive_directory_iterator::operator*() const -> const_reference
{
    return entry_;
}


void recursive_directory_iterator::swap(self_t& rhs)
{
    std::swap(entry_, rhs.entry_);
}


bool recursive_directory_iterator::operator==(const self_t& rhs) const
{
    return entry_ == rhs.entry_;
}


bool recursive_directory_iterator::operator!=(const self_t& rhs) const
{
    return !operator==(rhs);
}


PYCPP_END_NAMESPACE
