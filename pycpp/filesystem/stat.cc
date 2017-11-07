//  :copyright: (c) 2017 Joyent, Inc. and other Node contributors.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/filesystem/exception.h>
#include <pycpp/preprocessor/errno.h>
#include <pycpp/preprocessor/sysstat.h>
#include <sys/types.h>

#if defined(OS_WINDOWS)
#   include <pycpp/windows/winapi.h>
#   include <winerror.h>
#   include <tuple>
#else
#   include <fcntl.h>
#   include <limits.h>
#   include <unistd.h>
#   include <utime.h>
#endif

PYCPP_BEGIN_NAMESPACE

// MACROS
// -------

#ifndef S_IREAD
#   define S_IREAD _S_IREAD
#endif

#ifndef S_IWRITE
#   define S_IWRITE _S_IWRITE
#endif

#ifndef S_IFLNK
#   define S_IFLNK 0120000
#endif

#ifndef S_ISREG
#   define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif

#ifndef S_ISDIR
#   define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

#ifndef S_ISCHR
#   define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR)
#endif

#ifndef S_ISBLK
#   define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK)
#endif

#ifndef S_ISFIFO
#   define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#endif

#ifndef S_ISLNK
#   define S_ISLNK(m) (((m) & S_IFMT) == S_IFLNK)
#endif

#ifndef S_ISSOCK
#   define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
#endif

#ifndef S_ISDOOR
#   define S_ISDOOR(m) (0)
#endif

#ifndef S_ISPORT
#   define S_ISPORT(m) (0)
#endif

#ifndef S_ISWHT
#   define S_ISWHT(m) (0)
#endif


#if defined(OS_WINDOWS)
#   ifndef ERROR_SYMLINK_NOT_SUPPORTED
#       define ERROR_SYMLINK_NOT_SUPPORTED 1464
#   endif
#endif


// OBJECTS
// -------


#if defined(OS_WINDOWS)                 // WINDOWS

typedef struct _REPARSE_DATA_BUFFER {
  ULONG  ReparseTag;
  USHORT ReparseDataLength;
  USHORT Reserved;
  union {
    struct {
      USHORT SubstituteNameOffset;
      USHORT SubstituteNameLength;
      USHORT PrintNameOffset;
      USHORT PrintNameLength;
      ULONG  Flags;
      WCHAR  PathBuffer[1];
    } SymbolicLinkReparseBuffer;
    struct {
      USHORT SubstituteNameOffset;
      USHORT SubstituteNameLength;
      USHORT PrintNameOffset;
      USHORT PrintNameLength;
      WCHAR  PathBuffer[1];
    } MountPointReparseBuffer;
    struct {
      UCHAR DataBuffer[1];
    } GenericReparseBuffer;
  };
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;

#endif


// HELPERS
// -------

// STAT


static void handle_error(int code)
{
    switch (code) {
        case 0:
            return;
        case ENOENT:
            throw filesystem_error(filesystem_file_not_found);
         case EINVAL:
            throw filesystem_error(filesystem_invalid_parameter);
        case ENOMEM:
            throw filesystem_error(filesystem_out_of_memory);
         default:
            throw filesystem_error(filesystem_unexpected_error);
    }
}


#if defined(OS_WINDOWS)         // WINDOWS


#define FILETIME_TO_UINT(filetime)                                          \
   (*((uint64_t*) &(filetime)) - 116444736000000000ULL)


#define FILETIME_TO_TIME_T(filetime)                                        \
   (FILETIME_TO_UINT(filetime) / 10000000ULL)

#define FILETIME_TO_TIME_NS(filetime, secs)                                 \
   ((FILETIME_TO_UINT(filetime) - (secs * 10000000ULL)) * 100)


#define FILETIME_TO_TIMESPEC(ts, filetime)                                  \
   do {                                                                     \
     (ts).tv_sec = (long) FILETIME_TO_TIME_T(filetime);                     \
     (ts).tv_nsec = (long) FILETIME_TO_TIME_NS(filetime, (ts).tv_sec);      \
   } while(0)


#define TIME_T_TO_FILETIME(time, filetime)                                      \
  do {                                                                          \
    uint64_t bigtime = ((int64_t) (time) * 10000000LL) + 116444736000000000ULL; \
    (filetime).dwLowDateTime = bigtime & 0xFFFFFFFF;                            \
    (filetime).dwHighDateTime = bigtime >> 32;                                  \
  } while(0)


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


static int copy_stat(HANDLE handle, stat_t* buffer)
{
    BY_HANDLE_FILE_INFORMATION info;
    if (!GetFileInformationByHandle(handle, &info)) {
        return get_error_code();
    }

    // set our information
    buffer->st_dev = info.dwVolumeSerialNumber;

    ULARGE_INTEGER ul;
    buffer->st_mode = 0;
    if (info.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
        buffer->st_mode |= S_IFLNK;
        buffer->st_size = 0;
    } else if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        buffer->st_mode |= S_IFDIR;
        buffer->st_size = 0;
    } else {
        ul.HighPart = info.nFileSizeHigh;
        ul.LowPart = info.nFileSizeLow;
        buffer->st_mode |= S_IFREG;
        buffer->st_size = static_cast<off_t>(ul.QuadPart);
    }

    // get permissions
    int mode;
    if (info.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
        mode = S_IREAD;
    } else {
        mode = S_IREAD | S_IWRITE;
    }
    buffer->st_mode |= mode | (mode >> 3) | (mode >> 6);

    // get filetimes
    FILETIME_TO_TIMESPEC(buffer->st_ctim, info.ftCreationTime);
    FILETIME_TO_TIMESPEC(buffer->st_atim, info.ftLastAccessTime);
    FILETIME_TO_TIMESPEC(buffer->st_mtim, info.ftLastWriteTime);

    ul.HighPart = info.nFileIndexHigh;
    ul.LowPart = info.nFileIndexLow;
    buffer->st_ino = static_cast<ino_t>(ul.QuadPart);
    buffer->st_nlink = static_cast<nlink_t>(info.nNumberOfLinks);

    // Windows doesn't define these values
    buffer->st_gid = 0;
    buffer->st_uid = 0;
    buffer->st_rdev = 0;

    return 0;
}



