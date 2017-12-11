//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief HTTPS options and validation.
 */

#pragma once

#include <pycpp/stl/string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief SSL encryption protocl.
 */
enum ssl_protocol_t
{
    TLS     = 0,
    SSL_V23 = 1,
    TLS_V12 = 2,
    TLS_V11 = 3,
    TLS_V1  = 4,
    SSL_V3  = 5,
};


/**
 *  \brief Various file formats for the certificate chain.
 */
enum certificate_format_t
{
    PEM         = 1,
    ASN1        = 2,
    SSL_ENGINE  = 3,
    PKCS8       = 4,
    PKCS12      = 5,
};


/**
 *  \brief Holds path to certificate bundle.
 */
struct certificate_file_t: std::string
{
    typedef std::string base;
    using base::base;

    std::string suffix() const;
    certificate_format_t format() const;

    explicit operator bool() const;
};


/**
 *  \brief Holds path to certificate revocation lists.
 */
struct revocation_lists_t: std::string
{
    typedef std::string base;
    using base::base;

    explicit operator bool() const;
};


/**
 *  \brief Verify peer for SSL/TLS connections.
 */
struct verify_peer_t
{
    bool verify = true;

    verify_peer_t() = default;
    verify_peer_t(const verify_peer_t&) = default;
    verify_peer_t & operator=(const verify_peer_t&) = default;
    verify_peer_t(verify_peer_t&&) = default;
    verify_peer_t & operator=(verify_peer_t&&) = default;

    verify_peer_t(bool verify);

    explicit operator bool() const;
};

PYCPP_END_NAMESPACE
