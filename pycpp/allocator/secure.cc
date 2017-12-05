//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/allocator/secure.h>
#include <pycpp/secure/stdlib.h>
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


void* secure_allocator_base::allocate(size_t n, size_t size, const void*)
{
    if (n > max_size(size)) {
        throw std::runtime_error("allocator<T>::allocate(size_t n) exceeds max size.");
    }

    return secure_calloc(n, size);
}


void secure_allocator_base::deallocate(void* p, size_t)
{
    secure_free(p);
}


void secure_allocator_base::zero(void* p, size_t size)
{
    secure_zero(p, size);
}


size_t secure_allocator_base::max_size(size_t size) const noexcept
{
    return std::numeric_limits<size_t>::max() / size;
}


PYCPP_END_NAMESPACE
