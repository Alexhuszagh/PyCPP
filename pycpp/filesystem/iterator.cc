//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/filesystem/exception.h>
#include <errno.h>
#include <string.h>
#include <algorithm>
#include <tuple>
#if defined(OS_WINDOWS)
#   include <windows.h>
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
    stat_t *stat = nullptr;
    bool is_unicode = false;
    void* data = nullptr;

    ~directory_data_impl();
    void reset();
    path_t basename() const;
    void open(HANDLE& handle, const path_t& path);
    void open(HANDLE& handle, const backup_path_t& path);

    void increment(HANDLE& handle);
    bool operator==(const directory_data_impl&) const;
    explicit operator bool() const;
};


directory_data_impl::~directory_data_impl()
{
    reset();
    delete stat;
}


void directory_data_impl::reset()
{
    if (is_unicode) {
        delete (WIN32_FIND_DATAW*) data;
    } else {
        delete (WIN32_FIND_DATAA*) data;
    }
    data = nullptr;
}


path_t directory_data_impl::basename() const
{
    if (is_unicode) {
        auto d = (WIN32_FIND_DATAW*) data;
        return path_t(reinterpret_cast<char16_t*>(d->cFileName));
    } else {
        auto d = (WIN32_FIND_DATAA*) data;
        return path_t(ansi_to_utf16(d->cFileName));
    }
}


void directory_data_impl::open(HANDLE& handle, const path_t& path)
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
    reset();
    is_unicode = true;
    data = (void*) find_data;
}


void directory_data_impl::open(HANDLE& handle, const backup_path_t& path)
{
    // don't allow wildcards in the search
    if (has_wildcards(path)) {
        throw std::runtime_error("Cannot use wildcards in search.");
    }

    // get data
    auto d = new WIN32_FIND_DATAA;
    backup_path_list_t paths = {path, "*"};
    backup_path_t joined = join(paths);
    auto str = reinterpret_cast<const char*>(joined.data());

    // create our handle
    handle = FindFirstFileA(str, d);
    if (handle == INVALID_HANDLE_VALUE) {
        delete d;
        handle_error(get_error_code());
    }

    // reset class data
    reset();
    is_unicode = false;
    data = (void*) d;
}


template <typename FindNext, typename FindData>
static DWORD find_next_path(HANDLE handle, FindData* data, FindNext findnext)
{
    while (true) {
        if (findnext(handle, data) == 0) {
            return GetLastError();
        } else if (!is_relative_dot(data->cFileName)) {
            return 0;
        }
    }
}


void directory_data_impl::increment(HANDLE& handle)
{
    DWORD error = 0;
    if (is_unicode) {
        auto d = (WIN32_FIND_DATAW*) data;
        error = find_next_path(handle, d, FindNextFileW);
    } else {
        auto d = (WIN32_FIND_DATAA*) data;
        error = find_next_path(handle, d, FindNextFileA);
    }

    // handle any errors
    if (error == ERROR_NO_MORE_FILES) {
        reset();
    } else if (error != 0) {
        handle_error(get_error_code());
    }
}


bool directory_data_impl::operator==(const directory_data_impl& rhs) const
{
    return std::tie(is_unicode, data) == std::tie(rhs.is_unicode, rhs.data);
}


directory_data_impl::operator bool() const
{
    return bool(data);
}


/**
 *  \brief Data for a directory entry.
 */
struct directory_data: directory_data_impl
{
    HANDLE handle = INVALID_HANDLE_VALUE;
    path_t path;

    ~directory_data();
    path_t fullpath() const;
    const path_t& dirname() const;

    void open(const path_t& path);
    void open(const backup_path_t& path);
    directory_data& operator++();
    directory_data operator++(int);
    bool operator==(const directory_data&) const;
    bool operator!=(const directory_data&) const;
};


directory_data::~directory_data()
{
    FindClose(handle);
}


void directory_data::open(const path_t& p)
{
    directory_data_impl::open(handle, p);
    path = p;
}


void directory_data::open(const backup_path_t& p)
{
    directory_data_impl::open(handle, p);
    path = ansi_to_utf16(p);
}


directory_data& directory_data::operator++()
{
    increment(handle);
    return *this;
}


bool directory_data::operator==(const directory_data& rhs) const
{
    return directory_data_impl::operator==(rhs) && std::tie(handle, path) == std::tie(rhs.handle, rhs.path);
}


/**
 *  \brief Data for a recursive directory entry.
 */
struct recursive_directory_data: directory_data_impl
{
    std::deque<HANDLE> handle_list;
    path_list_t path_list;

    ~recursive_directory_data();
    void open(const path_t& path);
    void open(const backup_path_t& path);

    path_t fullpath() const;
    const path_t& dirname() const;
};


recursive_directory_data::~recursive_directory_data()
{
    std::for_each(handle_list.begin(), handle_list.end(), [](HANDLE handle) {
        FindClose(handle);
    });
}


