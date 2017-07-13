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
 *  \brief Data for a directory entry.
 */
struct directory_data
{
    HANDLE handle = INVALID_HANDLE_VALUE;
    path_t path;
    stat_t *stat = nullptr;
    bool is_unicode = false;
    void* data = nullptr;

    ~directory_data();
    void reset();
    path_t fullpath() const;
    path_t basename() const;
    const path_t& dirname() const;

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
    reset();
    FindClose(handle);
    delete stat;
}


void directory_data::reset()
{
    if (is_unicode) {
        delete (WIN32_FIND_DATAW*) data;
    } else {
        delete (WIN32_FIND_DATAA*) data;
    }
    data = nullptr;
}


void directory_data::open(const path_t& p)
{
    // don't allow wildcards in the search
    if (has_wildcards(p)) {
        throw std::runtime_error("Cannot use wildcards in search.");
    }

    // get data
    auto find_data = new WIN32_FIND_DATAW;
    path_list_t paths = {p, path_prefix("*")};
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
    path = p;
    is_unicode = true;
    data = (void*) find_data;
}


void directory_data::open(const backup_path_t& p)
{
    // don't allow wildcards in the search
    if (has_wildcards(p)) {
        throw std::runtime_error("Cannot use wildcards in search.");
    }

    // get data
    auto d = new WIN32_FIND_DATAA;
    backup_path_list_t paths = {p, "*"};
    backup_path_t joined = join(paths);
    auto str = reinterpret_cast<const char*>(joined.data());

    // create our handle
    handle = FindFirstFileA(str, d);
    if (handle == INVALID_HANDLE_VALUE) {
        delete d;
        handle_error(get_error_code());
    }

    // reset class data
    path = ansi_to_utf16(p);
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


directory_data& directory_data::operator++()
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

    return *this;
}


bool directory_data::operator==(const directory_data& rhs) const
{
    return std::tie(handle, path, is_unicode, data) == std::tie(rhs.handle, rhs.path, rhs.is_unicode, rhs.data);
}


directory_data::operator bool() const
{
    return bool(data);
}


path_t directory_data::basename() const
{
    if (is_unicode) {
        auto d = (WIN32_FIND_DATAW*) data;
        return path_t(reinterpret_cast<char16_t*>(d->cFileName));
    } else {
        auto d = (WIN32_FIND_DATAA*) data;
        return path_t(ansi_to_utf16(d->cFileName));
    }
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
 *  \brief Data for a directory entry.
 */
struct directory_data
{
    DIR* dir = nullptr;
    dirent* entry = nullptr;
    path_t path;
    stat_t *stat = nullptr;

    ~directory_data();
    void open(const path_t& path);

    path_t fullpath() const;
    path_t basename() const;
    const path_t& dirname() const;

    directory_data& operator++();
    directory_data operator++(int);
    bool operator==(const directory_data&) const;
    bool operator!=(const directory_data&) const;
    explicit operator bool() const;
};


directory_data::~directory_data()
{
    closedir((DIR*) dir);
    delete stat;
}


void directory_data::open(const path_t& p)
{
    path = p;
    dir = opendir(path.data());
    if (dir == nullptr) {
        handle_error(errno);
    }
}


path_t directory_data::basename() const
{
    return path_t(entry->d_name);
}


directory_data& directory_data::operator++()
{
    do {
        entry = readdir(dir);
    } while (entry && is_relative_dot(entry->d_name));

    return *this;
}


directory_data::operator bool() const
{
    return bool(entry);
}


bool directory_data::operator==(const directory_data& rhs) const
{
    return std::tie(dir, path, entry) == std::tie(rhs.dir, rhs.path, rhs.entry);
}


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
    if (ptr_->stat) {
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
