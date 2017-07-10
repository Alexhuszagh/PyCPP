//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lattice/proxy.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


proxy_t::operator bool() const
{
    return !empty();
}

PYCPP_END_NAMESPACE
