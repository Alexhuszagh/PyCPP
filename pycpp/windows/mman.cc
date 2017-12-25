//  :copyright: (c) 2012-2017 Steven Lee.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/preprocessor/os.h>

#if defined(OS_WINDOWS)         // WINDOWS
#include <pycpp/windows/mman.h>
#include <windows.h>
#include <errno.h>
#include <io.h>

// MACROS
// ------

#ifndef FILE_MAP_EXECUTE
#   define FILE_MAP_EXECUTE    0x0020
#endif /* FILE_MAP_EXECUTE */

// HELPERS
// -------

static int map_mman_error(const DWORD err, const int deferr) noexcept
{
    if (err == 0) {
        return 0;
    }
    return err;
}

static DWORD map_mmap_prot_page(const int prot) noexcept
{
    DWORD protect = 0;

    if (prot == PROT_NONE) {
        return protect;
    }

    if ((prot & PROT_EXEC) != 0) {
        protect = ((prot & PROT_WRITE) != 0) ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_READ;
    } else {
        protect = ((prot & PROT_WRITE) != 0) ? PAGE_READWRITE : PAGE_READONLY;
    }

    return protect;
}

static DWORD map_mmap_prot_file(const int prot) noexcept
{
    DWORD desired_access = 0;

    if (prot == PROT_NONE)
        return desired_access;

    if ((prot & PROT_READ) != 0)
        desired_access |= FILE_MAP_READ;
    if ((prot & PROT_WRITE) != 0)
        desired_access |= FILE_MAP_WRITE;
    if ((prot & PROT_EXEC) != 0)
        desired_access |= FILE_MAP_EXECUTE;

    return desired_access;
}


// FUNCTIONS
// ---------


void* mmap(void* addr, size_t len, int prot, int flags, int fildes, offset_t off) noexcept
{
    HANDLE fm, h;

    void* map = MAP_FAILED;

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4293)
#endif

    const DWORD offset_low = (sizeof(offset_t) <= sizeof(DWORD)) ?
                              (DWORD)off : (DWORD)(off & 0xFFFFFFFFL);
    const DWORD offset_high = (sizeof(offset_t) <= sizeof(DWORD)) ?
                               (DWORD)0 : (DWORD)((off >> 32) & 0xFFFFFFFFL);
    const DWORD protect = map_mmap_prot_page(prot);
    const DWORD desired_access = map_mmap_prot_file(prot);

    const offset_t max_size = off + (offset_t)len;

    const DWORD dwMaxSizeLow = (sizeof(offset_t) <= sizeof(DWORD)) ?
                                (DWORD)max_size : (DWORD)(max_size & 0xFFFFFFFFL);
    const DWORD dwMaxSizeHigh = (sizeof(offset_t) <= sizeof(DWORD)) ?
                                 (DWORD)0 : (DWORD)((max_size >> 32) & 0xFFFFFFFFL);

#ifdef _MSC_VER
#   pragma warning(pop)
#endif

    errno = 0;

    if (len == 0
        /* Unsupported flag combinations */
        || (flags & MAP_FIXED) != 0
        /* Usupported protection combinations */
        || prot == PROT_EXEC)
    {
        errno = EINVAL;
        return MAP_FAILED;
    }

    h = ((flags & MAP_ANONYMOUS) == 0) ? (HANDLE)_get_osfhandle(fildes) : INVALID_HANDLE_VALUE;
    if ((flags & MAP_ANONYMOUS) == 0 && h == INVALID_HANDLE_VALUE) {
        errno = EBADF;
        return MAP_FAILED;
    }

    fm = CreateFileMapping(h, NULL, protect, dwMaxSizeHigh, dwMaxSizeLow, NULL);
    if (fm == NULL) {
        errno = map_mman_error(GetLastError(), EPERM);
        return MAP_FAILED;
    }

    map = MapViewOfFile(fm, desired_access, offset_high, offset_low, len);
    CloseHandle(fm);
    if (map == NULL) {
        errno = map_mman_error(GetLastError(), EPERM);
        return MAP_FAILED;
    }

    return map;
}

int munmap(void* addr, size_t len) noexcept
{
    if (UnmapViewOfFile(addr)) {
        return 0;
    }

    errno = map_mman_error(GetLastError(), EPERM);

    return -1;
}

int mprotect(void* addr, size_t len, int prot) noexcept
{
    DWORD newProtect = map_mmap_prot_page(prot);
    DWORD oldProtect = 0;

    if (VirtualProtect(addr, len, newProtect, &oldProtect)) {
        return 0;
    }

    errno = map_mman_error(GetLastError(), EPERM);

    return -1;
}

int msync(void* addr, size_t len, int flags) noexcept
{
    if (FlushViewOfFile(addr, len)) {
        return 0;
    }

    errno = map_mman_error(GetLastError(), EPERM);

    return -1;
}

int mlock(const void* addr, size_t len) noexcept
{
    if (VirtualLock((LPVOID)addr, len)) {
        return 0;
    }

    errno = map_mman_error(GetLastError(), EPERM);

    return -1;
}

int munlock(const void* addr, size_t len) noexcept
{
    if (VirtualUnlock((LPVOID)addr, len)) {
        return 0;
    }

    errno = map_mman_error(GetLastError(), EPERM);

    return -1;
}

#endif                          // WINDOWS
