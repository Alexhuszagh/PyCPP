//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/atof.h>
#include <pycpp/lexical/float.h>
#include <pycpp/lexical/ftoa.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

lexical_float_formatter::lexical_float_formatter(float value)
{
    f32toa(value, buffer_, last_, 10);
}


lexical_float_formatter::lexical_float_formatter(double value)
{
    f64toa(value, buffer_, last_, 10);
}


size_t lexical_float_formatter::size() const
{
    return last_ - buffer_;
}


size_t lexical_float_formatter::length() const
{
    return size();
}


const char* lexical_float_formatter::data() const
{
    return c_str();
}


const char* lexical_float_formatter::c_str() const
{
    return buffer_;
}


string_view lexical_float_formatter::string() const
{
    return string_view(data(), size());
}


string_view lexical_float_formatter::escape() const
{
    return string();
}


string_view lexical_float_formatter::jsonify() const
{
    return string();
}


lexical_float_formatter::operator string_view() const
{
    return string();
}


lexical_float_extractor::lexical_float_extractor(const string_view& string)
{
    data_ = atof64(string, 10);
}


lexical_float_extractor::operator float() const
{
    return static_cast<float>(data_);
}


lexical_float_extractor::operator double() const
{
    return static_cast<double>(data_);
}

PYCPP_END_NAMESPACE
