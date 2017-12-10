//  :copyright: (c) 2015-2017 MapBox.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.

#include <pycpp/stl/variant.h>

#if !defined(HAVE_CPP17)             // !HAVE_CPP17

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

const char* bad_variant_access::what() const noexcept
{
    return "bad_variant_access";
}

PYCPP_END_NAMESPACE

#endif                              // !HAVE_CPP17
