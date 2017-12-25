//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Weakly encrypted authentication for requests.
 *
 *  \warning Digest authentication, although popular, is insecure
 *  and should be considered deprecated and broken beyond repair.
 */

#pragma once

#include <pycpp/lattice/auth.h>
#include <pycpp/lattice/crypto.h>
#include <pycpp/stl/unordered_map.h>
#include <pycpp/stl/vector.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct parameters_t;
struct url_t;

// OBJECTS
// -------

/**
 *  \brief Case-insensitive hash for ASCII.
 */
struct lowercase_hash
{
    size_t operator()(const string&) const;
};


/**
 *  \brief Case-insensitive key_equal for ASCII.
 */
struct lowercase_equal_to
{
    bool operator()(const string&, const string&) const;
};


/**
 *  \brief Authorization string for requests.
 */
struct digest_t: authentication_t
{
    using authentication_t::authentication_t;
};


/**
 *  \brief Quality of protection directive.
 */
struct quality_of_protection_t: public vector<string>
{
    quality_of_protection_t() = default;
    quality_of_protection_t(const quality_of_protection_t&) = default;
    quality_of_protection_t& operator=(const quality_of_protection_t&) = default;
    quality_of_protection_t(quality_of_protection_t&&) = default;
    quality_of_protection_t& operator=(quality_of_protection_t&&) = default;
    quality_of_protection_t(const string_wrapper& qop);

    bool auth() const;
    bool authint() const;
    explicit operator bool() const;
};


/**
 *  \brief Authenticate challenge sent from the server.
 *
 *  \format
 *      Digest nonce="42148a112dd92b7e5b6ac4769c2a6693", opaque="35fa82343c10f5a83c7d9b8bb29d8518", realm="me@kennethreitz.com", qop=auth
 */
struct digest_challenge_t: public unordered_map<
        string,
        string,
        lowercase_hash,
        lowercase_equal_to
    >
{
public:
    digest_challenge_t() = default;
    digest_challenge_t(const digest_challenge_t&) = default;
    digest_challenge_t& operator=(const digest_challenge_t&) = default;
    digest_challenge_t(digest_challenge_t&&) = default;
    digest_challenge_t& operator=(digest_challenge_t&&) = default;
    digest_challenge_t(const string_wrapper&);

    // DATA
    const string& realm() const;
    const string& nonce() const;
    const string& cnonce();
    string nc() const;
    digest_algorithm_t algorithm() const;
    quality_of_protection_t qop() const;
    string header(const url_t& url,
        const parameters_t& parameters,
        const digest_t& digest,
        const string_wrapper& body,
        const string_wrapper& method);

    explicit operator bool() const;

protected:
    uint32_t nonce_counter = 0;
    string client_nonce;
};

PYCPP_END_NAMESPACE
