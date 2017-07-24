//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/hashlib.h>
#include <pycpp/random.h>
#include <pycpp/lattice/digest.h>
#include <pycpp/lattice/parameter.h>
#include <pycpp/lattice/url.h>
#include <pycpp/string/casemap.h>
#include <pycpp/string/hex.h>
#include <pycpp/string/string.h>
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <sstream>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

static std::string sha1_hex(const std::string& str)
{
    secure_string_view view(str.data(), str.size());
    auto hex = sha1_hash(view).hexdigest();
    return std::string(hex.data(), hex.size());
}


static std::string md5_hex(const std::string& str)
{
    secure_string_view view(str.data(), str.size());
    auto hex = md5_hash(view).hexdigest();
    return std::string(hex.data(), hex.size());
}

// OBJECTS
// -------


size_t lowercase_hash::operator()(const std::string& str) const
{
    return std::hash<std::string>()(ascii_tolower(str));
}


bool lowercase_equal_to::operator()(const std::string& lhs, const std::string& rhs) const
{
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin(), [](char l, char r) {
        return ascii_tolower(l) == ascii_tolower(r);
    });
}


quality_of_protection_t::quality_of_protection_t(const std::string& qop)
{
    auto data = split(qop, ",");
    insert(begin(), data.begin(), data.end());
    for (auto &item: *this) {
        item = trim(item);
    }
}


bool quality_of_protection_t::auth() const
{
    return std::any_of(begin(), end(), [](const value_type& data) {
        return data == "auth";
    });
}


bool quality_of_protection_t::authint() const
{
    return std::any_of(begin(), end(), [](const value_type& data) {
        return data == "auth-int";
    });
}


quality_of_protection_t::operator bool() const
{
    return !empty();
}


digest_challenge_t::digest_challenge_t(const std::string& string)
{
    auto data = quoted_split(string.substr(7), ',', '"', '\\');
    for (auto &value: data) {
        value = trim(value);
        const char *begin = value.data();
        const char *ptr = strchr(begin, '=');
        if (ptr) {
            std::string first(begin, ptr);
            std::string second(ptr+1);

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


const std::string & digest_challenge_t::realm() const
{
    return at("realm");
}


const std::string & digest_challenge_t::nonce() const
{
    return at("nonce");
}


const std::string & digest_challenge_t::cnonce()
{
    if (client_nonce.empty()) {
        client_nonce = hex_i32(sysrandom(8));
    }
    return client_nonce;
}


std::string digest_challenge_t::nc() const
{
    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(8) << std::hex << nonce_counter;

    return stream.str();
}


digest_algorithm_t digest_challenge_t::algorithm() const
{
    auto it = find("algorithm");
    if (it == end()) {
        return md5_digest_algorithm;
    }

    std::string data = ascii_tolower(it->second);

    if (data == "md5") {
        return md5_digest_algorithm;
    } else if (data == "md5-sess") {
        return md5_sess_digest_algorithm;
    } else if (data == "sha") {
        return sha1_digest_algorithm;
    }

    throw std::runtime_error("Unknown hashing algorithm for digest authentication.");
}


quality_of_protection_t digest_challenge_t::qop() const
{
    auto it = find("qop");
    if (it != end()) {
        return quality_of_protection_t(it->second);
    }
    return quality_of_protection_t{};
}


std::string digest_challenge_t::header(const url_t& url,
    const parameters_t& parameters,
    const digest_t& digest,
    const std::string& body,
    const std::string& method)
{
    // get string to hash
    auto quality = qop();
    auto path = url.path() + parameters.get();
    std::string a1 = digest.username + ":" + realm() + ":" + digest.password;
    std::string a2 = method + ":" + path;
    if (quality.authint()) {
        a2 += ":" + body;
    }

    // get our initial hash values
    auto algo = algorithm();
    auto hasher = (algo == sha1_digest_algorithm) ? sha1_hex : md5_hex;
    std::string ha1 = hasher(a1);
    std::string ha2 = hasher(a2);

    // MD5 session digests also hash the nonce
    if (algo == md5_sess_digest_algorithm) {
        ha1 = hasher(ha1 + ":" + nonce() + ":" + cnonce());
    }

    // create the hex digest
    ++nonce_counter;
    std::string response;
    if (quality.empty()) {
        response = hasher(ha1 + ":" + nonce() + ":" + ha2);
    } else {
        response = hasher(ha1 + ":" + nonce() + ":" + nc() + ":" + cnonce() + ":auth:" + ha2);
    }

    // create our header
    std::ostringstream header;
    header << "Authorization: Digest " << "username=\"" + digest.username
           << "\", realm=\"" << realm()
           << "\", nonce=\"" << nonce()
           << "\", uri=\"" << path
           << "\", response=\"" << response << "\"";

    // optional arguments
    if (quality) {
        header << ", qop=\"" << join(quality, ",")
               << "\", nc=" << nc()
               << ", cnonce=\"" << cnonce() << "\"";
    }
    if (find("opaque") != end()) {
        header << ", opaque=\"" << at("opaque") << "\"";
    }
    if (find("algorithm") != end()) {
        header << ", algorithm=\"" << at("algorithm") << "\"";
    }

    header << "\r\n";

    return header.str();
}


digest_challenge_t::operator bool() const
{
    return !empty();
}

PYCPP_END_NAMESPACE
