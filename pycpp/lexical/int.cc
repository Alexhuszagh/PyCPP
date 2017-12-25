//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lexical/atoi.h>
#include <pycpp/lexical/int.h>
#include <pycpp/lexical/itoa.h>
#include <pycpp/stl/limits.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

lexical_int_formatter::lexical_int_formatter(short value) noexcept
{
    i16toa(value, buffer_, last_, 10);
}


lexical_int_formatter::lexical_int_formatter(unsigned short value) noexcept
{
    u16toa(value, buffer_, last_, 10);
}


lexical_int_formatter::lexical_int_formatter(int value) noexcept
{
    i32toa(value, buffer_, last_, 10);
}


lexical_int_formatter::lexical_int_formatter(unsigned int value) noexcept
{
    u32toa(value, buffer_, last_, 10);
}


lexical_int_formatter::lexical_int_formatter(long value) noexcept
{
    #if !defined(_WIN32) && SYSTEM_ARCHITECTURE == 64
        // longs are 64-bits on all 64-bit systems except Windows
        i64toa(value, buffer_, last_, 10);
    #else
        i32toa(value, buffer_, last_, 10);
    #endif

    *last_ = '\0';
}


lexical_int_formatter::lexical_int_formatter(unsigned long value) noexcept
{
    #if !defined(_WIN32) && SYSTEM_ARCHITECTURE == 64
        // longs are 64-bits on all 64-bit systems except Windows
        u64toa(value, buffer_, last_, 10);
    #else
        u32toa(value, buffer_, last_, 10);
    #endif

    *last_ = '\0';
}


lexical_int_formatter::lexical_int_formatter(long long value) noexcept
{
    i64toa(value, buffer_, last_, 10);
}


lexical_int_formatter::lexical_int_formatter(unsigned long long value) noexcept
{
    u64toa(value, buffer_, last_, 10);
}


lexical_int_formatter::lexical_int_formatter(const lexical_int_formatter& rhs) noexcept
{
    memcpy(buffer_, rhs.buffer_, MAX_INTEGER_SIZE);
    last_ = buffer_ + rhs.size();
}


lexical_int_formatter& lexical_int_formatter::operator=(const lexical_int_formatter& rhs) noexcept
{
    memcpy(buffer_, rhs.buffer_, MAX_INTEGER_SIZE);
    last_ = buffer_ + rhs.size();
    return *this;
}


size_t lexical_int_formatter::size() const noexcept
{
    return last_ - buffer_;
}


size_t lexical_int_formatter::length() const noexcept
{
    return size();
}


const char* lexical_int_formatter::data() const noexcept
{
    return c_str();
}


const char* lexical_int_formatter::c_str() const noexcept
{
    return buffer_;
}


string_view lexical_int_formatter::str() const noexcept
{
    return string_view(data(), size());
}


lexical_int_formatter::operator string_view() const noexcept
{
    return str();
}


lexical_int_extractor::lexical_int_extractor(const string_view& string)noexcept
{
    if (string.empty()) {
        // 0 condition
        data_.i = 0;
        minus_ = false;
    } else if (string[0] == '-') {
        data_.i = atoi64(string, 10);
        minus_ = true;
    } else {
        data_.u = atou64(string, 10);
        minus_ = false;
    }
}


bool lexical_int_extractor::is_signed() const noexcept
{
    return minus_;
}


bool lexical_int_extractor::is_unsigned() const noexcept
{
    return !is_signed();
}


uint8_t lexical_int_extractor::bytes() const noexcept
{
    if (is_signed()) {
        // must be negative
        if (data_.i >= numeric_limits<int8_t>::min()) {
            return 1;
        } else if (data_.i >= numeric_limits<int16_t>::min()) {
            return 2;
        } else if (data_.i >= numeric_limits<int32_t>::min()) {
            return 4;
        } else {
            return 8;
        }
    } else {
        // must be positive
        if (data_.u <= numeric_limits<uint8_t>::max()) {
            return 1;
        } else if (data_.u <= numeric_limits<uint16_t>::max()) {
            return 2;
        } else if (data_.u <= numeric_limits<uint32_t>::max()) {
            return 4;
        } else {
            return 8;
        }
    }
}


lexical_int_extractor::operator int8_t() const noexcept
{
    if (is_signed()) {
        return static_cast<int8_t>(data_.i);
    } else {
        return static_cast<int8_t>(data_.u);
    }
}


lexical_int_extractor::operator uint8_t() const noexcept
{
    if (is_signed()) {
        return static_cast<uint8_t>(data_.i);
    } else {
        return static_cast<uint8_t>(data_.u);
    }
}


lexical_int_extractor::operator short() const noexcept
{
    if (is_signed()) {
        return static_cast<short>(data_.i);
    } else {
        return static_cast<short>(data_.u);
    }
}


lexical_int_extractor::operator unsigned short() const noexcept
{
    if (is_signed()) {
        return static_cast<unsigned short>(data_.i);
    } else {
        return static_cast<unsigned short>(data_.u);
    }
}


lexical_int_extractor::operator int() const noexcept
{
    if (is_signed()) {
        return static_cast<int>(data_.i);
    } else {
        return static_cast<int>(data_.u);
    }
}


lexical_int_extractor::operator unsigned int() const noexcept
{
    if (is_signed()) {
        return static_cast<unsigned int>(data_.i);
    } else {
        return static_cast<unsigned int>(data_.u);
    }
}


lexical_int_extractor::operator long() const noexcept
{
    if (is_signed()) {
        return static_cast<long>(data_.i);
    } else {
        return static_cast<long>(data_.u);
    }
}


lexical_int_extractor::operator unsigned long() const noexcept
{
    if (is_signed()) {
        return static_cast<unsigned long>(data_.i);
    } else {
        return static_cast<unsigned long>(data_.u);
    }
}


lexical_int_extractor::operator long long() const noexcept
{
    if (is_signed()) {
        return static_cast<long long>(data_.i);
    } else {
        return static_cast<long long>(data_.u);
    }
}


lexical_int_extractor::operator unsigned long long() const noexcept
{
    if (is_signed()) {
        return static_cast<unsigned long long>(data_.i);
    } else {
        return static_cast<unsigned long long>(data_.u);
    }
}

PYCPP_END_NAMESPACE
