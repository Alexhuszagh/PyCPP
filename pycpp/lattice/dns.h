//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Lattice
 *  \brief Domain name lookups and address caching.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/stl/initializer_list.h>
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
using dns_cache_t = shared_ptr<address_cache_t>;

// OBJECTS
// -------


/**
 *  \brief Iterator over addresses.
 */
struct address_iterator_t: iterator<forward_iterator_tag, addrinfo>
{
    // MEMBER TYPES
    // ------------
    using self_t = address_iterator_t;
    using base_t = iterator<forward_iterator_tag, addrinfo>;
    using typename base_t::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    // MEMBER FUNCTIONS
    // ----------------
    address_iterator_t() = default;
    address_iterator_t(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    address_iterator_t(self_t&&) = default;
    self_t& operator=(self_t&&) = default;

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
    mutable sockaddr address;
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
struct address_cache_t: unordered_multimap<std::string, address_t>
{
    address_cache_t() = default;
    address_cache_t(const address_cache_t&) = default;
    address_cache_t& operator=(const address_cache_t&) = default;
    address_cache_t(address_cache_t&&) = default;
    address_cache_t& operator=(address_cache_t&&) = default;
    address_cache_t(initializer_list<typename address_cache_t::value_type> list);
};


/**
 *  \brief DNS lookup for a server host.
 *
 *  \param host             Base url of host address, "example.com"
 *  \param service          Service for connection, "http"
 */
class dns_lookup_t
{
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

protected:
    addrinfo* info = nullptr;
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
    return make_shared<address_cache_t>(forward<Ts>(ts)...);
}

PYCPP_END_NAMESPACE
