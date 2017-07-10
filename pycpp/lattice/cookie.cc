//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/url.h>
#include <pycpp/lattice/cookie.h>
#include <sstream>


PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/** \brief Special case of type 1 cookies, with leading and trailing quotes.
 */
bool encode_version_one_cookie(const std::string &cookie)
{
    if (!cookie.empty()) {
        return (cookie.front() == '"' && cookie.back() == '"');
    }
    return false;
}


std::string cookies_t::encode() const
{
    std::stringstream stream;
    for (const auto &item: *this) {
        stream << url_encode(item.first) << "=";
        if (encode_version_one_cookie(item.second)) {
            stream << item.second;
        } else {
            stream << url_encode(item.second);
        }
        stream << "; ";
    }

    return stream.str();
}


cookies_t::operator bool() const
{
    return !empty();
}

PYCPP_END_NAMESPACE
