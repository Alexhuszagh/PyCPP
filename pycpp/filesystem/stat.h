//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Cross-platform stat declarations.
 */

#pragma once

#include <cstddef>
#include <ctime>
#include <sys/types.h>

PYCPP_BEGIN_NAMESPACE

#if defined(_WIN32)                 // WINDOWS

// ALIAS
// -----

using mode_t = unsigned short;
using nlink_t = short;
using uid_t = short;
using gid_t = short;

#endif

// OBJECTS
// -------

struct stat_t
{
    dev_t    st_dev;
    ino_t    st_ino;
    mode_t   st_mode;
    nlink_t  st_nlink;
    uid_t    st_uid;
    gid_t    st_gid;
    dev_t    st_rdev;
    off_t    st_size;
    timespec st_atim;
    timespec st_mtim;
    timespec st_ctim;
};


PYCPP_END_NAMESPACE
