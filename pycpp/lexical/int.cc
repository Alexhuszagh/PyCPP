//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/int.h>
#include <pycpp/lexical/itoa.h>
#include <cstring>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

lexical_int_formatter::lexical_int_formatter(short value)
{
    last_ = i32toa(value, buffer_);
    *last_ = '\0';
}


lexical_int_formatter::lexical_int_formatter(unsigned short value)
{
    last_ = u32toa(value, buffer_);
    *last_ = '\0';
}


lexical_int_formatter::lexical_int_formatter(int value)
{
    last_ = i32toa(value, buffer_);
    *last_ = '\0';
}


lexical_int_formatter::lexical_int_formatter(unsigned int value)
{
    last_ = u32toa(value, buffer_);
    *last_ = '\0';
}


lexical_int_formatter::lexical_int_formatter(long value)
{
    #if !defined(_WIN32) && SYSTEM_ARCHITECTURE == 64
        // longs are 64-bits on all 64-bit systems except Windows
        last_ = i64toa(value, buffer_);
    #else
        last_ = i32toa(value, buffer_);
    #endif

    *last_ = '\0';
}


lexical_int_formatter::lexical_int_formatter(unsigned long value)
{
    #if !defined(_WIN32) && SYSTEM_ARCHITECTURE == 64
        // longs are 64-bits on all 64-bit systems except Windows
        last_ = u64toa(value, buffer_);
    #else
        last_ = u32toa(value, buffer_);
    #endif

    *last_ = '\0';
}


lexical_int_formatter::lexical_int_formatter(long long value)
{
    last_ = i64toa(value, buffer_);
    *last_ = '\0';
}


lexical_int_formatter::lexical_int_formatter(unsigned long long value)
{
    last_ = u64toa(value, buffer_);
    *last_ = '\0';
}


size_t lexical_int_formatter::size() const
{
    return last_ - buffer_;
}


size_t lexical_int_formatter::length() const
{
    return size();
}


const char * lexical_int_formatter::data() const
{
    return c_str();
}


const char * lexical_int_formatter::c_str() const
{
    return buffer_;
}


string_view lexical_int_formatter::string() const
{
    return string_view(data(), size());
}


string_view lexical_int_formatter::escape() const
{
    return string();
}


string_view lexical_int_formatter::jsonify() const
{
    return string();
}


lexical_int_formatter::operator string_view() const
{
    return string();
}


#if 0
lexical_int_extractor::lexical_int_extractor(const string_view& string)
{
    // TODO: optimize this.
    data_ = std::strtoull(string.data(), nullptr, 10);
}


lexical_int_extractor::operator int8_t() const
{
    return static_cast<long long>(data_);
}


lexical_int_extractor::operator uint8_t() const
{
    return data_;
}


lexical_int_extractor::operator short() const
{
    return static_cast<long long>(data_);
}


lexical_int_extractor::operator unsigned short() const
{
    return data_;
}


lexical_int_extractor::operator int() const
{
    return static_cast<long long>(data_);
}


lexical_int_extractor::operator unsigned int() const
{
    return data_;
}


lexical_int_extractor::operator long() const
{
    return static_cast<long long>(data_);
}


lexical_int_extractor::operator unsigned long() const
{
    return data_;
}


lexical_int_extractor::operator long long() const
{
    return static_cast<long long>(data_);
}


lexical_int_extractor::operator unsigned long long() const
{
    return data_;
}
#endif

PYCPP_END_NAMESPACE
