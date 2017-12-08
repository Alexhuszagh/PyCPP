//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Socket connection.
 */

#pragma once

#include <pycpp/lattice/adaptor.h>
#include <pycpp/lattice/dns.h>
#include <pycpp/lattice/method.h>
#include <pycpp/lattice/ssl.h>
#include <pycpp/lattice/timeout.h>
#include <pycpp/lattice/util.h>
#include <pycpp/lexical.h>
#include <pycpp/lexical/atoi.h>
#include <pycpp/view/string.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>
#include <cstdlib>
#include <cstring>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr size_t BUFFER_SIZE = 8092;

// FUNCTION
// --------


/**
 *  \brief Open connection without a cache.
 */
template <typename Adapter>
void open_connection(Adapter& adaptor, const std::string& host, const std::string& service)
{
    // perform DNS lookup
    for (auto &&info: dns_lookup_t(host, service)) {
        if (adaptor.open(info, host)) {
            return;
        }
    }

    // no suitable addresses found
    throw std::runtime_error("Unable to establish a connection.");
}


/**
 *  \brief Open connection with DNS cache.
 */
template <typename Adapter>
void open_connection(Adapter& adaptor,
    const std::string& host,
    const std::string& service,
    address_cache_t& cache)
{
    // try cached results
    typename address_cache_t::iterator it;
    if ((it = cache.find(host)) != cache.end()) {
        if (adaptor.open(addrinfo(it->second), host)) {
            return;
        }
    }

    // perform DNS lookup
    for (auto &&info: dns_lookup_t(host, service)) {
        if (adaptor.open(info, host)) {
            cache.emplace(host, info);
            return;
        }
    }

    throw std::runtime_error("Unable to establish a connection.");
}


// OBJECTS
// -------


/**
 *  \brief Socket connection.
 *
 *  Establish and maintain connection over socket.
 */
template <typename Adapter>
class connection_t
{
protected:
    Adapter adaptor;
    dns_cache_t cache = nullptr;

    long readn(char *dst, long bytes);

public:
    connection_t();
    connection_t(const connection_t&) = delete;
    connection_t & operator=(const connection_t&) = delete;
    connection_t(connection_t&&) = default;
    connection_t & operator=(connection_t&&) = default;
    ~connection_t();

    // REQUESTS
    void open(const url_t& url);
    void close();
    void write(const string_view& data);
    void set_cache(const dns_cache_t& cache);

    // RESPONSE
    std::string headers();
    std::string chunked();
    std::string body(long length);
    std::string read();

    // OPTIONAL
    template <typename T = Adapter>
    typename std::enable_if<(has_set_timeout<T>::value), void>::type
    set_timeout(const timeout_t& timeout);

    template <typename T = Adapter>
    typename std::enable_if<(!has_set_timeout<T>::value), void>::type
    set_timeout(const timeout_t& timeout);

    template <typename T = Adapter>
    typename std::enable_if<(has_set_certificate_file<T>::value), void>::type
    set_certificate_file(const certificate_file_t& certificate);

    template <typename T = Adapter>
    typename std::enable_if<(!has_set_certificate_file<T>::value), void>::type
    set_certificate_file(const certificate_file_t& certificate);

    template <typename T = Adapter>
    typename std::enable_if<(has_set_revocation_lists<T>::value), void>::type
    set_revocation_lists(const revocation_lists_t& revoke);

    template <typename T = Adapter>
    typename std::enable_if<(!has_set_revocation_lists<T>::value), void>::type
    set_revocation_lists(const revocation_lists_t& revoke);

    template <typename T = Adapter>
    typename std::enable_if<(has_set_ssl_protocol<T>::value), void>::type
    set_ssl_protocol(ssl_protocol_t ssl);

    template <typename T = Adapter>
    typename std::enable_if<(!has_set_ssl_protocol<T>::value), void>::type
    set_ssl_protocol(ssl_protocol_t ssl);

    template <typename T = Adapter>
    typename std::enable_if<(has_set_verify_peer<T>::value), void>::type
    set_verify_peer(const verify_peer_t& peer);

    template <typename T = Adapter>
    typename std::enable_if<(!has_set_verify_peer<T>::value), void>::type
    set_verify_peer(const verify_peer_t& peer);
};


// IMPLEMENTATION
// --------------


/**
 *  Sockets guarantee at least 1 byte will be read, while valid, but do
 *  not guarantee N-bytes will be successfully read. Read until all
 *  data have been extracted.
 */
template <typename Adapter>
long connection_t<Adapter>::readn(char *dst, long bytes)
{
    long count = 0;
    while (bytes) {
        long read = adaptor.read(dst, bytes);
        if (!read) {
            return count;
        }
        bytes -= read;
        dst += read;
        count += read;
    }

    return count;
}


template <typename Adapter>
connection_t<Adapter>::connection_t()
{}


template <typename Adapter>
connection_t<Adapter>::~connection_t()
{
    close();
}


template <typename Adapter>
void connection_t<Adapter>::open(const url_t& url)
{
    if (cache) {
        open_connection(adaptor, url.host(), url.service(), *cache);
    } else {
        open_connection(adaptor, url.host(), url.service());
    }
}


template <typename Adapter>
void connection_t<Adapter>::close()
{
    adaptor.close();
}


template <typename Adapter>
template <typename T>
typename std::enable_if<(has_set_timeout<T>::value), void>::type
connection_t<Adapter>::set_timeout(const timeout_t& timeout)
{
    adaptor.set_timeout(timeout);
}


template <typename Adapter>
template <typename T>
typename std::enable_if<(!has_set_timeout<T>::value), void>::type
connection_t<Adapter>::set_timeout(const timeout_t& timeout)
{}


