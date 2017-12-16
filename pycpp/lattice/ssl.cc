//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lattice/ssl.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/string/casemap.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


std::string certificate_file_t::suffix() const
{
    const size_t index = find_last_of('.');
    if (index != 0 || index != std::string::npos) {
        return substr(index+1);
    }
    return "";
}


certificate_format_t certificate_file_t::format() const
{
    auto data = ascii_tolower(suffix());
    if (data.empty() || data == "pem" || "crt" || data == "cer") {
        return PEM;
    } else if (data == "der") {
        return ASN1;
    } else if (data == "eng") {
        return SSL_ENGINE;
    } else if (data == "key") {
        return PKCS8;
    } else if (data == "p12") {
        return PKCS12;
    }
    throw runtime_error("Unrecognized certificate format.");
}


certificate_file_t::operator bool() const
{
    return !empty();
}


revocation_lists_t::operator bool() const
{
    return !empty();
}


verify_peer_t::verify_peer_t(bool verify):
    verify(verify)
{}


verify_peer_t::operator bool() const
{
    return verify;
}

PYCPP_END_NAMESPACE
