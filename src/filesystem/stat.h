//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Cross-platform stat declarations.
 */

#pragma once

#include <cstddef>
#include <ctime>
#include <sys/types.h>


// OBJECTS
// -------


#if defined(_WIN32)                 // WINDOWS

struct stat_t
{
   dev_t          st_dev;
   ino_t          st_ino;
   unsigned short st_mode;
   short          st_nlink;
   short          st_uid;
   short          st_gid;
   dev_t          st_rdev;
   off_t          st_size;
   timespec       st_atim;
   timespec       st_mtim;
   timespec       st_ctim;
};

#else                               // POSIX

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

#endif