template <typename Pointer, typename Function>
static HANDLE get_handle(Pointer path, bool use_lstat, Function function)
{
    DWORD access = FILE_READ_ATTRIBUTES;
    DWORD share = 0;
    LPSECURITY_ATTRIBUTES security = nullptr;
    DWORD create = OPEN_EXISTING;
    DWORD flags = FILE_FLAG_BACKUP_SEMANTICS;
    HANDLE file = nullptr;
    if (use_lstat) {
        flags |= FILE_FLAG_OPEN_REPARSE_POINT;
    }

    return function(path, access, share, security, create, flags, file);
}


static HANDLE get_handle(const path_t& path, bool use_lstat)
{
    auto data = reinterpret_cast<const wchar_t*>(path.data());
    return get_handle(data, use_lstat, CreateFileW);
}


static HANDLE get_handle(const backup_path_t& path, bool use_lstat)
{
    auto data = reinterpret_cast<const char*>(path.data());
    return get_handle(data, use_lstat, CreateFileA);
}


template <typename Path>
static bool set_stat_impl(const Path& src, const Path& dst)
{
    auto src_stat = stat(src);
    HANDLE handle = get_handle(dst, false);
    FILETIME atime, mtime;

    // update filetime
    TIME_T_TO_FILETIME(src_stat.st_atim.tv_sec, atime);
    TIME_T_TO_FILETIME(src_stat.st_mtim.tv_sec, mtime);
    if (!SetFileTime(handle, NULL, &atime, &mtime)) {
        fd_close(handle);
        return false;
    }

    // we don't define st_uid, and st_gid, so don't implement them

    fd_close(handle);
    return true;
}


static DWORD get_ioctrl(HANDLE handle, REPARSE_DATA_BUFFER* buf)
{
    DWORD code = FSCTL_GET_REPARSE_POINT;
    LPVOID in = 0;
    DWORD in_size = 0;
    LPVOID out = buf;
    DWORD out_size = sizeof(REPARSE_DATA_BUFFER);
    DWORD bytes;
    LPOVERLAPPED overlapped = 0;

    return DeviceIoControl(handle, code, in, in_size, out, out_size, &bytes, overlapped);
}


template <typename Path>
static int stat_impl(const Path& path, stat_t* buffer, bool use_lstat)
{
    HANDLE handle = get_handle(path, use_lstat);
    if (handle == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        if (use_lstat && (error == ERROR_SYMLINK_NOT_SUPPORTED || error == ERROR_NOT_A_REPARSE_POINT)) {
            return stat_impl(path, buffer, false);
        }
        return get_error_code();
    }
    int status = copy_stat(handle, buffer);
    fd_close(handle);
    if (status) {
        return get_error_code();
    }

    return 0;
}


