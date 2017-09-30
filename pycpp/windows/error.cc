//  :copyright: (c) 2017 Joyent, Inc. and other Node contributors.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/preprocessor/os.h>
#include <pycpp/windows/error.h>

#if defined(OS_WINDOWS)
#   include <errno.h>
#   include <windows.h>
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
    switch (code) {
        case ERROR_NOACCESS:                    errno = EACCES;         break;
        case WSAEACCES:                         errno = EACCES;         break;
        case ERROR_ELEVATION_REQUIRED:          errno = EACCES;         break;
        case ERROR_ADDRESS_ALREADY_ASSOCIATED:  errno = EADDRINUSE;     break;
        case WSAEADDRINUSE:                     errno = EADDRINUSE;     break;
        case WSAEADDRNOTAVAIL:                  errno = EADDRNOTAVAIL;  break;
        case WSAEAFNOSUPPORT:                   errno = EAFNOSUPPORT;   break;
//    case WSAEWOULDBLOCK:                    return UV_EAGAIN;
//    case WSAEALREADY:                       return UV_EALREADY;
//    case ERROR_INVALID_FLAGS:               return UV_EBADF;
//    case ERROR_INVALID_HANDLE:              return UV_EBADF;
//    case ERROR_LOCK_VIOLATION:              return UV_EBUSY;
//    case ERROR_PIPE_BUSY:                   return UV_EBUSY;
//    case ERROR_SHARING_VIOLATION:           return UV_EBUSY;
//    case ERROR_OPERATION_ABORTED:           return UV_ECANCELED;
//    case WSAEINTR:                          return UV_ECANCELED;
//    case ERROR_NO_UNICODE_TRANSLATION:      return UV_ECHARSET;
//    case ERROR_CONNECTION_ABORTED:          return UV_ECONNABORTED;
//    case WSAECONNABORTED:                   return UV_ECONNABORTED;
//    case ERROR_CONNECTION_REFUSED:          return UV_ECONNREFUSED;
//    case WSAECONNREFUSED:                   return UV_ECONNREFUSED;
//    case ERROR_NETNAME_DELETED:             return UV_ECONNRESET;
//    case WSAECONNRESET:                     return UV_ECONNRESET;
//    case ERROR_ALREADY_EXISTS:              return UV_EEXIST;
//    case ERROR_FILE_EXISTS:                 return UV_EEXIST;
//    case ERROR_BUFFER_OVERFLOW:             return UV_EFAULT;
//    case WSAEFAULT:                         return UV_EFAULT;
//    case ERROR_HOST_UNREACHABLE:            return UV_EHOSTUNREACH;
//    case WSAEHOSTUNREACH:                   return UV_EHOSTUNREACH;
//    case ERROR_INSUFFICIENT_BUFFER:         return UV_EINVAL;
//    case ERROR_INVALID_DATA:                return UV_EINVAL;
//    case ERROR_INVALID_PARAMETER:           return UV_EINVAL;
//    case ERROR_SYMLINK_NOT_SUPPORTED:       return UV_EINVAL;
//    case WSAEINVAL:                         return UV_EINVAL;
//    case WSAEPFNOSUPPORT:                   return UV_EINVAL;
//    case WSAESOCKTNOSUPPORT:                return UV_EINVAL;
//    case ERROR_BEGINNING_OF_MEDIA:          return UV_EIO;
//    case ERROR_BUS_RESET:                   return UV_EIO;
//    case ERROR_CRC:                         return UV_EIO;
//    case ERROR_DEVICE_DOOR_OPEN:            return UV_EIO;
//    case ERROR_DEVICE_REQUIRES_CLEANING:    return UV_EIO;
//    case ERROR_DISK_CORRUPT:                return UV_EIO;
//    case ERROR_EOM_OVERFLOW:                return UV_EIO;
//    case ERROR_FILEMARK_DETECTED:           return UV_EIO;
//    case ERROR_GEN_FAILURE:                 return UV_EIO;
//    case ERROR_INVALID_BLOCK_LENGTH:        return UV_EIO;
//    case ERROR_IO_DEVICE:                   return UV_EIO;
//    case ERROR_NO_DATA_DETECTED:            return UV_EIO;
//    case ERROR_NO_SIGNAL_SENT:              return UV_EIO;
//    case ERROR_OPEN_FAILED:                 return UV_EIO;
//    case ERROR_SETMARK_DETECTED:            return UV_EIO;
//    case ERROR_SIGNAL_REFUSED:              return UV_EIO;
//    case WSAEISCONN:                        return UV_EISCONN;
//    case ERROR_CANT_RESOLVE_FILENAME:       return UV_ELOOP;
//    case ERROR_TOO_MANY_OPEN_FILES:         return UV_EMFILE;
//    case WSAEMFILE:                         return UV_EMFILE;
//    case WSAEMSGSIZE:                       return UV_EMSGSIZE;
//    case ERROR_FILENAME_EXCED_RANGE:        return UV_ENAMETOOLONG;
//    case ERROR_NETWORK_UNREACHABLE:         return UV_ENETUNREACH;
//    case WSAENETUNREACH:                    return UV_ENETUNREACH;
//    case WSAENOBUFS:                        return UV_ENOBUFS;
//    case ERROR_BAD_PATHNAME:                return UV_ENOENT;
//    case ERROR_DIRECTORY:                   return UV_ENOENT;
//    case ERROR_FILE_NOT_FOUND:              return UV_ENOENT;
//    case ERROR_INVALID_NAME:                return UV_ENOENT;
//    case ERROR_INVALID_DRIVE:               return UV_ENOENT;
//    case ERROR_INVALID_REPARSE_DATA:        return UV_ENOENT;
//    case ERROR_MOD_NOT_FOUND:               return UV_ENOENT;
//    case ERROR_PATH_NOT_FOUND:              return UV_ENOENT;
//    case WSAHOST_NOT_FOUND:                 return UV_ENOENT;
//    case WSANO_DATA:                        return UV_ENOENT;
//    case ERROR_NOT_ENOUGH_MEMORY:           return UV_ENOMEM;
//    case ERROR_OUTOFMEMORY:                 return UV_ENOMEM;
//    case ERROR_CANNOT_MAKE:                 return UV_ENOSPC;
//    case ERROR_DISK_FULL:                   return UV_ENOSPC;
//    case ERROR_EA_TABLE_FULL:               return UV_ENOSPC;
//    case ERROR_END_OF_MEDIA:                return UV_ENOSPC;
//    case ERROR_HANDLE_DISK_FULL:            return UV_ENOSPC;
//    case ERROR_NOT_CONNECTED:               return UV_ENOTCONN;
//    case WSAENOTCONN:                       return UV_ENOTCONN;
//    case ERROR_DIR_NOT_EMPTY:               return UV_ENOTEMPTY;
//    case WSAENOTSOCK:                       return UV_ENOTSOCK;
//    case ERROR_NOT_SUPPORTED:               return UV_ENOTSUP;
//    case ERROR_BROKEN_PIPE:                 return UV_EOF;
//    case ERROR_ACCESS_DENIED:               return UV_EPERM;
//    case ERROR_PRIVILEGE_NOT_HELD:          return UV_EPERM;
//    case ERROR_BAD_PIPE:                    return UV_EPIPE;
//    case ERROR_NO_DATA:                     return UV_EPIPE;
//    case ERROR_PIPE_NOT_CONNECTED:          return UV_EPIPE;
//    case WSAESHUTDOWN:                      return UV_EPIPE;
//    case WSAEPROTONOSUPPORT:                return UV_EPROTONOSUPPORT;
//    case ERROR_WRITE_PROTECT:               return UV_EROFS;
//    case ERROR_SEM_TIMEOUT:                 return UV_ETIMEDOUT;
//    case WSAETIMEDOUT:                      return UV_ETIMEDOUT;
//    case ERROR_NOT_SAME_DEVICE:             return UV_EXDEV;
//    case ERROR_INVALID_FUNCTION:            return UV_EISDIR;
//    case ERROR_META_EXPANSION_TOO_LONG:     return UV_E2BIG;
//        default:        errno = UNKNOWN;    break;
    }
}

#else                                       // POSIX

void set_errno_win32()      // nullopt
{}

#endif                                      // WINDOWS

PYCPP_END_NAMESPACE
