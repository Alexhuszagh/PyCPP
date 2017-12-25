//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lattice/auth.h>
#include <pycpp/stl/utility.h>


PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


authentication_t::authentication_t(const char* username, const char* password):
    authentication_t(string_wrapper(username), string_wrapper(password))
{}


authentication_t::authentication_t(string &&username, string &&password):
    username(forward<string>(username)),
    password(forward<string>(password))
{}


authentication_t::authentication_t(const string_wrapper& username, const string_wrapper& password):
    username(username),
    password(password)
{}


const string authentication_t::str() const noexcept
{
    return username + ":" + password;
}


authentication_t::operator bool() const
{
    return !(username.empty() || password.empty());
}

PYCPP_END_NAMESPACE
