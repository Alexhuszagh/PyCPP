//  :copyright: (c) 2013-2017 Frank Denis.
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

#include <compiler.h>
#include <os.h>
#include <secure/string.h>
#include <new>
#include <stdexcept>

#if defined(OS_WINDOWS)
#   include <windows.h>
#else
#   include <string.h>
#   include <strings.h>
#endif

// MACROS
// ------

#define WEAK(x) asm("")

// FUNCTIONS
// ---------

// SECURE MEMSET
// -------------


/**
 *  Some memset implementations use SSE/AVX instructions, meaning
 *  sensitive data will be stored in SSE registers after zeroing
 *  the data. Implement a slow memset version to ensure no SSE
 *  instructions are used.
 */
static void* memset_slow(void* dst, int c, size_t len)
{
    volatile char* d = (volatile char*) dst;

    while(len--) {
        *d++ = c;
    }
    return dst;
}


#if defined(HAVE_MEMSET_S)              // MEMSET_S

static void* memset_s_impl(void* dst, int c, size_t len)
{
    memset_s(dst, len, c, len);
    return dst;
}

static void * (* const volatile memset_ptr)(void*, int, size_t) = memset_s_impl;

#else                                   // MEMSET_SLOW

static void * (* const volatile memset_ptr)(void*, int, size_t) = memset_slow;

#endif


void* secure_memset(void* dst, int c, size_t len)
{
    return (memset_ptr)(dst, c, len);
}


// SECURE MEMCPY
// -------------


/**
 *  Some memcpy implementations use SSE/AVX instructions, meaning
 *  sensitive data will be stored in SSE registers after zeroing
 *  the data. Implement a slow memcpy version to ensure no SSE
 *  instructions are used.
 */
static void* memcpy_slow(void* dst, const void* src, size_t len)
{
    volatile char* d = (volatile char*) dst;
    volatile const char* s = (volatile const char*) src;

    while(len--) {
        *d++ = *s++;
    }
    return dst;
}


#if defined(HAVE_MEMCPY_S)              // MEMCPY_S

static void* memcpy_s_impl(void* dst, const void* src, size_t len)
{
    memcpy_s(dst, len, src, len);
    return dst;
}

static void * (* const volatile memcpy_ptr)(void*, const void*, size_t) = memcpy_s_impl;

#else                                   // MEMCPY_SLOW

static void * (* const volatile memcpy_ptr)(void*, const void*, size_t) = memcpy_slow;

#endif


void* secure_memcpy(void* dst, const void* src, size_t len)
{
    return (memcpy_ptr)(dst, src, len);
}

// SECURE MEMMOVE
// --------------

/**
 *  Some memcpy implementations use SSE/AVX instructions, meaning
 *  sensitive data will be stored in SSE registers after zeroing
 *  the data. Implement a slow memcpy version to ensure no SSE
 *  instructions are used.
 */
static void* memmove_slow(void* dst, const void* src, size_t len)
{
    volatile char* d = (volatile char*) dst;
    volatile const char* s = (volatile const char*) src;

    // check for non-overlapping regions
    if (d - s >= len) {
        return secure_memcpy(dst, src, len);
    }

    // overlapping regions
    s += len;
    d += len;
    while (len--) {
        *--d = *--s;
    }

    return dst;
}


#if defined(HAVE_MEMMOVE_S)              // MEMMOVE_S

static void* memmove_s_impl(void* dst, const void* src, size_t len)
{
    memmove_s(dst, len, src, len);
    return dst;
}

static void * (* const volatile memmove_ptr)(void*, const void*, size_t) = memmove_s_impl;

#else                                   // MEMMOVE_SLOW

static void * (* const volatile memmove_ptr)(void*, const void*, size_t) = memmove_slow;

#endif


void* secure_memmove(void* dst, const void* src, size_t len)
{
    return (memmove_ptr)(dst, src, len);
}

// SECURE ZERO
// -----------


#if defined(OS_WINDOWS)                 // WINDOWS

static void secure_zero_impl(void* ptr, size_t len)
{
    SecureZeroMemory(ptr, len);
}

#elif defined(HAVE_EXPLICIT_BZERO)      // EXPLICIT_BZERO

static void secure_zero_impl(void* ptr, size_t len)
{
    explicit_bzero(ptr, len);
}

#elif defined(HAVE_MEMSET_S)            // MEMSET_S

static void secure_zero_impl(void* ptr, size_t len)
{
    memset_s(ptr, len, 0, len);
}

#else                                   // POSIX

static void secure_zero_impl(void* ptr, size_t len)
{
    (memset_ptr)(ptr, 0, len);
}

#endif


void secure_zero(void* dst, size_t bytes)
{
    secure_zero_impl(dst, bytes);
}

// MALLOC
// ------

void* secure_malloc(size_t size)
{
    #pragma message("TODO: secure_malloc() not yet been implemented")
    void* ptr = malloc(size);
    if (size > 0 && ptr == nullptr) {
        throw std::bad_alloc();
    }
    return ptr;
}

// CALLOC
// ------

void* secure_calloc(size_t num, size_t size)
{
    #pragma message("TODO: secure_calloc() not yet been implemented")
    void* ptr = calloc(num, size);
    if (size > 0 && ptr == nullptr) {
        throw std::bad_alloc();
    }
    return ptr;
}

// FREE
// ----

void secure_free(void* ptr)
{
    #pragma message("TODO: secure_free() not yet been implemented")
    free(ptr);
}


// NOACCESS
// --------

int secure_protect_noaccess(void *ptr)
{
    #pragma message("TODO: secure_protect_noaccess() not yet been implemented")
    return -1;
}

// READONLY
// --------

int secure_protect_readonly(void *ptr)
{
    #pragma message("TODO: secure_protect_readonly() not yet been implemented")
    return -1;
}

// READWRITE
// ---------

int secure_protect_readwrite(void *ptr)
{
    #pragma message("TODO: secure_protect_readwrite() not yet been implemented")
    return -1;
}

// WEAK
// ----

// force asm("") to prevent volatility and prevent optimizations
WEAK(secure_memset);
WEAK(secure_memcpy);
WEAK(secure_zero);
