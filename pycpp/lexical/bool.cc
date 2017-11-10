//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/bool.h>
#include <cstring>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static const char TRUE_STRING[] = "true";
static const char FALSE_STRING[] = "false";

// OBJECTS
// -------

lexical_bool_formatter::lexical_bool_formatter()
{
    strncpy(buffer_, FALSE_STRING, sizeof(FALSE_STRING));
    size_ = 5;
}


lexical_bool_formatter::lexical_bool_formatter(bool value)
{
    if (value) {
        strncpy(buffer_, TRUE_STRING, sizeof(TRUE_STRING));
        size_ = 4;
    } else {
        strncpy(buffer_, FALSE_STRING, sizeof(FALSE_STRING));
        size_ = 5;
    }
}


size_t lexical_bool_formatter::size() const
{
    return size_;
}


size_t lexical_bool_formatter::length() const
{
    return size();
}


const char* lexical_bool_formatter::data() const
{
    return c_str();
}


const char* lexical_bool_formatter::c_str() const
{
    return buffer_;
}


string_view lexical_bool_formatter::string() const
{
    return string_view(data(), size());
}


string_view lexical_bool_formatter::escape() const
{
    return string();
}


string_view lexical_bool_formatter::jsonify() const
{
    return string();
}


lexical_bool_formatter::operator string_view() const
{
    return string();
}


lexical_bool_extractor::lexical_bool_extractor(const string_view &string)
{
    if (string == TRUE_STRING) {
        data_ = true;
    } else if (string == FALSE_STRING) {
        data_ = false;
    } else {
        throw std::runtime_error("String is not boolean.");
    }
}


bool lexical_bool_extractor::value() const
{
    // WARNING: This is used as a patch for MSVC, where functional-style
    // type-casts are not allowed with std::nullptr_t.
    return data_;
}


lexical_bool_extractor::operator bool() const
{
    return data_;
}

PYCPP_END_NAMESPACE
