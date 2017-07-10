//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/compiler.h>
#include <pycpp/lattice/redirect.h>

#if defined(HAVE_MSVC)
#   pragma warning(push)
#   pragma warning(disable:4800)
#endif

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


redirects_t::redirects_t(const long count):
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

#if defined(HAVE_MSVC)
#   pragma warning(pop)
#endif
