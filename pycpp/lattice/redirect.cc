//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lattice/redirect.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


redirects_t::redirects_t(long count):
    count(count)
{}


redirects_t & redirects_t::operator++()
{
    ++count;
    return *this;
}


redirects_t redirects_t::operator++(int)
{
    redirects_t copy(*this);
    operator++();
    return copy;
}


redirects_t & redirects_t::operator--()
{
    --count;
    return *this;
}


redirects_t redirects_t::operator--(int)
{
    redirects_t copy(*this);
    operator--();
    return copy;
}


redirects_t::operator bool() const
{
    return bool(count);
}

PYCPP_END_NAMESPACE

#include <warnings/pop.h>
