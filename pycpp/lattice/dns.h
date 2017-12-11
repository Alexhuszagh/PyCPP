//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Domain name lookups and address caching.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/string.h>
#include <pycpp/stl/unordered_map.h>
#include <pycpp/string/string.h>

#if defined(OS_WINDOWS)
#   include <winsock2.h>
#   include <ws2tcpip.h>
#   include <pycpp/windows/winapi.h>
#else
#   include <netdb.h>
#endif

PYCPP_BEGIN_NAMESPACE

// TYPES
// -----

struct address_cache_t;
typedef std::shared_ptr<address_cache_t> dns_cache_t;

// OBJECTS
// -------


/**
 *  \brief Iterator over addresses.
 */
struct address_iterator_t: std::iterator<std::forward_iterator_tag, addrinfo>
{
    // MEMBER TYPES
    // ------------
    typedef address_iterator_t self;
    typedef std::iterator<std::forward_iterator_tag, addrinfo> base;
    using typename base::value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    // MEMBER FUNCTIONS
    // ----------------
    address_iterator_t() = default;
    address_iterator_t(const self&) = default;
    self& operator=(const self&) = default;
    address_iterator_t(self&&) = default;
    self& operator=(self&&) = default;

    address_iterator_t(pointer ptr);

    reference operator*();
    const_reference operator*() const;
    pointer operator->();
    const_pointer operator->() const;
    address_iterator_t& operator++();
    address_iterator_t operator++(int);
    bool operator==(const address_iterator_t&) const;
    bool operator!=(const address_iterator_t&) const;

private:
    pointer ptr = nullptr;
};


/**
 *  \brief Host address information.
 */
struct address_t
{
    int family;
    int socket_type;
    int protocol;
    sockaddr address;
    size_t length;

    address_t() = default;
    address_t(const address_t&) = default;
    address_t& operator=(const address_t&) = default;
    address_t(address_t&&) = default;
    address_t& operator=(address_t&&) = default;

    address_t(const addrinfo& info);

    explicit operator addrinfo() const;
};


/**
 *  \brief Cache for DNS lookups.
 */
struct address_cache_t: std::unordered_multimap<std::string, address_t>
{
    typedef std::unordered_multimap<std::string, address_t> base;
    using base::base;

    template <typename ...Args>
    friend dns_cache_t create_dns_cache(Args&& ...args);
};


/**
 *  \brief DNS lookup for a server host.
 *
 *  \param host             Base url of host address, "example.com"
 *  \param service          Service for connection, "http"
 */
class dns_lookup_t
{
protected:
    addrinfo* info = nullptr;

public:
    dns_lookup_t() = default;
    dns_lookup_t(const dns_lookup_t&) = delete;
    dns_lookup_t& operator=(const dns_lookup_t&) = delete;
    dns_lookup_t(dns_lookup_t&&) = default;
    dns_lookup_t& operator=(dns_lookup_t&&) = default;
    ~dns_lookup_t();

    dns_lookup_t(const string_wrapper& host, const string_wrapper& service);

    // ITERATORS
    address_iterator_t begin() const;
    address_iterator_t end() const;
};

// IMPLEMENTATION
// --------------


/**
 *  \brief Only expose cache creator for lifetime management.
 *
 *  Expose only the `create_dns_cache` function call to set automatic
 *  lifetime management for the API.
 */
template <typename... Ts>
dns_cache_t create_dns_cache(Ts&& ...ts)
{
    return std::make_shared<address_cache_t>(std::forward<Ts>(ts)...);
}

PYCPP_END_NAMESPACE
