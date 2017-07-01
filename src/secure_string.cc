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

#include "compiler.h"
#include "os.h"
#include "secure_string.h"

#if defined(OS_WINDOWS)
#   include "windows.h"
#else
#   include <string.h>
#   include <strings.h>
#endif

// MACROS
// ------

#define WEAK(x) asm("")

// FUNCTIONS
// ---------


#if defined(OS_WINDOWS)                 // WINDOWS

static void secure_zero_memory(void* ptr, size_t len)
{
    SecureZeroMemory(ptr, len);
}

#elif defined(HAVE_EXPLICIT_BZERO)      // EXPLICIT_BZERO

static void secure_zero_memory(void* ptr, size_t len)
{
    explicit_bzero(ptr, len);
}

#elif defined(HAVE_MEMSET_S)            // MEMSET_S

static void secure_zero_memory(void* ptr, size_t len)
{
    memset_s(ptr, len, 0, len);
}

#else                                   // POSIX

/**
 *  Some memset implementations use SSE/AVX instructions, meaning
 *  sensitive data will be stored in SSE registers after zeroing
 *  the data. Implement a slow memset version to ensure no SSE
 *  instructions are used.
 */
static void* memset_slow(void* ptr, int c, size_t len)
{
    volatile char* p = (volatile char*) ptr;

    while(len--) {
        *p++ = c;
    }
    return (void*) p;
}


static void * (* const volatile memset_ptr)(void*, int, size_t) = memset_slow;

static void secure_zero_memory(void* ptr, size_t len)
{
    (memset_ptr)(ptr, 0, len);
}

#endif


void secure_zero(void* dst, size_t bytes)
{
    secure_zero_memory(dst, bytes);
}
