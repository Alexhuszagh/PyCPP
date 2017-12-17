//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lattice/dns.h>
#include <pycpp/stl/stdexcept.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


address_iterator_t::address_iterator_t(pointer ptr):
    ptr(ptr)
{}


auto address_iterator_t::operator*() -> reference
{
    return *ptr;
}


auto address_iterator_t::operator*() const -> const_reference
{
    return *ptr;
}


auto address_iterator_t::operator->() -> pointer
{
    return ptr;
}


auto address_iterator_t::operator->() const -> const_pointer
{
    return ptr;
}


auto address_iterator_t::operator++() -> address_iterator_t&
{
    ptr = ptr->ai_next;
    return *this;
}


auto address_iterator_t::operator++(int) -> address_iterator_t
{
    address_iterator_t copy(*this);
    operator++();

    return copy;
}


bool address_iterator_t::operator==(const address_iterator_t& other) const
{
    short sum = bool(ptr) + bool(other.ptr);
    switch (sum) {
        case 0:
            return true;
        case 1:
            return false;
        case 2:
            /* fallthrough */
        default: {
            return (
                ptr->ai_flags == other.ptr->ai_flags &&
                ptr->ai_family == other.ptr->ai_family &&
                ptr->ai_socktype == other.ptr->ai_socktype &&
                ptr->ai_protocol == other.ptr->ai_protocol &&
                ptr->ai_addrlen == other.ptr->ai_addrlen &&
                ptr->ai_addr == other.ptr->ai_addr &&
                ptr->ai_canonname == other.ptr->ai_canonname
            );
        }
    }
}


bool address_iterator_t::operator!=(const address_iterator_t& other) const
{
    return !operator==(other);
}


address_t::address_t(const addrinfo& info):
    family(info.ai_family),
    socket_type(info.ai_socktype),
    protocol(info.ai_protocol),
    address(*info.ai_addr),
    length(info.ai_addrlen)
{}


address_t::operator addrinfo() const
{
    addrinfo info;
    info.ai_family = family;
    info.ai_socktype = socket_type;
    info.ai_protocol = protocol;
    info.ai_addr = const_cast<sockaddr*>(&address);
    info.ai_addrlen = length;

    return info;
}


address_cache_t::address_cache_t(initializer_list<typename address_cache_t::value_type> list)
{
    for (auto it = list.begin(); it != list.end(); ++it) {
        emplace(*it);
    }
}


dns_lookup_t::dns_lookup_t(const string_wrapper& host, const string_wrapper& service)
{
    assert(is_null_terminated(host));
    assert(is_null_terminated(service));

    // initialize our hints
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // get our host and port
    string_wrapper node_string, port_string;
    const char *node = host.data();
    const char *port = service.data();
    const size_t index = host.find(":");
    if (index != std::string::npos) {
        node_string = host.substr(0, index);
        port_string = host.substr(index+1);
        node = node_string.data();
        port = port_string.data();
    }

    if (getaddrinfo(node, port, &hints, &result)) {
        throw runtime_error("Unable to get address from getaddrinfo(): " + std::string(host) + std::string(service));
    }

    info = result;
}


dns_lookup_t::~dns_lookup_t()
{
    if (info) {
        freeaddrinfo(info);
        info = nullptr;
    }
}


address_iterator_t dns_lookup_t::begin() const
{
    return address_iterator_t(info);
}


address_iterator_t dns_lookup_t::end() const
{
    return address_iterator_t(nullptr);
}


PYCPP_END_NAMESPACE

#include <warnings/pop.h>
