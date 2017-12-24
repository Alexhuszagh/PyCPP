//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/char.h>
#include <pycpp/stl/stdexcept.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


lexical_char_formatter::lexical_char_formatter(char c) noexcept
{
    buffer_[0] = c;
    buffer_[1] = '\0';
}


lexical_char_formatter::lexical_char_formatter(unsigned char c) noexcept
{
    buffer_[0] = static_cast<char>(c);
    buffer_[1] = '\0';
}


size_t lexical_char_formatter::size() const noexcept
{
    return 1;
}


size_t lexical_char_formatter::length() const noexcept
{
    return size();
}


const char * lexical_char_formatter::data() const noexcept
{
    return c_str();
}


const char * lexical_char_formatter::c_str() const noexcept
{
    return buffer_;
}


string_view lexical_char_formatter::string() const noexcept
{
    return string_view(data(), size());
}


lexical_char_formatter::operator string_view() const noexcept
{
    return string();
}


lexical_char_extractor::lexical_char_extractor(const string_view &string)
{
    if (string.size() != 1) {
        throw runtime_error("String is not a single character.");
    } else {
        c = string.front();
    }
}


lexical_char_extractor::operator char() const noexcept
{
    return c;
}


lexical_char_extractor::operator unsigned char() const noexcept
{
    return static_cast<unsigned char>(c);
}

PYCPP_END_NAMESPACE
