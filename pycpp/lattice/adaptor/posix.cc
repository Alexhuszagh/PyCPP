//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief POSIX socket adaptor.
 */

#include <pycpp/preprocessor/os.h>
#if defined(OS_POSIX)

#include <pycpp/lattice/adaptor/posix.h>
#include <pycpp/lattice/util.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


posix_socket_adaptor_t::posix_socket_adaptor_t()
{}


posix_socket_adaptor_t::~posix_socket_adaptor_t()
{}


bool posix_socket_adaptor_t::open(const addrinfo& info, const string_view&)
{
    sock = ::socket(info.ai_family, info.ai_socktype, info.ai_protocol);
    if (sock < 0) {
        return false;
    }
    set_reuse_address();
    if (::connect(sock, info.ai_addr, info.ai_addrlen) >= 0) {
        return true;
    }

    ::close(sock);
    return false;
}


void posix_socket_adaptor_t::close()
{
    if (sock >= 0) {
        ::close(sock);
        sock = -1;
    }
}


size_t posix_socket_adaptor_t::write(const char *buf, size_t len)
{
    return ::send(sock, buf, len, 0);
}


size_t posix_socket_adaptor_t::read(char *buf, size_t count)
{
    return ::recv(sock, buf, count, 0);
}


void posix_socket_adaptor_t::set_reuse_address()
{
    int reuse = 1;
    char *option = reinterpret_cast<char*>(&reuse);
    int size = sizeof(reuse);
    if (::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, option, size)) {
        throw std::runtime_error("Unable to set socket option via setsockopt().");
    }

    #ifdef SO_REUSEPORT
        // use reuseport if available
        if (::setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, option, size)) {
            throw std::runtime_error("Unable to set socket option via setsockopt().");
        }
    #endif
}


/**
 *  \warning Microseconds are not set since it causes stoichastic
 *  errors.
 */
void posix_socket_adaptor_t::set_timeout(const timeout_t& timeout)
{
    // create timeout, do not set microseconds as it causes errors
    struct timeval value;
    value.tv_sec = timeout.seconds();
    value.tv_usec = 0;

    // set options
    char *option = reinterpret_cast<char*>(&value);
    socklen_t size = sizeof(timeval);
    if (::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, option, size)) {
        throw std::runtime_error("Unable to set socket option via setsockopt().");
    }
    if (::setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, option, size)) {
        throw std::runtime_error("Unable to set socket option via setsockopt().");
    }
}


void posix_socket_adaptor_t::set_certificate_file(const certificate_file_t& certificate)
{
    encryption_warning();
}


void posix_socket_adaptor_t::set_revocation_lists(const revocation_lists_t& revoke)
{
    encryption_warning();
}


void posix_socket_adaptor_t::set_ssl_protocol(ssl_protocol_t ssl)
{
    encryption_warning();
}


const int posix_socket_adaptor_t::fd() const
{
    return sock;
}

PYCPP_END_NAMESPACE

#endif
