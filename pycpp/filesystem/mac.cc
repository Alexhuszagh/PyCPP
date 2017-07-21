//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief macOS-specific path normalization routines.
 */

#include <pycpp/os.h>

#if defined(OS_MACOS)
#include <pycpp/filesystem.h>
#include <fcntl.h>
#include <unistd.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


bool file_allocate(fd_t fd, size_t size)
{
    fstore_t store = {F_ALLOCATECONTIG, F_PEOFPOSMODE, 0, static_cast<off_t>(size), 0};
    int status = fcntl(fd, F_PREALLOCATE, &store);
    if (status == -1) {
        store.fst_flags = F_ALLOCATEALL;
        status = fcntl(fd, F_PREALLOCATE, &store);
    }

    if (status != -1) {
        // required for OS X to properly report the length
        // fnctl returns anything but -1 on success, but truncate returns
        // 0 on success, so we can guarantee -1 is error, 0 is success.
        status = ftruncate(fd, static_cast<off_t>(size));
    }

    return status == 0;
}

PYCPP_END_NAMESPACE

#endif
