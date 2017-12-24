//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/atof.h>
#include <pycpp/lexical/float.h>
#include <pycpp/lexical/ftoa.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

lexical_float_formatter::lexical_float_formatter(float value) noexcept
{
    f32toa(value, buffer_, last_, 10);
}


lexical_float_formatter::lexical_float_formatter(double value) noexcept
{
    f64toa(value, buffer_, last_, 10);
}


lexical_float_formatter::lexical_float_formatter(const lexical_float_formatter& rhs) noexcept
{
    memcpy(buffer_, rhs.buffer_, MAX_FLOAT_SIZE);
    last_ = buffer_ + rhs.size();
}


lexical_float_formatter& lexical_float_formatter::operator=(const lexical_float_formatter& rhs) noexcept
{
    memcpy(buffer_, rhs.buffer_, MAX_FLOAT_SIZE);
    last_ = buffer_ + rhs.size();
    return *this;
}


size_t lexical_float_formatter::size() const noexcept
{
    return last_ - buffer_;
}


size_t lexical_float_formatter::length() const noexcept
{
    return size();
}


const char* lexical_float_formatter::data() const noexcept
{
    return c_str();
}


const char* lexical_float_formatter::c_str() const noexcept
{
    return buffer_;
}


string_view lexical_float_formatter::string() const noexcept
{
    return string_view(data(), size());
}


lexical_float_formatter::operator string_view() const noexcept
{
    return string();
}


lexical_float_extractor::lexical_float_extractor(const string_view& string) noexcept
{
    data_ = atof64(string, 10);
}


lexical_float_extractor::operator float() const noexcept
{
    return static_cast<float>(data_);
}


lexical_float_extractor::operator double() const noexcept
{
    return static_cast<double>(data_);
}

PYCPP_END_NAMESPACE
