//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief HTTP/HTTPS request.
 */

#pragma once

#include <pycpp/lattice/adaptor.h>
#include <pycpp/lattice/auth.h>
#include <pycpp/lattice/connection.h>
#include <pycpp/lattice/cookie.h>
#include <pycpp/lattice/digest.h>
#include <pycpp/lattice/dns.h>
#include <pycpp/lattice/header.h>
#include <pycpp/lattice/method.h>
#include <pycpp/lattice/multipart.h>
#include <pycpp/lattice/parameter.h>
#include <pycpp/lattice/proxy.h>
#include <pycpp/lattice/redirect.h>
#include <pycpp/lattice/response.h>
#include <pycpp/lattice/ssl.h>
#include <pycpp/lattice/timeout.h>
#include <pycpp/lattice/url.h>
#include <pycpp/lexical.h>
#include <pycpp/stl/sstream.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief HTTP request object.
 */
class request_t
{
public:
    request_t() = default;
    request_t(const request_t &other) = default;
    request_t & operator=(const request_t&) = default;
    request_t(request_t&&) = default;
    request_t & operator=(request_t&&) = default;

    // EXPLICIT OPTIONS
    void set_method(method_t);
    void set_url(const url_t&);
    void set_url(url_t&&);
    void set_parameters(const parameters_t&);
    void set_parameters(parameters_t&&);
    void set_header(const header_t&);
    void set_timeout(const timeout_t&);
    void set_auth(const authentication_t&);
    void set_digest(const digest_t&);
    void set_multipart(const multipart_t&);
    void set_multipart(multipart_t&&);
    void set_proxy(const proxy_t&);
    void set_proxy(proxy_t&&);
    void set_body(const body_t&);
    void set_body(body_t&&);
    void set_payload(const payload_t&);
    void set_payload(payload_t&&);
    void set_cookies(const cookies_t&);
    void set_redirects(const redirects_t&);
    void set_certificate_file(const certificate_file_t&);
    void set_certificate_file(certificate_file_t&&);
    void set_revocation_lists(const revocation_lists_t&);
    void set_revocation_lists(revocation_lists_t&&);
    void set_ssl_protocol(ssl_protocol_t);
    void set_verify_peer(const verify_peer_t&);
    void set_verify_peer(verify_peer_t&&);
    void set_cache(const dns_cache_t&);

    // LATTICE_FWDING OPTIONS
    void set_option(method_t);
    void set_option(const url_t&);
    void set_option(url_t&&);
    void set_option(const parameters_t&);
    void set_option(parameters_t&&);
    void set_option(const header_t&);
    void set_option(const timeout_t&);
    void set_option(const authentication_t&);
    void set_option(const digest_t&);
    void set_option(const multipart_t&);
    void set_option(multipart_t&&);
    void set_option(const proxy_t&);
    void set_option(proxy_t&&);
    void set_option(const body_t&);
    void set_option(body_t&&);
    void set_option(const payload_t&);
    void set_option(payload_t&&);
    void set_option(const cookies_t&);
    void set_option(const redirects_t&);
    void set_option(const certificate_file_t&);
    void set_option(certificate_file_t&&);
    void set_option(const revocation_lists_t&);
    void set_option(revocation_lists_t&&);
    void set_option(ssl_protocol_t);
    void set_option(const verify_peer_t&);
    void set_option(verify_peer_t&&);
    void set_option(const dns_cache_t&);

    // ACCESS
    method_t get_method() const;
    const url_t& get_url() const;
    const parameters_t& get_parameters() const;
    const header_t& get_header() const;
    const timeout_t& get_timeout() const;
    const digest_t& get_digest() const;
    const redirects_t& get_redirects() const;
    const certificate_file_t& get_certificate_file() const;
    const revocation_lists_t& get_revocation_lists() const;
    ssl_protocol_t get_ssl_protocol() const;
    const verify_peer_t& get_verify_peer() const;
    const dns_cache_t get_dns_cache() const;

    // CONNECTIONS
    template <typename... Ts>
    string message(Ts&&... ts) const;
    string method_name() const;

    response_t exec();

    template <typename Connection>
    void open(Connection&) const;

    template <typename Connection>
    void reset(Connection&, const response_t&);

    template <typename Connection>
    response_t exec(Connection&);

protected:
    url_t url;
    parameters_t parameters;
    header_t header;
    digest_t digest;
    multipart_t multipart;
    proxy_t proxy;
    timeout_t timeout;
    redirects_t redirects;
    certificate_file_t certificate;
    revocation_lists_t revoke;
    method_t method = static_cast<method_t>(0);
    ssl_protocol_t ssl = static_cast<ssl_protocol_t>(0);
    verify_peer_t verifypeer;
    dns_cache_t cache = nullptr;

    string method_header() const;
    string method_header(const response_t&) const;
};


// FUNCTIONS
// ---------