static path_t read_link_impl(HANDLE handle)
{
    typedef typename path_t::value_type Char;

    // get our io control code
    auto buf = new REPARSE_DATA_BUFFER;
    DWORD io = get_ioctrl(handle, buf);
    fd_close(handle);

    // handle errors
    if (io == 0) {
        handle_error(get_error_code());
    } else if (buf->ReparseTag != IO_REPARSE_TAG_SYMLINK) {
        throw filesystem_error(filesystem_not_a_symlink);
    }

    // get data from link (length is stored in bytes, convert to u16)
    auto link = buf->SymbolicLinkReparseBuffer.PathBuffer;
    link += buf->SymbolicLinkReparseBuffer.PrintNameOffset;
    auto length = buf->SymbolicLinkReparseBuffer.PrintNameLength / 2;

    // create path
    path_t out(reinterpret_cast<Char*>(link), length);
    delete buf;

    return out;
}

static int stat(const backup_path_t& path, stat_t* buffer, bool use_lstat)
{
    return stat_impl(path, buffer, use_lstat);
}


static int wstat(const path_t& path, stat_t* buffer, bool use_lstat)
{
    return stat_impl(path, buffer, use_lstat);
}


stat_t stat(const path_t& path)
{
    stat_t data;
    int code;

    code = wstat(path, &data, false);
    handle_error(code);

    return data;
}


stat_t lstat(const path_t& path)
{
    stat_t data;
    int code;

    code = wstat(path, &data, true);
    handle_error(code);

    return data;
}


path_t read_link(const path_t& path)
{
    HANDLE handle = get_handle(path, true);
    if (handle == INVALID_HANDLE_VALUE) {
        handle_error(get_error_code());
    }

    return read_link_impl(handle);
}

#else                           // POSIX


template <typename Path>
static bool set_stat_impl(const Path& src, const Path& dst)
{
    auto src_stat = stat(src);
    utimbuf times = {src_stat.st_atim.tv_sec, src_stat.st_mtim.tv_sec};

    // update filetime
    int status = utime(dst.data(), &times);
    if (status != 0) {
        return false;
    }

    // get open file descriptor
    int fd = open(dst.data(), O_RDWR);
    if (fd == -1) {
        return false;
    }

    // update owner
    status = fchown(fd, src_stat.st_uid, src_stat.st_gid);
    if (status != 0) {
        close(fd);
        return false;
    }

    // update permission
    status = fchmod(fd, src_stat.st_mode);
    if (status != 0) {
        close(fd);
        return false;
    }

    return close(fd) == 0;
}


static void copy_native(const struct stat& src, stat_t& dst)
{
    dst.st_dev = src.st_dev;
    dst.st_ino = src.st_ino;
    dst.st_mode = src.st_mode;
    dst.st_nlink = src.st_nlink;
    dst.st_uid = src.st_uid;
    dst.st_gid = src.st_gid;
    dst.st_rdev = src.st_rdev;
    dst.st_size = src.st_size;

    // ignore the missing precision for backwards compatibility
    dst.st_atim = {src.st_atime, 0};
    dst.st_mtim = {src.st_mtime, 0};
    dst.st_ctim = {src.st_ctime, 0};
}


stat_t stat(const path_t& path)
{
    struct stat sb;
    stat_t data;

    if (::stat(path.c_str(), &sb) == -1) {
        handle_error(errno);
    }
    copy_native(sb, data);

    return data;
}


stat_t lstat(const path_t& path)
{
    struct stat sb;
    stat_t data;

    if (::lstat(path.c_str(), &sb) == -1)  {
        handle_error(errno);
    }
    copy_native(sb, data);

    return data;
}


path_t read_link(const path_t& path)
{
    typedef typename path_t::value_type Char;

    auto buf = new Char[PATH_MAX];
    auto length = ::readlink(path.data(), buf, PATH_MAX);
    if (length == -1) {
        handle_error(errno);
    }

    path_t link(buf, length);
    delete[] buf;

    return link;
}

#endif

#if defined(OS_WINDOWS)             // BACKUP PATH

stat_t stat(const backup_path_t& path)
{
    stat_t data;
    int code;

    code = stat(path, &data, false);
    handle_error(code);

    return data;
}


stat_t lstat(const backup_path_t& path)
{
    stat_t data;
    int code;

    code = stat(path, &data, true);
    handle_error(code);

    return data;
}


