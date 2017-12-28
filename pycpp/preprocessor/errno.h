//  :copyright: (c) 2017 Joyent, Inc. and other Node contributors.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Cross-platform errno definitions.
 *
 *  Reasonable defaults for non-standard errno constants.
 *
 *  \synopsis
 *      #define EADDRINFO       implementation-defined
 *      #define EADDRINUSE      implementation-defined
 *      #define EADDRNOTAVAIL   implementation-defined
 *      #define EAFNOSUPPORT    implementation-defined
 *      #define EAGAIN          implementation-defined
 *      #define EALREADY        implementation-defined
 *      #define EBADF           implementation-defined
 *      #define EBUSY           implementation-defined
 *      #define ECANCELED       implementation-defined
 *      #define ECHARSET        implementation-defined
 *      #define ECONNABORTED    implementation-defined
 *      #define ECONNREFUSED    implementation-defined
 *      #define ECONNRESET      implementation-defined
 *      #define EDESTADDRREQ    implementation-defined
 *      #define EEXIST          implementation-defined
 *      #define EFAULT          implementation-defined
 *      #define EHOSTUNREACH    implementation-defined
 *      #define EINTR           implementation-defined
 *      #define EINVAL          implementation-defined
 *      #define EIO             implementation-defined
 *      #define EISCONN         implementation-defined
 *      #define EISDIR          implementation-defined
 *      #define ELOOP           implementation-defined
 *      #define EMFILE          implementation-defined
 *      #define EMSGSIZE        implementation-defined
 *      #define ENAMETOOLONG    implementation-defined
 *      #define ENETDOWN        implementation-defined
 *      #define ENETUNREACH     implementation-defined
 *      #define ENFILE          implementation-defined
 *      #define ENOBUFS         implementation-defined
 *      #define ENODEV          implementation-defined
 *      #define ENOENT          implementation-defined
 *      #define ENOMEM          implementation-defined
 *      #define ENONET          implementation-defined
 *      #define ENOSPC          implementation-defined
 *      #define ENOSYS          implementation-defined
 *      #define ENOTCONN        implementation-defined
 *      #define ENOTDIR         implementation-defined
 *      #define ENOTEMPTY       implementation-defined
 *      #define ENOTSOCK        implementation-defined
 *      #define ENOTSUP         implementation-defined
 *      #define EPERM           implementation-defined
 *      #define EPIPE           implementation-defined
 *      #define EPROTO          implementation-defined
 *      #define EPROTONOSUPPORT implementation-defined
 *      #define EPROTOTYPE      implementation-defined
 *      #define EROFS           implementation-defined
 *      #define ESHUTDOWN       implementation-defined
 *      #define ESPIPE          implementation-defined
 *      #define ESRCH           implementation-defined
 *      #define ETIMEDOUT       implementation-defined
 *      #define EUNKNOWN        implementation-defined
 *      #define EXDEV           implementation-defined
 */

#pragma once

#include <errno.h>

// MACROS
// ------

#if !defined(EUNKNOWN)
// non-standard error code, give it a custom value
#   define EUNKNOWN 8000
#endif

#if !defined(EADDRINFO)
#   define EADDRINFO 4092
#endif

#if !defined(EADDRINUSE)
#   define EADDRINUSE 4092
#endif

#if !defined(EADDRNOTAVAIL)
#   define EADDRNOTAVAIL 4091
#endif

#if !defined(EAFNOSUPPORT)
#   define EAFNOSUPPORT 4090
#endif

#if !defined(EAGAIN)
#   define EAGAIN 4089
#endif

#if !defined(EALREADY)
#   define EALREADY 4085
#endif

#if !defined(EBADF)
#   define EBADF 4084
#endif

#if !defined(EBUSY)
#   define EBUSY 4083
#endif

#if !defined(ECANCELED)
#   define ECANCELED 4082
#endif

#if !defined(ECHARSET)
#   define ECHARSET 4081
#endif

#if !defined(ECONNABORTED)
#   define ECONNABORTED 4080
#endif

#if !defined(ECONNREFUSED)
#   define ECONNREFUSED 4079
#endif

#if !defined(ECONNRESET)
#   define ECONNRESET 4078
#endif

#if !defined(EDESTADDRREQ)
#   define EDESTADDRREQ 4077
#endif

#if !defined(EEXIST)
#   define EEXIST 4076
#endif

#if !defined(EFAULT)
#   define EFAULT 4075
#endif

#if !defined(EHOSTUNREACH)
#   define EHOSTUNREACH 4074
#endif

#if !defined(EINTR)
#   define EINTR 4073
#endif

#if !defined(EINVAL)
#   define EINVAL 4072
#endif

#if !defined(EIO)
#   define EIO 4071
#endif

#if !defined(EISCONN)
#   define EISCONN 4070
#endif

#if !defined(EISDIR)
#   define EISDIR 4069
#endif

#if !defined(ELOOP)
#   define ELOOP 4068
#endif

#if !defined(EMFILE)
#   define EMFILE 4067
#endif

#if !defined(EMSGSIZE)
#   define EMSGSIZE 4066
#endif

#if !defined(ENAMETOOLONG)
#   define ENAMETOOLONG 4065
#endif

#if !defined(ENETDOWN)
#   define ENETDOWN 4064
#endif

#if !defined(ENETUNREACH)
#   define ENETUNREACH 4063
#endif

#if !defined(ENFILE)
#   define ENFILE 4062
#endif

#if !defined(ENOBUFS)
#   define ENOBUFS 4061
#endif

#if !defined(ENODEV)
#   define ENODEV 4060
#endif

#if !defined(ENOENT)
#   define ENOENT 4059
#endif

#if !defined(ENOMEM)
#   define ENOMEM 4058
#endif

#if !defined(ENONET)
#   define ENONET 4057
#endif

#if !defined(ENOSPC)
#   define ENOSPC 4056
#endif

#if !defined(ENOSYS)
#   define ENOSYS 4055
#endif

#if !defined(ENOTCONN)
#   define ENOTCONN 4054
#endif

#if !defined(ENOTDIR)
#   define ENOTDIR 4053
#endif

#if !defined(ENOTEMPTY)
#   define ENOTEMPTY 4052
#endif

#if !defined(ENOTSOCK)
#   define ENOTSOCK 4051
#endif

#if !defined(ENOTSUP)
#   define ENOTSUP 4050
#endif

#if !defined(EPERM)
#   define EPERM 4049
#endif

#if !defined(EPIPE)
#   define EPIPE 4048
#endif

#if !defined(EPROTO)
#   define EPROTO 4047
#endif

#if !defined(EPROTONOSUPPORT)
#   define EPROTONOSUPPORT 4046
#endif

#if !defined(EPROTOTYPE)
#   define EPROTOTYPE 4045
#endif

#if !defined(EROFS)
#   define EROFS 4044
#endif

#if !defined(ESHUTDOWN)
#   define ESHUTDOWN 4043
#endif

#if !defined(ESPIPE)
#   define ESPIPE 4042
#endif

#if !defined(ESRCH)
#   define ESRCH 4041
#endif

#if !defined(ETIMEDOUT)
#   define ETIMEDOUT 4040
#endif

#if !defined(EXDEV)
#   define EXDEV 4038
#endif
