//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/allocator/standard.h>
#include <pycpp/stl/new.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


void* standard_allocator_base::allocate(size_t n, size_t size, const void*)
{
    return operator new(n * size);
}


void standard_allocator_base::deallocate(void* p, size_t)
{
    operator delete(p);
}

PYCPP_END_NAMESPACE
