//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lattice/cookie.h>
#include <pycpp/string/url.h>


PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Special case of type 1 cookies, with leading and trailing quotes.
 */
bool encode_version_one_cookie(const std::string &cookie)
{
    if (!cookie.empty()) {
        return (cookie.front() == '"' && cookie.back() == '"');
    }
    return false;
}


cookies_t::cookies_t(initializer_list<typename cookies_t::value_type> list)
{
    for (auto it = list.begin(); it != list.end(); ++it) {
        emplace(*it);
    }
}


std::string cookies_t::encode() const
{
    std::string string;
    for (const auto &item: *this) {
        string += url_encode(item.first) + "=";
        if (encode_version_one_cookie(item.second)) {
            string += item.second;
        } else {
            string += url_encode(item.second);
        }
        string += "; ";
    }

    return string;
}


cookies_t::operator bool() const
{
    return !empty();
}

PYCPP_END_NAMESPACE
