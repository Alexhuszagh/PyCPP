//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Windows socket adaptor.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/os.h>

#if defined(OS_WINDOWS)

#include <pycpp/lattice/dns.h>
#include <pycpp/lattice/method.h>
#include <pycpp/lattice/ssl.h>
#include <pycpp/lattice/timeout.h>
#include <pycpp/lattice/url.h>


PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Adaptor for Win32 sockets.
 */
class win32_socket_adaptor_t
{
public:
    typedef win32_socket_adaptor_t self;

    win32_socket_adaptor_t();
    win32_socket_adaptor_t(const self&) = delete;
    self& operator=(const self&) = delete;
    ~win32_socket_adaptor_t();

    // REQUESTS
    bool open(const addrinfo& info, const std::string&);
    bool close();
    size_t write(const char *buf, size_t len);
    size_t read(char *buf, size_t count);

    // OPTIONS
    void set_reuse_address();
    void set_timeout(const timeout_t& timeout);
    void set_certificate_file(const certificate_file_t& certificate);
    void set_revocation_lists(const revocation_lists_t& revoke);
    void set_ssl_protocol(const ssl_protocol_t ssl);

    // DATA
    const SOCKET fd() const;

protected:
    SOCKET sock = INVALID_SOCKET;
};

PYCPP_END_NAMESPACE

#endif              // WIN32
