//  :copyright: (c) 2017 Joyent, Inc. and other Node contributors.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/preprocessor/os.h>
#include <pycpp/windows/error.h>

#if defined(OS_WINDOWS)
#   include <pycpp/preprocessor/errno.h>
#   include <pycpp/windows/winapi.h>
#   include <stdio.h>
#endif

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


#if defined(OS_WINDOWS)                     // WINDOWS

void set_errno_win32()
{
    // errno is thread-safe on POSIX and Windows.
    // On Windows, however, this feature is "undocumented", but
    // present as of MSVC 2008.
    DWORD code = GetLastError();
    // TODO: restore commented out codes
    switch (code) {
        case ERROR_NOACCESS:                    errno = EACCES;             break;
        case WSAEACCES:                         errno = EACCES;             break;
        case ERROR_ELEVATION_REQUIRED:          errno = EACCES;             break;
        case ERROR_ADDRESS_ALREADY_ASSOCIATED:  errno = EADDRINUSE;         break;
        case WSAEADDRINUSE:                     errno = EADDRINUSE;         break;
        case WSAEADDRNOTAVAIL:                  errno = EADDRNOTAVAIL;      break;
        case WSAEAFNOSUPPORT:                   errno = EAFNOSUPPORT;       break;
        case WSAEWOULDBLOCK:                    errno = EAGAIN;             break;
        case WSAEALREADY:                       errno = EALREADY;           break;
        case ERROR_INVALID_FLAGS:               errno = EBADF;              break;
        case ERROR_INVALID_HANDLE:              errno = EBADF;              break;
        case ERROR_LOCK_VIOLATION:              errno = EBUSY;              break;
        case ERROR_PIPE_BUSY:                   errno = EBUSY;              break;
        case ERROR_SHARING_VIOLATION:           errno = EBUSY;              break;
        case ERROR_OPERATION_ABORTED:           errno = ECANCELED;          break;
        case WSAEINTR:                          errno = ECANCELED;          break;
        case ERROR_NO_UNICODE_TRANSLATION:      errno = ECHARSET;           break;
        case ERROR_CONNECTION_ABORTED:          errno = ECONNABORTED;       break;
        case WSAECONNABORTED:                   errno = ECONNABORTED;       break;
        case ERROR_CONNECTION_REFUSED:          errno = ECONNREFUSED;       break;
        case WSAECONNREFUSED:                   errno = ECONNREFUSED;       break;
        case ERROR_NETNAME_DELETED:             errno = ECONNRESET;         break;
        case WSAECONNRESET:                     errno = ECONNRESET;         break;
        case ERROR_ALREADY_EXISTS:              errno = EEXIST;             break;
        case ERROR_FILE_EXISTS:                 errno = EEXIST;             break;
        case ERROR_BUFFER_OVERFLOW:             errno = EFAULT;             break;
        case WSAEFAULT:                         errno = EFAULT;             break;
        case ERROR_HOST_UNREACHABLE:            errno = EHOSTUNREACH;       break;
        case WSAEHOSTUNREACH:                   errno = EHOSTUNREACH;       break;
        case ERROR_INSUFFICIENT_BUFFER:         errno = EINVAL;             break;
        case ERROR_INVALID_DATA:                errno = EINVAL;             break;
        case ERROR_INVALID_PARAMETER:           errno = EINVAL;             break;
        case ERROR_SYMLINK_NOT_SUPPORTED:       errno = EINVAL;             break;
        case WSAEINVAL:                         errno = EINVAL;             break;
        case WSAEPFNOSUPPORT:                   errno = EINVAL;             break;
        case WSAESOCKTNOSUPPORT:                errno = EINVAL;             break;
        case ERROR_BEGINNING_OF_MEDIA:          errno = EIO;                break;
        case ERROR_BUS_RESET:                   errno = EIO;                break;
        case ERROR_CRC:                         errno = EIO;                break;
        case ERROR_DEVICE_DOOR_OPEN:            errno = EIO;                break;
        case ERROR_DEVICE_REQUIRES_CLEANING:    errno = EIO;                break;
        case ERROR_DISK_CORRUPT:                errno = EIO;                break;
        case ERROR_EOM_OVERFLOW:                errno = EIO;                break;
        case ERROR_FILEMARK_DETECTED:           errno = EIO;                break;
        case ERROR_GEN_FAILURE:                 errno = EIO;                break;
        case ERROR_INVALID_BLOCK_LENGTH:        errno = EIO;                break;
        case ERROR_IO_DEVICE:                   errno = EIO;                break;
        case ERROR_NO_DATA_DETECTED:            errno = EIO;                break;
        case ERROR_NO_SIGNAL_SENT:              errno = EIO;                break;
        case ERROR_OPEN_FAILED:                 errno = EIO;                break;
        case ERROR_SETMARK_DETECTED:            errno = EIO;                break;
        case ERROR_SIGNAL_REFUSED:              errno = EIO;                break;
        case WSAEISCONN:                        errno = EISCONN;            break;
        case ERROR_CANT_RESOLVE_FILENAME:       errno = ELOOP;              break;
        case ERROR_TOO_MANY_OPEN_FILES:         errno = EMFILE;             break;
        case WSAEMFILE:                         errno = EMFILE;             break;
        case WSAEMSGSIZE:                       errno = EMSGSIZE;           break;
        case ERROR_FILENAME_EXCED_RANGE:        errno = ENAMETOOLONG;       break;
        case ERROR_NETWORK_UNREACHABLE:         errno = ENETUNREACH;        break;
        case WSAENETUNREACH:                    errno = ENETUNREACH;        break;
        case WSAENOBUFS:                        errno = ENOBUFS;            break;
        case ERROR_BAD_PATHNAME:                errno = ENOENT;             break;
        case ERROR_DIRECTORY:                   errno = ENOENT;             break;
        case ERROR_FILE_NOT_FOUND:              errno = ENOENT;             break;
        case ERROR_INVALID_NAME:                errno = ENOENT;             break;
        case ERROR_INVALID_DRIVE:               errno = ENOENT;             break;
        case ERROR_INVALID_REPARSE_DATA:        errno = ENOENT;             break;
        case ERROR_MOD_NOT_FOUND:               errno = ENOENT;             break;
        case ERROR_PATH_NOT_FOUND:              errno = ENOENT;             break;
        case WSAHOST_NOT_FOUND:                 errno = ENOENT;             break;
        case WSANO_DATA:                        errno = ENOENT;             break;
        case ERROR_NOT_ENOUGH_MEMORY:           errno = ENOMEM;             break;
        case ERROR_OUTOFMEMORY:                 errno = ENOMEM;             break;
        case ERROR_CANNOT_MAKE:                 errno = ENOSPC;             break;
        case ERROR_DISK_FULL:                   errno = ENOSPC;             break;
        case ERROR_EA_TABLE_FULL:               errno = ENOSPC;             break;
        case ERROR_END_OF_MEDIA:                errno = ENOSPC;             break;
        case ERROR_HANDLE_DISK_FULL:            errno = ENOSPC;             break;
        case ERROR_NOT_CONNECTED:               errno = ENOTCONN;           break;
        case WSAENOTCONN:                       errno = ENOTCONN;           break;
        case ERROR_DIR_NOT_EMPTY:               errno = ENOTEMPTY;          break;
        case WSAENOTSOCK:                       errno = ENOTSOCK;           break;
        case ERROR_NOT_SUPPORTED:               errno = ENOTSUP;            break;
        case ERROR_BROKEN_PIPE:                 errno = EOF;                break;
        case ERROR_ACCESS_DENIED:               errno = EPERM;              break;
        case ERROR_PRIVILEGE_NOT_HELD:          errno = EPERM;              break;
        case ERROR_BAD_PIPE:                    errno = EPIPE;              break;
        case ERROR_NO_DATA:                     errno = EPIPE;              break;
        case ERROR_PIPE_NOT_CONNECTED:          errno = EPIPE;              break;
        case WSAESHUTDOWN:                      errno = EPIPE;              break;
        case WSAEPROTONOSUPPORT:                errno = EPROTONOSUPPORT;    break;
        case ERROR_WRITE_PROTECT:               errno = EROFS;              break;
        case ERROR_SEM_TIMEOUT:                 errno = ETIMEDOUT;          break;
        case WSAETIMEDOUT:                      errno = ETIMEDOUT;          break;
        case ERROR_NOT_SAME_DEVICE:             errno = EXDEV;              break;
        case ERROR_INVALID_FUNCTION:            errno = EISDIR;             break;
        case ERROR_META_EXPANSION_TOO_LONG:     errno = E2BIG;              break;
        default:                                errno = EUNKNOWN;           break;
    }
}

#else                                       // POSIX

void set_errno_win32()      // nullopt
{}

#endif                                      // WINDOWS

PYCPP_END_NAMESPACE
