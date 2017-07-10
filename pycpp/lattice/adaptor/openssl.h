//  :copyright: (c) 1998 - 2016, Daniel Stenberg, <daniel@haxx.se>, et al.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief OpenSSL socket adaptor.
 */

#pragma once

#ifdef PYCPP_HAVE_OPENSSL

#include <pycpp/lattice/dns.h>
#include <pycpp/lattice/method.h>
#include <pycpp/lattice/ssl.h>
#include <pycpp/lattice/timeout.h>
#include <pycpp/lattice/url.h>
#include <pycpp/lattice/util.h>
#include <openssl/asn1.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>

// LEGACY
#ifndef TLS_client_method
#   define TLS_client_method SSLv23_client_method
#endif
#ifndef ASN1_STRING_get0_data
#   define ASN1_STRING_get0_data(x) ASN1_STRING_data(x)
#endif

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

const char * const PREFERRED_CIPHERS = "HIGH:!aNULL:!kRSA:!SRP:!PSK:!CAMELLIA:!RC4:!MD5:!DSS";
static bool SSL_INITIALIZED = false;
static X509_STORE *STORE = nullptr;

// OBJECTS
// -------


/**
 *  \brief Socket adaptor for OpenSSL.
 */
template <typename HttpAdaptor>
class open_ssl_adaptor_t
{
public:
    typedef open_ssl_adaptor_t<HttpAdaptor> self;

    open_ssl_adaptor_t();
    open_ssl_adaptor_t(const self&) = delete;
    self& operator=(const self&) = delete;
    ~open_ssl_adaptor_t();

    // REQUESTS
    bool open(const addrinfo& info, const std::string& host);
    void close();
    size_t write(const char *buf, size_t len);
    size_t read(char *buf, size_t count);

    // OPTIONS
    void set_reuse_address();
    void set_timeout(const timeout_t& timeout);
    void set_certificate_file(const certificate_file_t& certificate);
    void set_revocation_lists(const revocation_lists_t& revoke);
    void set_ssl_protocol(ssl_protocol_t protocol);
    void set_verify_peer(const verify_peer_t& peer);

protected:
    HttpAdaptor adaptor;
    certificate_file_t certificate;
    revocation_lists_t revoke;
    ssl_protocol_t protocol = TLS;
    verify_peer_t verifypeer;

    SSL_CTX *ctx = nullptr;
    SSL *ssl = nullptr;

    static void initialize();
    static void cleanup();
    void set_context();
    void set_certificate();
    void set_revoke();
    void set_verify(const std::string& host);
    void ssl_connect();
};


// IMPLEMENTATION
// --------------


/**
 *  \brief Initialize OpenSSL.
 */
template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::initialize()
{
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_ssl_algorithms();
}


/**
 *  \brief Cleanup OpenSSL (noop).
 */
template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::cleanup()
{}


/**
 *  \brief Set the SSL connection context.
 */
template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::set_context()
{
    // initialize SSL methods
    switch (protocol) {
        case SSL_V23:
            ctx = SSL_CTX_new(SSLv23_client_method());
            break;
        case TLS_V12:
            ctx = SSL_CTX_new(TLSv1_2_client_method());
            break;
        case TLS_V11:
            ctx = SSL_CTX_new(TLSv1_1_client_method());
            break;
        case TLS_V1:
            ctx = SSL_CTX_new(TLSv1_client_method());
            break;
        case SSL_V3:
            ctx = SSL_CTX_new(SSLv3_client_method());
            break;
        case TLS:
            /* fallthrough */
        default:
            ctx = SSL_CTX_new(TLS_client_method());
            break;
    }
    if (!ctx) {
        throw std::runtime_error("Unable to initialize SSL context.");
    }
    SSL_CTX_set_options(ctx, SSL_OP_SINGLE_DH_USE);
}


/**
 *  \brief Set certificate file for the store.
 */
template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::set_certificate()
{
    int ok = 1;
    const char *data = certificate.data();
    switch (certificate.format()) {
        case PEM:
            ok = SSL_CTX_use_certificate_chain_file(ctx, data);
            break;
        case ASN1:
            ok = SSL_CTX_use_certificate_file(ctx, data, SSL_FILETYPE_ASN1);
            break;
        case SSL_ENGINE:
            /* fallthrough */
        case PKCS8:
            /* fallthrough */
        case PKCS12:
            /* fallthrough */
        default:
            /* don't recognize certificates, fail */
            ok = -1;
    }

    if (ok != 1) {
        throw std::runtime_error("Unable to load certificates from file.");
    }
}


/**
 *  \brief Set revocation lists for the store.
 */
template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::set_revoke()
{
    if (revoke.empty()) {
        return;
    }

    // get our store and lookup, and load the store
    X509_STORE *store = SSL_CTX_get_cert_store(ctx);
    X509_LOOKUP *lookup = X509_STORE_add_lookup(store, X509_LOOKUP_file());
    int format = X509_FILETYPE_PEM;
    if (!lookup || !(X509_load_crl_file(lookup, revoke.data(), format))) {
        throw std::runtime_error("Unable to load certificates from file.");
    } else {
        int flags = X509_V_FLAG_CRL_CHECK | X509_V_FLAG_CRL_CHECK_ALL;
        X509_STORE_set_flags(store, flags);
    }
}


