//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/allocator/standard.h>
#include <cstdlib>
#include <limits>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

// ¯\_(ツ)_/¯
#undef max
#undef min

// OBJECTS
// -------


void* standard_allocator_base::allocate(size_t n, size_t size, const void*)
{
    if (n > max_size(size)) {
        throw std::runtime_error("allocator<T>::allocate(size_t n) exceeds max size.");
    }

    return malloc(n * size);
}


void standard_allocator_base::deallocate(void* p, size_t)
{
    free(p);
}


size_t standard_allocator_base::max_size(size_t size) const noexcept
{
    return std::numeric_limits<size_t>::max() / size;
}

PYCPP_END_NAMESPACE