void recursive_directory_data::open(const path_t& p)
{
    path_list.emplace_back(p);
    handle_list.emplace_back(nullptr);
    directory_data_impl::open(handle_list.back(), p);
}


void recursive_directory_data::open(const backup_path_t& p)
{
    path_list.emplace_back(ansi_to_utf16(p));
    handle_list.emplace_back(nullptr);
    directory_data_impl::open(handle_list.back(), p);
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


// TODO: need to think long and hard about this one...
//recursive_directory_iterator::recursive_directory_iterator(const path_t& path):
//    ptr_(new directory_data)
//{
//    ptr_->open(path);
//    operator++();
//}
//
//
//recursive_directory_iterator::recursive_directory_iterator(const backup_path_t& path):
//    ptr_(new directory_data)
//{
//    ptr_->open(path);
//    operator++();
//}


#else                                       // POSIX


/**
 *  \brief Generic base class for directory data.
 */
struct directory_data_impl
{
    dirent* entry = nullptr;
    stat_t *stat = nullptr;

    ~directory_data_impl();
    void reset();
    path_t basename() const;
    void open(DIR*& dir, const path_t& path);

    void increment(DIR*& dir);
    bool operator==(const directory_data_impl&) const;
    explicit operator bool() const;
};


directory_data_impl::~directory_data_impl()
{
    delete stat;
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
    do {
        entry = readdir(dir);
    } while (entry && is_relative_dot(entry->d_name));
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

    path_t fullpath() const;
    const path_t& dirname() const;

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
    void open(const path_t& path);

    path_t fullpath() const;
    const path_t& dirname() const;
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


// TODO: implement recursive_directory_data..


directory_iterator::directory_iterator(const path_t& path)
{
    entry_.ptr_.reset(new directory_data);
    entry_.ptr_->open(path);
    operator++();
}


//recursive_directory_iterator::recursive_directory_iterator(const path_t& path):
//    ptr_(new directory_data)
//{
//    ptr_->open(path);
//    operator++();
//}


#endif                                      // WINDOWS


// OBJECTS -- GENERIC
// ------------------


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


bool directory_entry::operator==(const self& rhs) const
{
    if (ptr_ && rhs.ptr_) {
        return *ptr_ == *rhs.ptr_;
    }
    return ptr_ == rhs.ptr_;
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
    if (!ptr_->stat) {
        ptr_->stat = new stat_t(lstat(path()));
    }
    return *ptr_->stat;
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


void directory_entry::swap(self& rhs)
{
    std::swap(ptr_, rhs.ptr_);
}


bool directory_entry::operator!=(const self& rhs) const
{
    return !operator==(rhs);
}


directory_iterator::~directory_iterator()
{}


auto directory_iterator::operator++() -> self&
{
    entry_.ptr_->operator++();
    if (!*entry_.ptr_) {
        entry_.ptr_.reset();
    }
    return *this;
}


auto directory_iterator::operator++(int) -> self
{
    self copy(*this);
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


void directory_iterator::swap(self& rhs)
{
    std::swap(entry_, rhs.entry_);
}


bool directory_iterator::operator==(const self& rhs) const
{
    return entry_ == rhs.entry_;
}


bool directory_iterator::operator!=(const self& rhs) const
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


// TODO: need to think long and hard about this one...
//recursive_directory_iterator::~recursive_directory_iterator()
//{}
//
//
//auto recursive_directory_iterator::operator++() -> self&
//{
//    ptr_->operator++();
//    if (!*ptr_) {
//        ptr_.reset();
//    }
//    return *this;
//}
//
//
//auto recursive_directory_iterator::operator++(int) -> self
//{
//    self copy(*this);
//    operator++();
//    return copy;
//}
//
//
//auto recursive_directory_iterator::operator->() -> pointer
//{
//    return &ptr_->entry;
//}
//
//
//auto recursive_directory_iterator::operator->() const -> const_pointer
//{
//    return &ptr_->entry;
//}
//
//
//auto recursive_directory_iterator::operator*() -> reference
//{
//    return ptr_->entry;
//}
//
//
//auto recursive_directory_iterator::operator*() const -> const_reference
//{
//    return ptr_->entry;
//}
//
//
//void recursive_directory_iterator::swap(self& rhs)
//{
//    std::swap(ptr_, rhs.ptr_);
//}
//
//
//bool recursive_directory_iterator::operator==(const self& rhs) const
//{
//    if (ptr_ && rhs.ptr_) {
//        return *ptr_ == *rhs.ptr_;
//    }
//    return ptr_ == rhs.ptr_;
//}
//
//
//bool recursive_directory_iterator::operator!=(const self& rhs) const
//{
//    return !operator==(rhs);
//}


PYCPP_END_NAMESPACE
