//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Create no-opt adaptor.
 */

#include <pycpp/config.h>
#include <pycpp/lattice/dns.h>
#include <pycpp/lattice/ssl.h>
#include <pycpp/preprocessor/compiler.h>
#include <pycpp/view/string.h>
#include <cassert>

#if defined(HAVE_MSVC)
#   pragma warning(push)
#   pragma warning(disable:4722)
#endif

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief No-opt SSL socket adaptor.
 */
template <typename HttpAdaptor>
struct no_ssl_adaptor_t
{
    typedef no_ssl_adaptor_t<HttpAdaptor> self;

    no_ssl_adaptor_t();
    no_ssl_adaptor_t(const self&) = delete;
    self & operator=(const self&) = delete;
    ~no_ssl_adaptor_t();

    // REQUESTS
    bool open(const addrinfo& info, const string_view& host);
    void close();
    size_t write(const char *buf, size_t len);
    size_t read(char *buf, size_t count);
};


// IMPLEMENTATION
// --------------


template <typename HttpAdaptor>
no_ssl_adaptor_t<HttpAdaptor>::no_ssl_adaptor_t()
{}


template <typename HttpAdaptor>
no_ssl_adaptor_t<HttpAdaptor>::~no_ssl_adaptor_t()
{}


template <typename HttpAdaptor>
bool no_ssl_adaptor_t<HttpAdaptor>::open(const addrinfo &info, const string_view &host)
{
    assert(false);
    return false;
}


template <typename HttpAdaptor>
void no_ssl_adaptor_t<HttpAdaptor>::close()
{
    assert(false);
}


template <typename HttpAdaptor>
size_t no_ssl_adaptor_t<HttpAdaptor>::write(const char *buf,
    size_t len)
{
    assert(false);
    return 0;
}


template <typename HttpAdaptor>
size_t no_ssl_adaptor_t<HttpAdaptor>::read(char *buf, size_t count)
{
    assert(false);
    return 0;
}

PYCPP_END_NAMESPACE

#if defined(HAVE_MSVC)
#   pragma warning(pop)
#endif