template <typename Adapter>
template <typename T>
typename std::enable_if<(has_set_certificate_file<T>::value), void>::type
connection_t<Adapter>::set_certificate_file(const certificate_file_t& certificate)
{
    adaptor.set_certificate_file(certificate);
}


template <typename Adapter>
template <typename T>
typename std::enable_if<(!has_set_certificate_file<T>::value), void>::type
connection_t<Adapter>::set_certificate_file(const certificate_file_t& certificate)
{}


template <typename Adapter>
template <typename T>
typename std::enable_if<(has_set_revocation_lists<T>::value), void>::type
connection_t<Adapter>::set_revocation_lists(const revocation_lists_t& revoke)
{
    adaptor.set_revocation_lists(revoke);
}


/**
 *  \brief Set file to manually revoke certificates (noop).
 */
template <typename Adapter>
template <typename T>
typename std::enable_if<(!has_set_revocation_lists<T>::value), void>::type
connection_t<Adapter>::set_revocation_lists(const revocation_lists_t& revoke)
{}


/**
 *  \brief Set SSL protocol.
 */
template <typename Adapter>
template <typename T>
typename std::enable_if<(has_set_ssl_protocol<T>::value), void>::type
connection_t<Adapter>::set_ssl_protocol(ssl_protocol_t ssl)
{
    adaptor.set_ssl_protocol(ssl);
}


/**
 *  \brief Set SSL protocol (noop).
 */
template <typename Adapter>
template <typename T>
typename std::enable_if<(!has_set_ssl_protocol<T>::value), void>::type
connection_t<Adapter>::set_ssl_protocol(ssl_protocol_t ssl)
{}


/**
 *  \brief Change peer certificate validation.
 */
template <typename Adapter>
template <typename T>
typename std::enable_if<(has_set_verify_peer<T>::value), void>::type
connection_t<Adapter>::set_verify_peer(const verify_peer_t& peer)
{
    adaptor.set_verify_peer(peer);
}


/**
 *  \brief Change peer certificate validation (noop).
 */
template <typename Adapter>
template <typename T>
typename std::enable_if<(!has_set_verify_peer<T>::value), void>::type
connection_t<Adapter>::set_verify_peer(const verify_peer_t& peer)
{}


/**
 *  \brief Set DNS cache.
 */
template <typename Adapter>
void connection_t<Adapter>::set_cache(const dns_cache_t& cache)
{
    this->cache = cache;
}


/**
 *  \brief Send data through socket.
 */
template <typename Adapter>
void connection_t<Adapter>::write(const string_view& data)
{
    int sent = adaptor.write(data.data(), data.size());
    if (sent != static_cast<int>(data.size())) {
        throw std::runtime_error("Unable to make request, sent " + lexical(sent) + " bytes.");
    }
}


/**
 *  \brief Read headers data from server.
 *
 *  Slowly read from buffer until a double carriage return is found.
 */
template <typename Adapter>
std::string connection_t<Adapter>::headers()
{
    std::string string;
    int result;
    char src;
    while ((result = adaptor.read(&src, 1))) {
        string += src;
        size_t size = string.size() - 4;
        if (size > 0 && src == '\n' && string.substr(size) == "\r\n\r\n") {
            break;
        }
    }

    return string;
}


/**
 *  \brief Read chunked transfer encoding.
 *
 *  Each message is prefixed with a single line denoting how
 *  long the message is, in hex.
 */
template <typename Adapter>
std::string connection_t<Adapter>::chunked()
{
    // initialize alloc
    std::string hex;
    int result, offset = 0;
    char *buffer = static_cast<char*>(malloc(offset));
    char byte, *src = buffer + offset;

    while ((result = adaptor.read(&byte, 1))) {
        if (!(byte == '\r' || byte == '\n')) {
            hex += byte;
        } else if (hex == "0") {
            // end of file
            break;
        } else if (hex.size()) {
            // get carriage return
            result = adaptor.read(&byte, 1);

            // read bytes
            int64_t bytes = atoi64(hex, 16);
            buffer = static_cast<char*>(realloc(buffer, bytes + offset));
            int64_t read = readn(buffer + offset, bytes);
            offset += read;
            if (read != bytes) {
                break;
            }

            // clear our hex buffer for new messages
            hex.clear();
        }
    }

    // create string output
    std::string output(buffer, offset);
    free(buffer);

    return output;
}


/**
 *  \brief Read non-chunked content of fixed length.
 */
template <typename Adapter>
std::string connection_t<Adapter>::body(long length)
{
    std::string string;
    if (length > 0) {
        string.resize(length);
        adaptor.read(const_cast<char*>(&string[0]), length);
    } else if (length) {
        throw std::runtime_error("Asked to read negative bytes.");
    }

    return string;
}


/**
 *  \brief Read non-chunked content of unknown length.
 */
template <typename Adapter>
std::string connection_t<Adapter>::read()
{
    // read from connection
    int result, offset = 0;
    char *buffer = static_cast<char*>(malloc(BUFFER_SIZE));
    char *src = buffer + offset;
    while ((result = adaptor.read(src, BUFFER_SIZE))) {
        offset += result;
        buffer = static_cast<char*>(realloc(buffer, BUFFER_SIZE + offset));
        src = buffer + offset;
    }

    // create string output
    std::string output(buffer, offset);
    free(buffer);

    return output;
}


// TYPES
// -----

typedef connection_t<http_adaptor_t> http_connection_t;
typedef connection_t<ssl_adaptor_t> https_connection_t;

PYCPP_END_NAMESPACE

#include <warnings/pop.h>
