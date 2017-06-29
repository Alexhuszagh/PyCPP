//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  This idea comes from Colin Percival, since there is no complete,
 *  cross-platform way to free a buffer, however, you can use volatile
 *  pointers to force access to the pointer.
 *
 *  [reference]
 *      http://www.daemonology.net/blog/2014-09-04-how-to-zero-a-buffer.html
 */

#include "os.h"
#include "secure_string.h"

#include <cstring>
#if defined(OS_WINDOWS)
#   include "windows.h"
#endif

// FUNCTIONS
// ---------


#if defined(OS_WINDOWS)                 // WINDOWS

static void secure_zero_memory(void* ptr, size_t len)
{
    SecureZeroMemory(ptr, len);
}

#else                                   // POSIX

static void * (* const volatile memset_ptr)(void*, int, size_t) = memset;

static void secure_zero_memory(void* ptr, size_t len)
{
    (memset_ptr)(ptr, 0, len);
}

#endif


void secure_zero(void* dst, size_t bytes)
{
    secure_zero_memory(dst, bytes);
}