backup_path_t read_link(const backup_path_t& path)
{
    HANDLE handle = get_handle(path, true);
    if (handle == INVALID_HANDLE_VALUE) {
        handle_error(get_error_code());
    }

    return path_to_string(read_link_impl(handle));
}

#endif

// STAT PROPERTIES

time_t getatime(const stat_t& s)
{
    return s.st_atim.tv_sec;
}


time_t getmtime(const stat_t& s)
{
    return s.st_mtim.tv_sec;
}


time_t getctime(const stat_t& s)
{
    return s.st_ctim.tv_sec;
}


off_t getsize(const stat_t& s)
{
    return s.st_size;
}


bool exists(const stat_t& s)
{
    return true;
}


bool isfile(const stat_t& s)
{
    return S_ISREG(s.st_mode);
}


bool isdir(const stat_t& s)
{
    return S_ISDIR(s.st_mode);
}


bool islink(const stat_t& s)
{
    return S_ISLNK(s.st_mode);
}


static bool exists_stat(const stat_t& s)
{
    return exists(s);
}


static bool isfile_stat(const stat_t& s)
{
    return isfile(s);
}


static bool isdir_stat(const stat_t& s)
{
    return isdir(s);
}


static bool islink_stat(const stat_t& s)
{
    return islink(s);
}

// PATH PROPERTIES


template <typename Path, typename Function>
static bool check_impl(const Path& path, Function function, bool use_lstat = false)
{
    try {
        if (use_lstat) {
            return function(lstat(path));
        }
        return function(stat(path));
    } catch (filesystem_error& error) {
        if (error.code() == filesystem_file_not_found) {
            return false;
        }
        throw;
    }
}


template <typename Path>
static bool exists_impl(const Path& path)
{
    return check_impl(path, exists_stat);
}


template <typename Path>
static bool lexists_impl(const Path& path)
{
    return check_impl(path, exists_stat, true);
}


template <typename Path>
static bool isfile_impl(const Path& path)
{
    return check_impl(path, isfile_stat);
}


template <typename Path>
static bool isdir_impl(const Path& path)
{
    return check_impl(path, isdir_stat);
}


template <typename Path>
static bool islink_impl(const Path& path)
{
    return check_impl(path, islink_stat, true);
}


template <typename Path>
static bool copystat_impl(const Path& src, const Path& dst)
{
    return set_stat_impl(src, dst);
}

// FUNCTIONS
// ---------


time_t getatime(const path_t& path)
{
    return getatime(stat(path));
}


time_t getmtime(const path_t& path)
{
    return getmtime(stat(path));
}


time_t getctime(const path_t& path)
{
    return getctime(stat(path));
}


off_t getsize(const path_t& path)
{
    return getsize(stat(path));
}


bool isfile(const path_t& path)
{
    return isfile_impl(path);
}


bool isdir(const path_t& path)
{
    return isdir_impl(path);
}


bool islink(const path_t& path)
{
    return islink_impl(path);
}


bool exists(const path_t& path)
{
    return exists_impl(path);
}


bool lexists(const path_t& path)
{
    return lexists_impl(path);
}


bool samefile(const path_t& p1, const path_t& p2)
{
    return samestat(stat(p1), stat(p2));
}


bool samestat(const stat_t& s1, const stat_t& s2)
{
    return s1.st_ino == s2.st_ino && s1.st_dev == s2.st_dev;
}

bool copystat(const path_t& src, const path_t& dst)
{
    return copystat_impl(src, dst);
}


#if defined(OS_WINDOWS)             // BACKUP PATH


time_t getatime(const backup_path_t& path)
{
    return getatime(stat(path));
}


time_t getmtime(const backup_path_t& path)
{
    return getmtime(stat(path));
}


time_t getctime(const backup_path_t& path)
{
    return getctime(stat(path));
}


off_t getsize(const backup_path_t& path)
{
    return getsize(stat(path));
}


bool isfile(const backup_path_t& path)
{
    return isfile_impl(path);
}


bool isdir(const backup_path_t& path)
{
    return isdir_impl(path);
}


bool islink(const backup_path_t& path)
{
    return islink_impl(path);
}


bool exists(const backup_path_t& path)
{
    return exists_impl(path);
}


bool lexists(const backup_path_t& path)
{
    return lexists_impl(path);
}


bool samefile(const backup_path_t& p1, const backup_path_t& p2)
{
    return samestat(stat(p1), stat(p2));
}


bool copystat(const backup_path_t& src, const backup_path_t& dst)
{
    return copystat_impl(src, dst);
}

#endif

PYCPP_END_NAMESPACE