template <typename T>
void set_option(request_t& request, T&& t)
{
    request.set_option(forward<T>(t));
}


template <typename T, typename... Ts>
void set_option(request_t& request, T&& t, Ts&&... ts)
{
    set_option(request, forward<T>(t));
    set_option(request, forward<Ts>(ts)...);
}


template <typename... Ts>
response_t Delete(Ts&&... ts)
{
    request_t request;
    set_option(request, DELETE, forward<Ts>(ts)...);

    return request.exec();
}


template <typename... Ts>
response_t Get(Ts&&... ts)
{
    request_t request;
    set_option(request, GET, forward<Ts>(ts)...);

    return request.exec();
}


template <typename... Ts>
response_t Head(Ts&&... ts)
{
    request_t request;
    set_option(request, HEAD, forward<Ts>(ts)...);

    return request.exec();
}


template <typename... Ts>
response_t Options(Ts&&... ts)
{
    request_t request;
    set_option(request, OPTIONS, forward<Ts>(ts)...);

    return request.exec();
}


template <typename... Ts>
response_t Patch(Ts&&... ts)
{
    request_t request;
    set_option(request, PATCH, forward<Ts>(ts)...);

    return request.exec();
}


template <typename... Ts>
response_t Post(Ts&&... ts)
{
    request_t request;
    set_option(request, POST, forward<Ts>(ts)...);

    return request.exec();
}


template <typename... Ts>
response_t Put(Ts&&... ts)
{
    request_t request;
    set_option(request, PUT, forward<Ts>(ts)...);

    return request.exec();
}


template <typename... Ts>
response_t Trace(Ts&&... ts)
{
    request_t request;
    set_option(request, TRACE, forward<Ts>(ts)...);

    return request.exec();
}


// IMPLEMENTATION
// --------------


template <typename... Ts>
string request_t::message(Ts&&... ts) const
{
    string data;

    // get our formatted body
    string body;
    if (method == POST && parameters) {
        body += parameters.post();
    } else if (multipart) {
        body += multipart.str();
    }

    // get formatted headers
    auto headers = method_header(forward<Ts>(ts)...);
    if (!body.empty()) {
        headers += "Content-Length: " + lexical(body.size()) + "\r\n";
    }

    // get first line
    if (method == POST) {
        data += method_name() + " " + url.path()
             + " HTTP/1.1\r\n"
             + headers
             + "\r\n" + body;
    } else {
        data += method_name() + " " + url.path() + parameters.get()
             + " HTTP/1.1\r\n"
             + headers
             + "\r\n" + body;
    }

    // end message with double CRLF
    data += "\r\n";
    if (!body.empty()) {
        data += "\r\n";
    }

    return data;
}


/**
 *  \brief Make request to server.
 *
 *  To avoid compiling external libraries into lattice, misuse inline
 *  to keep this in the header.
 */
inline response_t request_t::exec()
{
    auto service = url.service();
    if (service == "http") {
        http_connection_t connection;
        return exec(connection);
    } else if (service == "https") {
        https_connection_t connection;
        return exec(connection);
    } else {
        string message("Network scheme " + service + " is not supported.");
        throw runtime_error(message.data());
    }

    return response_t();
}


template <typename Connection>
response_t request_t::exec(Connection& connection)
{
    open(connection);
    response_t response;
    do {
        connection.write(message());
        response = response_t(connection);
        if (response.unauthorized() && digest) {
            // using digest authentication
            connection.write(message(response));
            response = response_t(connection);
            return response;
        } else if ((method = response.redirect(method)) != STOP) {
            reset(connection, response);
        } else {
            break;
        }
    } while (redirects--);

    return response;
}


template <typename Connection>
void request_t::open(Connection& connection) const
{
    // set options
    connection.set_verify_peer(verifypeer);
    if (certificate) {
        connection.set_certificate_file(certificate);
    }
    if (revoke) {
        connection.set_revocation_lists(revoke);
    }
    if (int_t(ssl)) {
        connection.set_ssl_protocol(ssl);
    }
    if (cache) {
        connection.set_cache(cache);
    }

    // open and set timeout
    if (!proxy) {
        connection.open(url);
    } else {
        connection.open(url_t(proxy));
    }
    if (timeout) {
        connection.set_timeout(timeout);
    }
}


template <typename Connection>
void request_t::reset(Connection& connection, const response_t& response)
{
    // check if we need to reset connection
    bool reconnect = header.close_connection();
    reconnect |= response.headers().close_connection();

    url_t newurl(response.headers().at("location"));
    if (newurl.absolute()) {
        // reconnect if the service or host changes
        reconnect |= url.service() != newurl.service();
        reconnect |= url.host() != newurl.host();
        url = newurl;
    } else {
        url.set_path(newurl.path());
    }

    // reset connection
    if (reconnect) {
        connection.close();
        open(connection);
    }
}

PYCPP_END_NAMESPACE
