//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/hashlib.h>
#include <pycpp/random.h>
#include <pycpp/lattice/digest.h>
#include <pycpp/lattice/parameter.h>
#include <pycpp/lattice/url.h>
#include <pycpp/lexical/itoa.h>
#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/iomanip.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/string/casemap.h>
#include <pycpp/string/hex.h>
#include <pycpp/string/string.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

static string sha1_hex(const string_wrapper& view)
{
    auto hex = sha1_hash(view).hexdigest();
    return string(hex.data(), hex.size());
}


static string md5_hex(const string_wrapper& view)
{
    auto hex = md5_hash(view).hexdigest();
    return string(hex.data(), hex.size());
}

// OBJECTS
// -------


size_t lowercase_hash::operator()(const string& str) const
{
    return hash<string>()(ascii_tolower(str));
}


bool lowercase_equal_to::operator()(const string& lhs, const string& rhs) const
{
    return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin(), [](char l, char r) {
        return ascii_tolower(l) == ascii_tolower(r);
    });
}


quality_of_protection_t::quality_of_protection_t(const string_wrapper& qop)
{
    auto data = split(qop, ",");
    insert(begin(), data.begin(), data.end());
    for (auto &item: *this) {
        item = trim(item);
    }
}


bool quality_of_protection_t::auth() const
{
    return any_of(begin(), end(), [](const value_type& data) {
        return data == "auth";
    });
}


bool quality_of_protection_t::authint() const
{
    return any_of(begin(), end(), [](const value_type& data) {
        return data == "auth-int";
    });
}


quality_of_protection_t::operator bool() const
{
    return !empty();
}


digest_challenge_t::digest_challenge_t(const string_wrapper& str)
{
    auto data = quoted_split(str.substr(7), ',', '"', '\\');
    for (auto &value: data) {
        value = trim(value);
        const char *begin = value.data();
        const char *ptr = strchr(begin, '=');
        if (ptr) {
            string first(begin, ptr);
            string second(ptr+1);

            // remove quotes
            if (second.size() && second.front() == '"') {
                second.erase(0, 1);
            }
            if (second.size() && second.back() == '"') {
                second.erase(second.size() - 1);
            }
            emplace(first, second);
        }
    }
}


const string& digest_challenge_t::realm() const
{
    return at("realm");
}


const string& digest_challenge_t::nonce() const
{
    return at("nonce");
}


const string& digest_challenge_t::cnonce()
{
    if (client_nonce.empty()) {
        client_nonce = hex_i32(sysrandom(8));
    }
    return client_nonce;
}


string digest_challenge_t::nc() const
{
    string str = u64toa(nonce_counter, 8);
    str.insert(str.begin(), static_cast<size_t>(8 - str.size()), '0');

    return str;
}


digest_algorithm_t digest_challenge_t::algorithm() const
{
    auto it = find("algorithm");
    if (it == end()) {
        return md5_digest_algorithm;
    }

    string data = ascii_tolower(it->second);

    if (data == "md5") {
        return md5_digest_algorithm;
    } else if (data == "md5-sess") {
        return md5_sess_digest_algorithm;
    } else if (data == "sha") {
        return sha1_digest_algorithm;
    }

    throw runtime_error("Unknown hashing algorithm for digest authentication.");
}


quality_of_protection_t digest_challenge_t::qop() const
{
    auto it = find("qop");
    if (it != end()) {
        return quality_of_protection_t(it->second);
    }
    return quality_of_protection_t{};
}


string digest_challenge_t::header(const url_t& url,
    const parameters_t& parameters,
    const digest_t& digest,
    const string_wrapper& body,
    const string_wrapper& method)
{
    // get string to hash
    auto quality = qop();
    auto path = url.path() + parameters.get();
    string a1 = digest.username + ":" + realm() + ":" + digest.password;
    string a2(method);
    a2 += ":" + path;
    if (quality.authint()) {
        a2 += ":";
        a2.append(body.data(), body.size());
    }

    // get our initial hash values
    auto algo = algorithm();
    auto hasher = (algo == sha1_digest_algorithm) ? sha1_hex : md5_hex;
    string ha1 = hasher(a1);
    string ha2 = hasher(a2);

    // MD5 session digests also hash the nonce
    if (algo == md5_sess_digest_algorithm) {
        ha1 = hasher(ha1 + ":" + nonce() + ":" + cnonce());
    }

    // create the hex digest
    ++nonce_counter;
    string response;
    if (quality.empty()) {
        response = hasher(ha1 + ":" + nonce() + ":" + ha2);
    } else {
        response = hasher(ha1 + ":" + nonce() + ":" + nc() + ":" + cnonce() + ":auth:" + ha2);
    }

    // create our header
    string header;
    header += "Authorization: Digest username=\"" + digest.username;
    header += "\", realm=\"" + realm();
    header += "\", nonce=\"" + nonce();
    header += "\", uri=\"" + path;
    header += "\", response=\"" + response + "\"";

    // optional arguments
    if (quality) {
        header += ", qop=\"" + join(quality, ",");
        header += "\", nc=" + nc();
        header += ", cnonce=\"" + cnonce() + "\"";
    }
    if (find("opaque") != end()) {
        header += ", opaque=\"" + at("opaque") + "\"";
    }
    if (find("algorithm") != end()) {
        header += ", algorithm=\"" + at("algorithm") + "\"";
    }

    header += "\r\n";

    return header;
}


digest_challenge_t::operator bool() const
{
    return !empty();
}

PYCPP_END_NAMESPACE
