//  :copyright: (c) 2013-2017 Frank Denis.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  This for the secure_mem* implementations from Colin Percival, since
 *  there is no complete, cross-platform way to free a buffer, however,
 *  you can use volatile pointers to force access to the pointer.
 *
 *  [reference]
 *      http://www.daemonology.net/blog/2014-09-04-how-to-zero-a-buffer.html
 */

#define __STDC_WANT_LIB_EXT1__ 1

#include <pycpp/random.h>
#include <pycpp/preprocessor/compiler.h>
#include <pycpp/preprocessor/errno.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/secure/string.h>
#include <assert.h>
#include <stdlib.h>
#include <new>
#include <stdexcept>

#if defined(HAVE_SYS_MMAN_H)
#   include <sys/mman.h>
#endif

#if defined(OS_WINDOWS)
#   include <windows.h>
#else
#   include <string.h>
#   include <strings.h>
#   include <unistd.h>
#endif

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

#define CANARY_SIZE 16U
#define GARBAGE_VALUE 0xdb

#ifdef HAVE_MSVC
#   define WEAK_ATTRIBUTE
#   define MALLOC_ATTRIBUTE
#   define WEAK_ASM(x)
#else
#   define WEAK_ATTRIBUTE __attribute__((weak))
#   define MALLOC_ATTRIBUTE __attribute__((malloc))
#   define WEAK_ASM(x) asm("")
#endif

#if !defined(MAP_ANON) && defined(MAP_ANONYMOUS)
#   define MAP_ANON MAP_ANONYMOUS
#endif

#if defined(OS_WINAPI_DESKTOP) || (defined(MAP_ANON) && defined(HAVE_MMAP)) || defined(HAVE_POSIX_MEMALIGN)
#   define HAVE_ALIGNED_ALLOC
#endif

#if defined(HAVE_ALIGNED_ALLOC) && (defined(OS_WINAPI_DESKTOP) || defined(HAVE_MPROTECT))
#   define HAVE_PAGE_PROTECTION
#endif

#if !defined(MADV_DODUMP) && defined(MADV_CORE)
#   define MADV_DODUMP MADV_CORE
#   define MADV_DONTDUMP MADV_NOCORE
#elif !defined(MADV_DODUMP)
#   define MADV_DODUMP 0
#   define MADV_DONTDUMP 0
#endif

#ifndef MAP_NOCORE
#   define MAP_NOCORE 0
#endif

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
WEAK_ATTRIBUTE void* memset_slow(void* dst, int c, size_t len)
{
    volatile char* d = (volatile char*) dst;

    while(len--) {
        *d++ = c;
    }
    return dst;
}


#if defined(HAVE_MEMSET_S)              // MEMSET_S

WEAK_ATTRIBUTE void* memset_s_impl(void* dst, int c, size_t len)
{
    memset_s(dst, len, c, len);
    return dst;
}

static void * (* const volatile memset_ptr)(void*, int, size_t) = memset_s_impl;

#else                                   // MEMSET_SLOW

static void * (* const volatile memset_ptr)(void*, int, size_t) = memset_slow;

#endif


WEAK_ATTRIBUTE void* secure_memset(void* dst, int c, size_t len)
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
WEAK_ATTRIBUTE void* memcpy_slow(void* dst, const void* src, size_t len)
{
    volatile char* d = (volatile char*) dst;
    volatile const char* s = (volatile const char*) src;

    while(len--) {
        *d++ = *s++;
    }
    return dst;
}


#if defined(HAVE_MEMCPY_S)              // MEMCPY_S

WEAK_ATTRIBUTE void* memcpy_s_impl(void* dst, const void* src, size_t len)
{
    memcpy_s(dst, len, src, len);
    return dst;
}

static void * (* const volatile memcpy_ptr)(void*, const void*, size_t) = memcpy_s_impl;

#else                                   // MEMCPY_SLOW

