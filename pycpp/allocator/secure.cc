//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/allocator/secure.h>
#include <pycpp/secure/stdlib.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


void* secure_allocator_base::allocate(size_t n, size_t size, const void*)
{
    void* ptr = secure_calloc(n, size);
    if (size > 0 && ptr == nullptr) {
        throw bad_alloc();
    }
    return ptr;
}


void secure_allocator_base::deallocate(void* p, size_t)
{
    secure_free(p);
}


void secure_allocator_base::zero(void* p, size_t size)
{
    secure_zero(p, size);
}

PYCPP_END_NAMESPACE
