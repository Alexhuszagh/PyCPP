//  :copyright: (c) 2003 Davis E. King (davis@dlib.net)
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.

#include <url.h>

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
std::string url_encode(const std::string &string)
{
    std::string result;
    result.reserve(3 * string.size());

    for (auto it = string.cbegin(); it != string.cend(); ++it)
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
std::string url_decode(const std::string &string)
{
    std::string result;
    result.reserve(string.size());

    std::string::size_type i;
    for (i = 0; i < string.size(); ++i)
    {
        if (string[i] == '+') {
            result += ' ';
        } else if (string[i] == '%' && string.size() > i+2) {
            const unsigned char ch1 = from_hex(string[i+1]);
            const unsigned char ch2 = from_hex(string[i+2]);
            const unsigned char ch = (ch1 << 4) | ch2;
            result += ch;
            i += 2;
        } else {
            result += string[i];
        }
    }

    result.shrink_to_fit();
    return result;
}
