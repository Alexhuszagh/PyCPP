//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/bool.h>
#include <pycpp/lexical/format.h>
#include <pycpp/stl/stdexcept.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

lexical_bool_formatter::lexical_bool_formatter() noexcept
{
    string_ = &FALSE_STRING;
}


lexical_bool_formatter::lexical_bool_formatter(bool value) noexcept
{
    string_ = value ? &TRUE_STRING : &FALSE_STRING;
}


size_t lexical_bool_formatter::size() const noexcept
{
    return string_->size();
}


size_t lexical_bool_formatter::length() const noexcept
{
    return size();
}


const char* lexical_bool_formatter::data() const noexcept
{
    return c_str();
}


const char* lexical_bool_formatter::c_str() const noexcept
{
    return string_->c_str();
}


string_view lexical_bool_formatter::str() const noexcept
{
    return string_view(data(), size());
}


lexical_bool_formatter::operator string_view() const noexcept
{
    return str();
}


lexical_bool_extractor::lexical_bool_extractor(const string_view &string)
{
    if (string == string_view(TRUE_STRING)) {
        data_ = true;
    } else if (string == string_view(FALSE_STRING)) {
        data_ = false;
    } else {
        throw runtime_error("String is not boolean.");
    }
}


bool lexical_bool_extractor::value() const noexcept
{
    // WARNING: This is used as a patch for MSVC, where functional-style
    // type-casts are not allowed with nullptr_t.
    return data_;
}


lexical_bool_extractor::operator bool() const noexcept
{
    return data_;
}

PYCPP_END_NAMESPACE
