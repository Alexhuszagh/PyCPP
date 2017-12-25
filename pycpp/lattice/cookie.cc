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
bool encode_version_one_cookie(const string &cookie)
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


string cookies_t::encode() const
{
    string str;
    for (const auto &item: *this) {
        str += url_encode(item.first) + "=";
        if (encode_version_one_cookie(item.second)) {
            str += item.second;
        } else {
            str += url_encode(item.second);
        }
        str += "; ";
    }

    return str;
}


cookies_t::operator bool() const
{
    return !empty();
}

PYCPP_END_NAMESPACE
