//  :copyright: (c) 2003 Davis E. King (davis@dlib.net)
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.

#include <pycpp/string/url.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


/** Convert character to hex.
 */
unsigned char to_hex(unsigned char x)
{
    return x + (x > 9 ? ('A'-10) : '0');
}

/** Convert all ASCII character ranges back to bytes.
 */
unsigned char from_hex(unsigned char x)
{
    if (x <= '9' && x >= '0') {
        x -= '0';
    } else if (x <= 'f' && x >= 'a') {
        x -= 'a' - 10;
    } else if (x <= 'F' && x >= 'A') {
        x -= 'A' - 10;
    } else {
        x = 0;
    }
    return x;
}

// FUNCTIONS
// ---------


/** Pre-allocate upper bound of the encoded-string, that is, ~3x the
 *  size of the original string, and re-allocate string once.
 */
string url_encode(const string_wrapper& str)
{
    string result;
    result.reserve(3 * str.size());

    for (auto it = str.cbegin(); it != str.cend(); ++it)
    {
        if ((*it >= 'a' && *it <= 'z')||
            (*it >= 'A' && *it <= 'Z')||
            (*it >= '0' && *it <= '9'))
        {
            result += *it;
        } else if (*it == ' ') {
            result += '+';
        } else {
            result += '%';
            result += to_hex(*it >> 4);
            result += to_hex(*it % 16);
        }
    }

    result.shrink_to_fit();
    return result;
}


/** Pre-allocate upper bound of the encoded-string, that is, roughly the
 *  size of the original string, and re-allocate string once.
 */
string url_decode(const string_wrapper& str)
{
    string result;
    result.reserve(str.size());

    typename string::size_type i;
    for (i = 0; i < str.size(); ++i)
    {
        if (str[i] == '+') {
            result += ' ';
        } else if (str[i] == '%' && str.size() > i+2) {
            const unsigned char ch1 = from_hex(str[i+1]);
            const unsigned char ch2 = from_hex(str[i+2]);
            const unsigned char ch = (ch1 << 4) | ch2;
            result += ch;
            i += 2;
        } else {
            result += str[i];
        }
    }

    result.shrink_to_fit();
    return result;
}

PYCPP_END_NAMESPACE
