//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/allocator/crt.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


void* crt_allocator_base::allocate(size_t n, size_t size, const void*)
{
    return safe_malloc(n * size);
}


void crt_allocator_base::deallocate(void* p, size_t)
{
    safe_free(p);
}

PYCPP_END_NAMESPACE
