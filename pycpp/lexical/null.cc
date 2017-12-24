//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/format.h>
#include <pycpp/lexical/null.h>
#include <pycpp/stl/stdexcept.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

lexical_null_formatter::lexical_null_formatter(nullptr_t nullp) noexcept
{}


size_t lexical_null_formatter::size() const noexcept
{
    return NULL_STRING.size();
}


size_t lexical_null_formatter::length() const noexcept
{
    return size();
}


const char* lexical_null_formatter::data() const noexcept
{
    return c_str();
}


const char* lexical_null_formatter::c_str() const noexcept
{
    return NULL_STRING.data();
}


string_view lexical_null_formatter::string() const noexcept
{
    return string_view(data(), size());
}


lexical_null_formatter::operator string_view() const noexcept
{
    return string();
}


lexical_null_extractor::lexical_null_extractor(const string_view &string)
{
    if (string != string_view(NULL_STRING)) {
        throw runtime_error("String is not null.");
    }
}


nullptr_t lexical_null_extractor::value() const noexcept
{
    // WARNING: This is used as a patch for MSVC, where functional-style
    // type-casts are not allowed with nullptr_t.
    return nullptr;
}


lexical_null_extractor::operator nullptr_t() const noexcept
{
    return nullptr;
}

PYCPP_END_NAMESPACE