static void * (* const volatile memcpy_ptr)(void*, const void*, size_t) = memcpy_slow;

#endif


WEAK_ATTRIBUTE void* secure_memcpy(void* dst, const void* src, size_t len)
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
WEAK_ATTRIBUTE void* memmove_slow(void* dst, const void* src, size_t len)
{
    volatile char* d = (volatile char*) dst;
    volatile const char* s = (volatile const char*) src;

    // check for non-overlapping regions
    if (static_cast<size_t>(d - s) >= len) {
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

WEAK_ATTRIBUTE void* memmove_s_impl(void* dst, const void* src, size_t len)
{
    memmove_s(dst, len, src, len);
    return dst;
}

static void * (* const volatile memmove_ptr)(void*, const void*, size_t) = memmove_s_impl;

#else                                   // MEMMOVE_SLOW

static void * (* const volatile memmove_ptr)(void*, const void*, size_t) = memmove_slow;

#endif


WEAK_ATTRIBUTE void* secure_memmove(void* dst, const void* src, size_t len)
{
    return (memmove_ptr)(dst, src, len);
}

// MEMCMP
// ------


WEAK_ATTRIBUTE void memcmp_prevent_lto(volatile const uint8_t* lhs, volatile const uint8_t* rhs, size_t len)
{
    (void) lhs;
    (void) rhs;
    (void) len;
}

/**
 *  Some memcmp implementations use SSE/AVX instructions, meaning
 *  sensitive data will be stored in SSE registers after zeroing
 *  the data. Implement a slow memcmp version to ensure no SSE
 *  instructions are used.
 */
int memcmp_slow(const void* lhs, const void* rhs, size_t len)
{
    volatile const uint8_t* s1 = (volatile const uint8_t*) lhs;
    volatile const uint8_t* s2 = (volatile const uint8_t*) rhs;

    memcmp_prevent_lto(s1, s2, len);

    unsigned char u1, u2;
    for ( ; len-- ; s1++, s2++) {
        u1 = *s1;
        u2 = *s2;
        if (u1 != u2) {
            return u1 - u2;
        }
    }
    return 0;
}

static int (* const volatile memcmp_ptr)(const void*, const void*, size_t) = memcmp_slow;


WEAK_ATTRIBUTE int secure_memcmp(const void* lhs, const void* rhs, size_t len)
{
    return (memcmp_ptr)(lhs, rhs, len);
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

// CANARY
// ------

static uint8_t CANARY[CANARY_SIZE];


static const uint8_t* get_canary_impl()
{
    if (sysrandom(CANARY, CANARY_SIZE) != CANARY_SIZE) {
        throw std::runtime_error("Cannot read from CSPRNG for canary.");
    }

    return CANARY;
}


static const uint8_t* get_canary()
{
    static const uint8_t* canary = get_canary_impl();
    return canary;
}

// PAGE SIZE
// ---------


#if defined(HAVE_ALIGNED_ALLOC)                    // HAVE ALIGNED ALLOC

static size_t check_page_size(size_t size)
{
    if (size < CANARY_SIZE || size < sizeof(size_t)) {
        throw std::runtime_error("Cannot get page size");
    }
    return size;
}


#if defined(_SC_PAGESIZE)                           // SC_PAGESIZE

static size_t get_page_size_impl()
{
    static long page_size = sysconf(_SC_PAGESIZE);
    if (page_size > 0L) {
        return check_page_size(page_size);
    }
    return check_page_size(0);
}

#elif defined(OS_WINAPI_DESKTOP)

static size_t get_page_size_impl()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);

    return check_page_size(info.dwPageSize);
}

#else

#   error Bug

#endif


static size_t get_page_size()
{
    static size_t page_size = get_page_size_impl();
    return page_size;
}


static size_t page_round(size_t size)
{
    size_t page_mask = get_page_size() - 1U;
    return (size + page_mask) & ~page_mask;
}

#endif


// LOCK
// ----


#if defined(HAVE_MADVISE)                       // MADVISE

static void madvise_impl(void* ptr, size_t length, int advice)
{
    madvise(ptr, length, advice);
}

#else                                           // NO MADVISE

static void madvise_impl(void* ptr, size_t length, int advice)
{}

#endif

#if defined(HAVE_MLOCK)                         // MLOCK

static int mlock_impl(void* ptr, size_t len)
{
    return mlock(ptr, len);
}


static int munlock_impl(void* ptr, size_t len)
{
    return munlock(ptr, len);
}

#elif defined(OS_WINAPI_DESKTOP)                 // WINDOWS API

static int mlock_impl(void* ptr, size_t len)
{
    return -(VirtualLock(ptr, len) == 0);
}


static int munlock_impl(void* ptr, size_t len)
{
    return -(VirtualUnlock(ptr, len) == 0);
}

#else                                           // NO MLOCK

static int mlock_impl(void* ptr, size_t len)
{
    errno = ENOSYS;
    return -1;
}


static int munlock_impl(void* ptr, size_t len)
{
    errno = ENOSYS;
    return -1;
}

#endif


int secure_mlock(void* ptr, const size_t len)
{
    madvise_impl(ptr, len, MADV_DONTDUMP);
    return mlock_impl(ptr, len);
}


int secure_munlock(void* ptr, const size_t len)
{
    secure_zero(ptr, len);
    madvise_impl(ptr, len, MADV_DODUMP);
    return munlock_impl(ptr, len);
}

// PAGE PROTECTION
// ---------------

static uint8_t* unprotected_ptr_from_user_ptr(const void* ptr)
{
    uintptr_t unprotected_ptr_u;
    uint8_t* canary_ptr;
    size_t page_mask;
    static size_t page_size = get_page_size();

    canary_ptr = ((uint8_t*) ptr) - CANARY_SIZE;
    page_mask = page_size - 1U;
    unprotected_ptr_u = ((uintptr_t) canary_ptr & (uintptr_t) ~page_mask);
    if (unprotected_ptr_u <= page_size * 2U) {
        throw std::runtime_error("Unable to get unprotected ptr.");
    }

    return (uint8_t*) unprotected_ptr_u;
}

#if defined(HAVE_PAGE_PROTECTION)

static void* secure_memcpy_page_protection(void* dst, const void* src, size_t n)
{
    return dst;
}


static int secure_memcmp_page_protection(const void* lhs, const void* rhs, size_t n)
{
    return 0;
}


static int secure_protect(void *ptr, int (*cb)(void *ptr, size_t size))
{
    uint8_t* base_ptr;
    uint8_t* unprotected_ptr;
    size_t unprotected_size;
    static size_t page_size = get_page_size();

    unprotected_ptr = unprotected_ptr_from_user_ptr(ptr);
    base_ptr = unprotected_ptr - page_size * 2U;
    memcpy(&unprotected_size, base_ptr, sizeof unprotected_size);

    return cb(unprotected_ptr, unprotected_size);
}

#else

static void* secure_memcpy_page_protection(void* dst, const void* src, size_t n)
{
    return memcpy(dst, src, n);
}


static int secure_memcmp_page_protection(const void* lhs, const void* rhs, size_t n)
{
    return secure_memcmp(lhs, rhs, n);
}


static int secure_protect(void *ptr, int (*cb)(void *ptr, size_t size))
{
    (void) ptr;
    (void) cb;
    errno = ENOSYS;
    return -1;
}

#endif

// NOACCESS
// --------


static int secure_mprotect_noaccess_impl(void* ptr, size_t size)
{
#ifdef HAVE_MPROTECT
    return mprotect(ptr, size, PROT_NONE);
#elif defined(OS_WINAPI_DESKTOP)
    DWORD old;
    return -(VirtualProtect(ptr, size, PAGE_NOACCESS, &old) == 0);
#else
    errno = ENOSYS;
    return -1;
#endif
}


int secure_mprotect_noaccess(void *ptr)
{
    return secure_protect(ptr, secure_mprotect_noaccess_impl);
}

// READONLY
// --------

static int secure_mprotect_readonly_impl(void* ptr, size_t size)
{
#ifdef HAVE_MPROTECT
    return mprotect(ptr, size, PROT_READ);
#elif defined(OS_WINAPI_DESKTOP)
    DWORD old;
    return -(VirtualProtect(ptr, size, PAGE_READONLY, &old) == 0);
#else
    errno = ENOSYS;
    return -1;
#endif
}


int secure_mprotect_readonly(void *ptr)
{
    return secure_protect(ptr, secure_mprotect_readonly_impl);
}

// READWRITE
// ---------

static int secure_mprotect_readwrite_impl(void* ptr, size_t size)
{
#ifdef HAVE_MPROTECT
    return mprotect(ptr, size, PROT_READ | PROT_WRITE);
#elif defined(OS_WINAPI_DESKTOP)
    DWORD old;
    return -(VirtualProtect(ptr, size, PAGE_READWRITE, &old) == 0);
#else
    errno = ENOSYS;
    return -1;
#endif
}


int secure_mprotect_readwrite(void *ptr)
{
    return secure_protect(ptr, secure_mprotect_readwrite_impl);
}


// MALLOC
// ------

#if defined(MAP_ANON) && defined(HAVE_MMAP)                 // MMAP

static MALLOC_ATTRIBUTE void* aligned_alloc_impl(size_t size)
{
    void* ptr;
    if ((ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_NOCORE, -1, 0)) == MAP_FAILED) {
        ptr = nullptr;
    }
    return ptr;
}

