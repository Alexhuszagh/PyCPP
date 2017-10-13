//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/stream/random_access.h>

PYCPP_BEGIN_NAMESPACE


// POSIX
// int posix_fadvise(int fd, off_t offset, off_t len, int advice);

// WINDOWS
//FILE_FLAG_SEQUENTIAL_SCAN

PYCPP_END_NAMESPACE
