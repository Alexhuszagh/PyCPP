//  :copyright: (c) 2016 Denilson das Mercês Amorim.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.

#include <pycpp/collections/any.h>

#if !defined(HAVE_CPP17)             // !HAVE_CPP17

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

const char* bad_any_cast::what() const noexcept
{
    return "bad any cast";
}

PYCPP_END_NAMESPACE

#endif                              // !HAVE_CPP17
