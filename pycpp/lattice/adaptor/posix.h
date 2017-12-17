//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief POSIX socket adaptor.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/os.h>

#if defined(OS_POSIX)

#include <pycpp/lattice/dns.h>
#include <pycpp/lattice/ssl.h>
#include <pycpp/lattice/timeout.h>
#include <pycpp/lattice/url.h>
#include <pycpp/string/string.h>
#include <netdb.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Adapter for POSIX sockets.
 */
class posix_socket_adaptor_t
{
public:
    using self_t = posix_socket_adaptor_t;

    posix_socket_adaptor_t();
    posix_socket_adaptor_t(const self_t&) = delete;
    self_t& operator=(const self_t&) = delete;
    ~posix_socket_adaptor_t();

    // REQUESTS
    bool open(const addrinfo& info, const string_wrapper&);
    void close();
    size_t write(const char *buf, size_t len);
    size_t read(char *buf, size_t count);

    // OPTIONS
    void set_reuse_address();
    void set_timeout(const timeout_t& timeout);
    void set_certificate_file(const certificate_file_t& certificate);
    void set_revocation_lists(const revocation_lists_t& revoke);
    void set_ssl_protocol(ssl_protocol_t ssl);

    // DATA
    const int fd() const;

protected:
    int sock = -1;
};

PYCPP_END_NAMESPACE

#endif                      // OS_POSIX
