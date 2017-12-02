//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/char.h>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


lexical_char_formatter::lexical_char_formatter(char c)
{
    buffer_[0] = c;
    buffer_[1] = '\0';
}


lexical_char_formatter::lexical_char_formatter(unsigned char c)
{
    buffer_[0] = static_cast<char>(c);
    buffer_[1] = '\0';
}


size_t lexical_char_formatter::size() const
{
    return 1;
}


size_t lexical_char_formatter::length() const
{
    return size();
}


const char * lexical_char_formatter::data() const
{
    return c_str();
}


const char * lexical_char_formatter::c_str() const
{
    return buffer_;
}


string_view lexical_char_formatter::string() const
{
    return string_view(data(), size());
}


lexical_char_formatter::operator string_view() const
{
    return string();
}


lexical_char_extractor::lexical_char_extractor(const string_view &string)
{
    if (string.size() != 1) {
        throw std::runtime_error("String is not a single character.");
    } else {
        c = string.front();
    }
}


lexical_char_extractor::operator char() const
{
    return c;
}


lexical_char_extractor::operator unsigned char() const
{
    return static_cast<unsigned char>(c);
}

PYCPP_END_NAMESPACE
