//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Windows socket adaptor.
 */

#include <pycpp/preprocessor/compiler.h>
#include <pycpp/preprocessor/os.h>
#if defined(OS_WINDOWS)

#include <pycpp/lattice/adaptor/windows.h>
#include <pycpp/lattice/util.h>
#include <winsock2.h>
#include <windows.h>
#include <cstdlib>

#if defined(HAVE_MSVC)
#   pragma warning(push)
#   pragma warning(disable:4267)
#endif

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static WSADATA SOCK_DATA;
static bool WSA_INITIALIZED = false;

// OBJECTS
// -------


/**
 *  If not initialized, start the Winsock API and register cleanup.
 */
win32_socket_adaptor_t::win32_socket_adaptor_t()
{
    std::lock_guard<std::mutex> lock(LATTICE_MUTEX);
    if (!WSA_INITIALIZED) {
        if (WSAStartup(MAKEWORD(2,2), &SOCK_DATA) != 0){
            throw std::runtime_error("Unable to startup the Windows socket API.");
        }
        std::atexit([]() {
            WSACleanup();
        });
        WSA_INITIALIZED = true;
    }
}


win32_socket_adaptor_t::~win32_socket_adaptor_t()
{}


bool win32_socket_adaptor_t::open(const addrinfo& info, const std::string&)
{
    sock = ::socket(info.ai_family, info.ai_socktype, info.ai_protocol);
    if (sock == INVALID_SOCKET) {
        return false;
    }
    set_reuse_address();
    if (!::connect(sock, info.ai_addr, info.ai_addrlen)) {
        return true;
    }

    ::closesocket(sock);
    return false;
}


bool win32_socket_adaptor_t::close()
{
    if (sock != INVALID_SOCKET) {
        ::closesocket(sock);
        sock = INVALID_SOCKET;
        return true;
    }
    return false;
}


size_t win32_socket_adaptor_t::write(const char *buf, size_t len)
{
    return ::send(sock, buf, len, 0);
}


size_t win32_socket_adaptor_t::read(char *buf, size_t count)
{
    return ::recv(sock, buf, count, 0);
}


void win32_socket_adaptor_t::set_reuse_address()
{
    int reuse = 1;
    char *option = reinterpret_cast<char*>(&reuse);
    int size = sizeof(reuse);
    if (::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, option, size)) {
        throw std::runtime_error("Unable to set socket option via setsockopt().");
    }
}


void win32_socket_adaptor_t::set_timeout(const timeout_t& timeout)
{
    // create timeout
    int ms = timeout.milliseconds();
    char *option = reinterpret_cast<char*>(&ms);
    int size = sizeof(ms);

    // set options
    if (::setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, option, size)) {
        throw std::runtime_error("Unable to set socket option via setsockopt().");
    }
    if (::setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, option, size)) {
        throw std::runtime_error("Unable to set socket option via setsockopt().");
    }
}


void win32_socket_adaptor_t::set_certificate_file(const certificate_file_t& certificate)
{
    encryption_warning();
}


void win32_socket_adaptor_t::set_revocation_lists(const revocation_lists_t& revoke)
{
    encryption_warning();
}


void win32_socket_adaptor_t::set_ssl_protocol(ssl_protocol_t ssl)
{
    encryption_warning();
}


const SOCKET win32_socket_adaptor_t::fd() const
{
    return sock;
}

PYCPP_END_NAMESPACE

#if defined(HAVE_MSVC)
#   pragma warning(pop)
#endif

#endif