#elif defined(HAVE_POSIX_MEMALIGN)                          // POSIX_MEMALIGN

static MALLOC_ATTRIBUTE void* aligned_alloc_impl(size_t size)
{
    static size_t page_size = get_page_size();

    void* ptr;
    if (posix_memalign(&ptr, page_size, size) != 0) {
        ptr = nullptr;
    }
    return ptr;
}

#elif defined(OS_WINAPI_DESKTOP)                           // WINDOWS

static MALLOC_ATTRIBUTE void* aligned_alloc_impl(size_t size)
{
    return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

#endif


static MALLOC_ATTRIBUTE void* aligned_alloc(size_t size)
{
    return aligned_alloc_impl(size);
}


#if defined(HAVE_ALIGNED_ALLOC)                     // HAVE ALIGNED ALLOC

static MALLOC_ATTRIBUTE void* secure_malloc_impl(size_t size)
{
    void* user_ptr;
    uint8_t* base_ptr;
    uint8_t* canary_ptr;
    uint8_t* unprotected_ptr;
    size_t size_with_canary;
    size_t total_size;
    size_t unprotected_size;
    static size_t page_size = get_page_size();
    static const uint8_t* canary = get_canary();

    // runtime checks
    if (size >= SIZE_MAX - page_size * 4U) {
        errno = ENOMEM;
        return nullptr;
    }

    // get parameters
    size_with_canary = CANARY_SIZE + size;
    unprotected_size = page_round(size_with_canary);
    total_size = page_size + page_size + unprotected_size + page_size;

    //allocate data
    if ((base_ptr = (uint8_t*) aligned_alloc_impl(total_size)) == nullptr) {
        return nullptr;
    }
    // protect pointer
    unprotected_ptr = base_ptr + page_size * 2U;
    secure_mprotect_noaccess_impl(base_ptr + page_size, page_size);

    // add copy page protection
    secure_memcpy_page_protection(unprotected_ptr + unprotected_size, canary, CANARY_SIZE);

    // lock access to extra pages
    secure_mprotect_noaccess_impl(unprotected_ptr + unprotected_size, page_size);
    secure_mlock(unprotected_ptr, unprotected_size);
    canary_ptr = unprotected_ptr + page_round(size_with_canary) - size_with_canary;
    user_ptr = canary_ptr + CANARY_SIZE;
    memcpy(canary_ptr, canary, CANARY_SIZE);
    memcpy(base_ptr, &unprotected_size, sizeof(size_t));

    // set base_ptr page to be read-only
    secure_mprotect_readonly_impl(base_ptr, page_size);
    assert(unprotected_ptr_from_user_ptr(user_ptr) == unprotected_ptr);

    return user_ptr;
}

#else                                               // NO ALIGNED ALLOC

static MALLOC_ATTRIBUTE void* secure_malloc_impl(size_t size)
{
    if (size > 0) {
        return malloc(size);
    }
    return malloc(1);
}

#endif


void*  MALLOC_ATTRIBUTE secure_malloc(size_t size)
{
    void* ptr = secure_malloc_impl(size);
    if (size == 0) {
        return ptr;
    } else if (ptr == nullptr) {
        throw std::bad_alloc();
    }
    secure_memset(ptr, (int) GARBAGE_VALUE, size);

    return ptr;
}

// CALLOC
// ------

void* secure_calloc(size_t num, size_t size)
{
    void* ptr = secure_malloc(num * size);
    secure_zero(ptr, num * size);

    return ptr;
}

// FREE
// ----

#if defined(MAP_ANON) && defined(HAVE_MMAP)                 // MMAP

static void aligned_free_impl(void* ptr, size_t size)
{
    munmap(ptr, size);
}

#elif defined(HAVE_POSIX_MEMALIGN)                          // POSIX_MEMALIGN

static void aligned_free_impl(void* ptr, size_t size)
{
    free(ptr);
}

#elif defined(OS_WINAPI_DESKTOP)                            // WINDOWS

static void aligned_free_impl(void* ptr, size_t size)
{
    VirtualFree(ptr, 0U, MEM_RELEASE);
}

#endif


#if defined(HAVE_ALIGNED_ALLOC)                     // HAVE ALIGNED ALLOC


static void aligned_free(uint8_t* ptr, size_t size)
{
    aligned_free_impl(ptr, size);
}


static void secure_free_impl(void* ptr)
{
    uint8_t* base_ptr;
    uint8_t* canary_ptr;
    uint8_t* unprotected_ptr;
    size_t total_size;
    size_t unprotected_size;
    static size_t page_size = get_page_size();
    static const uint8_t* canary = get_canary();

    if (ptr == nullptr) {
        return;
    }

    canary_ptr = ((uint8_t*) ptr) - CANARY_SIZE;
    unprotected_ptr = unprotected_ptr_from_user_ptr(ptr);
    base_ptr = unprotected_ptr - page_size * 2U;
    memcpy(&unprotected_size, base_ptr, sizeof(size_t));
    total_size = page_size + page_size + unprotected_size + page_size;
    secure_mprotect_readwrite_impl(base_ptr, total_size);
    if (secure_memcmp(canary_ptr, canary, CANARY_SIZE) != 0) {
        throw std::out_of_range("Access out of bounds memory.");
    }
    if (secure_memcmp_page_protection(unprotected_ptr + unprotected_size, canary, CANARY_SIZE) != 0) {
        throw std::out_of_range("Access out of bounds memory.");
    }
    secure_munlock(unprotected_ptr, unprotected_size);
    aligned_free(base_ptr, total_size);
}

#else                                               // NO ALIGNED ALLOC

static void secure_free_impl(void* ptr)
{
    free(ptr);
}

#endif

void secure_free(void* ptr)
{
    secure_free_impl(ptr);
}

// WEAK
// ----

// force asm("") to prevent volatility and prevent optimizations
WEAK_ASM(secure_memset);
WEAK_ASM(secure_memcpy);
WEAK_ASM(secure_memmove);
WEAK_ASM(secure_memcmp);
WEAK_ASM(secure_zero);

PYCPP_END_NAMESPACE
