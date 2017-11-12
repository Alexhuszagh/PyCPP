//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/format.h>
#include <pycpp/lexical/null.h>
#include <cstring>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


lexical_null_formatter::lexical_null_formatter()
{}


lexical_null_formatter::lexical_null_formatter(const std::nullptr_t nullp)
{}

size_t lexical_null_formatter::size() const
{
    return NULL_STRING.size();
}


size_t lexical_null_formatter::length() const
{
    return size();
}


const char* lexical_null_formatter::data() const
{
    return c_str();
}


const char* lexical_null_formatter::c_str() const
{
    return NULL_STRING.c_str();
}


string_view lexical_null_formatter::string() const
{
    return string_view(data(), size());
}


string_view lexical_null_formatter::escape() const
{
    return string();
}


string_view lexical_null_formatter::jsonify() const
{
    return string();
}


lexical_null_formatter::operator string_view() const
{
    return string();
}


lexical_null_extractor::lexical_null_extractor(const string_view &string)
{
    if (string != NULL_STRING) {
        throw std::runtime_error("String is not null.");
    }
}


std::nullptr_t lexical_null_extractor::value() const
{
    // WARNING: This is used as a patch for MSVC, where functional-style
    // type-casts are not allowed with std::nullptr_t.
    return nullptr;
}


lexical_null_extractor::operator std::nullptr_t() const
{
    return nullptr;
}

PYCPP_END_NAMESPACE
