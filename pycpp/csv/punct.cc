//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/csv/punct.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

char csvpunct::delimiter() const
{
    return do_delimiter();
}


char csvpunct::quote() const
{
    return do_quote();
}


char csvpunct::escape() const
{
    return do_escape();
}


char csvpunct::do_delimiter() const
{
    return ',';
}


char csvpunct::do_quote() const
{
    return '"';
}


char csvpunct::do_escape() const
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
