//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lattice/header.h>
#include <pycpp/stl/algorithm.h>
#include <pycpp/string/casemap.h>
#include <cctype>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


bool lowercase_less::operator()(const std::string &lhs, const std::string &rhs) const noexcept
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), [](unsigned char l, unsigned char r) {
            return ascii_tolower(l) < ascii_tolower(r);
    });
}


std::string header_t::string() const
{
    std::string string;
    for (const auto &pair: *this) {
        if (pair.second.empty()) {
            string +=  pair.first + ";\r\n";
        } else {
            string += pair.first + ": " + pair.second + "\r\n";
        }
    }

    return string;
}


bool header_t::accept() const
{
    return find("accept") != end();
}


bool header_t::cookie() const
{
    return find("cookie") != end();
}


bool header_t::host() const
{
    return find("host") != end();
}


bool header_t::authorization() const
{
    return find("authorization") != end();
}


bool header_t::wwwauthenticate() const
{
    return find("www-authenticate") != end();
}


bool header_t::user_agent() const
{
    return find("user-agent") != end();
}


bool header_t::close_connection() const
{
    auto it = find("connection");
    if (it != end()) {
        return it->second == "close";
    }
    return false;
}


bool header_t::connection() const
{
    return find("connection") != end();
}


bool header_t::content_type() const
{
    return find("content-Type") != end();
}


std::ostream & operator<<(std::ostream &os, const header_t &header)
{
    os << header.string();
    return os;
}


header_t::operator bool() const
{
    return !empty();
}

PYCPP_END_NAMESPACE
