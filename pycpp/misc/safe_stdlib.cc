//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/misc/safe_stdlib.h>
#include <pycpp/stl/new.h>
#include <pycpp/stl/stdexcept.h>
#include <stdlib.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


void* safe_malloc(size_t size)
{
    void* ptr = malloc(size);
    if (size > 0 && ptr == nullptr) {
        throw bad_alloc();
    }
    return ptr;
}


void* safe_realloc(void* ptr, size_t size)
{
    void* output = realloc(ptr, size);
    if (size > 0 && output == nullptr) {
        free(ptr);
        throw bad_alloc();
    }
    return output;
}


void* safe_calloc(size_t num, size_t size)
{
    void* ptr = calloc(num, size);
    if (size > 0 && ptr == nullptr) {
        throw bad_alloc();
    }
    return ptr;
}


void safe_free(void* ptr) noexcept
{
    free(ptr);
}

PYCPP_END_NAMESPACE
