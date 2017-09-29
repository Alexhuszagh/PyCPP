//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/csv/punct.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

char csvpunct_impl::delimiter() const
{
    return do_delimiter();
}


char csvpunct_impl::quote() const
{
    return do_quote();
}


char csvpunct_impl::escape() const
{
    return do_escape();
}


char csvpunct_impl::do_delimiter() const
{
    return ',';
}


char csvpunct_impl::do_quote() const
{
    return '"';
}


char csvpunct_impl::do_escape() const
{
    return '\\';
}


char tabpunct::do_delimiter() const
{
    return '\t';
}


char pipepunct::do_delimiter() const
{
    return '|';
}

PYCPP_END_NAMESPACE
