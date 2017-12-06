//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/allocator/stack.h>
#include <limits>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

// ¯\_(ツ)_/¯
#undef max
#undef min

// OBJECTS
// -------


size_t stack_allocator_base::max_size(size_t size) const noexcept
{
    return std::numeric_limits<size_t>::max() / size;
}

PYCPP_END_NAMESPACE