/**
 *  \brief Verify untrusted certificate with certificate bundle.
 */
template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::set_verify(const std::string& host)
{
    // set verification context
    int mode = verifypeer ? SSL_VERIFY_PEER : SSL_VERIFY_NONE;
    SSL_CTX_set_verify(ctx, mode, nullptr);
    if (!verifypeer) {
        return;
    }

     // hostname verification
    X509_VERIFY_PARAM *param = SSL_get0_param(ssl);
    X509_VERIFY_PARAM_set_hostflags(param, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
    X509_VERIFY_PARAM_set1_host(param, host.data(), 0);

    // set flags to avoid issues with legacy certificates
    X509_STORE *store = SSL_CTX_get_cert_store(ctx);
    X509_STORE_set_flags(store, X509_V_FLAG_TRUSTED_FIRST);

    // set preferred ciphers
    SSL_set_cipher_list(ssl, PREFERRED_CIPHERS);

    // initalize content with bundle
    if (certificate) {
        if (!SSL_CTX_load_verify_locations(ctx, certificate.data(), nullptr)) {
            throw std::runtime_error("Unable to load certificates from file.");
        }
    } else {
        SSL_CTX_set_default_verify_paths(ctx);
    }
}


/**
 *  \brief Connect to the remote host via SSL connect.
 *
 *  Process read and write descriptors.
 *
 *  \warning Errors can be thrown for **no** reason, skip the null
 *  errors, including system errors without an error log, and throw
 *  handshake errors otherwise.
 */
template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::ssl_connect()
{
    while (SSL_connect(ssl) == -1) {
        fd_set desriptors;
        FD_ZERO(&desriptors);
        FD_SET(adaptor.fd(), &desriptors);

        switch (SSL_get_error(ssl, -1)) {
            case SSL_ERROR_NONE:
                /* fallthrough */
            case SSL_ERROR_ZERO_RETURN:
                /* no error */
                return;
            case SSL_ERROR_WANT_READ:
                select(adaptor.fd() + 1, &desriptors, NULL, NULL, NULL);
                break;
            case SSL_ERROR_WANT_WRITE:
                select(adaptor.fd() + 1, NULL, &desriptors, NULL, NULL);
                break;
            case SSL_ERROR_SYSCALL:
                /* unknown error */
                if (!ERR_get_error()) {
                    // no error
                    return;
                }
            default:
                throw std::runtime_error("Unable to complete SSL handshake.");
        }
    }
}


template <typename HttpAdaptor>
open_ssl_adaptor_t<HttpAdaptor>::~open_ssl_adaptor_t()
{
    close();
}


/**
 *  If not initialized, initialize SSL.
 */
template <typename HttpAdaptor>
open_ssl_adaptor_t<HttpAdaptor>::open_ssl_adaptor_t()
{
    std::lock_guard<std::mutex> lock(MUTEX);
    if (!SSL_INITIALIZED) {
        initialize();
        std::atexit(cleanup);
        SSL_INITIALIZED = true;
    }
}


template <typename HttpAdaptor>
bool open_ssl_adaptor_t<HttpAdaptor>::open(const addrinfo& info, const std::string& host)
{
    set_context();
    ssl = SSL_new(ctx);
    set_verify(host);
    if (certificate) {
        set_certificate();
    }
    if (revoke) {
        set_revoke();
    }

    // open socket and create SSL
    SSL_set_connect_state(ssl);
    adaptor.open(info, host);
    SSL_set_fd(ssl, adaptor.fd());
    ssl_connect();

    return true;
}


template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::close()
{
    if (ssl) {
        SSL_shutdown(ssl);
        adaptor.close();
        SSL_free(ssl);
        ssl = nullptr;
    }
    if (ctx) {
        SSL_CTX_free(ctx);
        ctx = nullptr;
    }
}


template <typename HttpAdaptor>
size_t open_ssl_adaptor_t<HttpAdaptor>::write(const char *buf, size_t len)
{
    return SSL_write(ssl, buf, len);
}


template <typename HttpAdaptor>
size_t open_ssl_adaptor_t<HttpAdaptor>::read(char *buf, size_t count)
{
    return SSL_read(ssl, buf, count);
}


template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::set_reuse_address()
{
    adaptor.set_reuse_address();
}


template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::set_timeout(const timeout_t& timeout)
{
    adaptor.set_timeout(timeout);
}


template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::set_certificate_file(const certificate_file_t& certificate)
{
    this->certificate = certificate;
}


template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::set_revocation_lists(const revocation_lists_t& revoke)
{
    this->revoke = revoke;
}


template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::set_ssl_protocol(ssl_protocol_t protocol)
{
    this->protocol = protocol;
}


template <typename HttpAdaptor>
void open_ssl_adaptor_t<HttpAdaptor>::set_verify_peer(const verify_peer_t& peer)
{
    this->verifypeer = verifypeer;
}

PYCPP_END_NAMESPACE

#endif
