//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lattice/request.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/string/base64.h>
#include <pycpp/string/codec.h>
#include <pycpp/string/unicode.h>
#include <assert.h>
#include <stdio.h>

#if defined(OS_WINDOWS)
#   include <wincrypt.h>
#endif

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

#define LATTICE_MAJOR_VERSION 0
#define LATTICE_MINOR_VERSION 1
#define LATTICE_PATCH_VERSION 0
#define LATTICE_VERSION "0.1.0"

// OBJECTS
// -------


string request_t::method_name() const
{
    switch (method) {
        case GET:
            return "GET";
        case HEAD:
            return "HEAD";
        case POST:
            return "POST";
        case DELETE:
            return "DELETE";
        case OPTIONS:
            return "OPTIONS";
        case PATCH:
            return "PATCH";
        case PUT:
            return "PUT";
        case TRACE:
            return "TRACE";
        case CONNECT:
            return "CONNECT";
        default:
            assert(false && "HTTP request method unknown.\n");
    }
}


string request_t::method_header() const
{
    string data;
    data += header.str();
    if (!header.host() && url.absolute()) {
        // specify a default host
        data += "Host: " + url.host() + "\r\n";
    }
    if (!header.user_agent()) {
        // specify a default user agent
        data += "User-Agent: lattice/" + string(LATTICE_VERSION) + "\r\n";
    }
    if (!header.connection()) {
        // Keep-Alive by default
        data += "Connection: keep-alive\r\n";
    }
    if (!header.accept()) {
        // accept everything by default
        data += "Accept: */*\r\n";
    }
    if (!header.cookie()) {
        // give a dummy cookie
        data += "Cookie: fake=fake_value\r\n";
    }
    if (!header.content_type() && is_unicode(parameters)) {
        // parameters must be UTF-8, are added to body
        data += "Content-Type: text/x-www-form-urlencoded; charset=utf-8\r\n";
    }

    return data;
}


/**
 *  Currently used only for digest authentication.
 */
string request_t::method_header(const response_t& response) const
{
    string data = method_header();
    if (digest) {
        try {
            auto str = response.headers().at("www-authenticate");
            digest_challenge_t challenge(str);
            data += challenge.header(url, parameters, digest, response.body(), method_name());
        } catch(exception) {
        }
    }

    return data;
}


void request_t::set_method(method_t method)
{
    this->method = method;
}


/**
 *  \warning HTTP1.1 **requires** a host for the session, so the URL
 *  must be absolute.
 */
void request_t::set_url(const url_t& url)
{
    this->url = url;
    if (this->url .relative()) {
        throw runtime_error("Cannot establish connection with relative URL.");
    }
}


void request_t::set_url(url_t&& url)
{
    this->url = move(url);
    if (this->url .relative()) {
        throw runtime_error("Cannot establish connection with relative URL.");
    }
}


void request_t::set_parameters(const parameters_t& parameters)
{
    this->parameters = parameters;
}


void request_t::set_parameters(parameters_t&& parameters)
{
    this->parameters = move(parameters);
}


void request_t::set_header(const header_t& header)
{
    this->header = header;
}


void request_t::set_timeout(const timeout_t& timeout)
{
    this->timeout = timeout;
}


void request_t::set_auth(const authentication_t& auth)
{
    header["Authorization"] = "Basic " + base64_encode(auth.str());
}


void request_t::set_digest(const digest_t& digest)
{
    this->digest = digest;
}


void request_t::set_proxy(const proxy_t& proxy)
{
    this->proxy = proxy;
}


void request_t::set_proxy(proxy_t&& proxy)
{
    this->proxy = move(proxy);
}


void request_t::set_multipart(const multipart_t& multipart)
{
    this->multipart = multipart;
    if (this->multipart) {
        header["Content-Type"] = this->multipart.header();
    }
}


void request_t::set_multipart(multipart_t&& multipart)
{
    this->multipart = move(multipart);
    if (this->multipart) {
        header["Content-Type"] = this->multipart.header();
    }
}


void request_t::set_body(const body_t& body)
{
    this->parameters = static_cast<parameters_t>(body);
    method = POST;
}


void request_t::set_body(body_t&& body)
{
    this->parameters = move(static_cast<parameters_t>(body));
    method = POST;
}


void request_t::set_payload(const payload_t& payload)
{
    this->parameters = static_cast<parameters_t>(payload);
    method = POST;
}


void request_t::set_payload(payload_t&& payload)
{
    this->parameters = move(static_cast<parameters_t>(payload));
    method = POST;
}



void request_t::set_cookies(const cookies_t& c)
{
    header["Cookie"] = c.encode();
}


void request_t::set_redirects(const redirects_t& redirects)
{
    this->redirects = redirects;
}


void request_t::set_certificate_file(const certificate_file_t& certificate)
{
    this->certificate = certificate;
}


void request_t::set_certificate_file(certificate_file_t&& certificate)
{
    this->certificate = move(certificate);
}


void request_t::set_revocation_lists(const revocation_lists_t& revoke)
{
    this->revoke = revoke;
}


void request_t::set_revocation_lists(revocation_lists_t&& revoke)
{
    this->revoke = move(revoke);
}


void request_t::set_ssl_protocol(ssl_protocol_t ssl)
{
    this->ssl = ssl;
}


void request_t::set_verify_peer(const verify_peer_t& peer)
{
    this->verifypeer = verifypeer;
}


void request_t::set_verify_peer(verify_peer_t&& peer)
{
    this->verifypeer = verifypeer;
}


void request_t::set_cache(const dns_cache_t& cache)
{
    this->cache = cache;
}


void request_t::set_option(method_t method)
{
    this->method = method;
}


void request_t::set_option(const url_t& url)
{
    set_url(url);
}


void request_t::set_option(url_t&& url)
{
    set_url(forward<url_t>(url));
}


void request_t::set_option(const parameters_t& parameters)
{
    this->parameters = parameters;
}


void request_t::set_option(parameters_t&& parameters)
{
    this->parameters = move(parameters);
}


void request_t::set_option(const header_t& header)
{
    this->header = header;
}


void request_t::set_option(const timeout_t& timeout)
{
    this->timeout = timeout;
}


void request_t::set_option(const authentication_t& auth)
{
    header["Authorization"] = "Basic " + base64_encode(auth.str());
}


void request_t::set_option(const digest_t& digest)
{
    this->digest = digest;
}


void request_t::set_option(const proxy_t& proxy)
{
    this->proxy = proxy;
}


void request_t::set_option(proxy_t&& proxy)
{
    this->proxy = move(proxy);
}


void request_t::set_option(const multipart_t& multipart)
{
    set_multipart(multipart);
}


void request_t::set_option(multipart_t&& multipart)
{
    set_multipart(forward<multipart_t>(multipart));
}


void request_t::set_option(const body_t& body)
{
    set_body(body);
}


void request_t::set_option(body_t&& body)
{
    set_body(forward<body_t>(body));
}


void request_t::set_option(const payload_t& payload)
{
    set_payload(payload);
}


void request_t::set_option(payload_t&& payload)
{
    set_payload(forward<payload_t>(payload));
}


void request_t::set_option(const cookies_t& c)
{
    header["Cookie"] = c.encode();
}


void request_t::set_option(const redirects_t& redirects)
{
    this->redirects = redirects;
}


void request_t::set_option(const certificate_file_t& certificate)
{
    this->certificate = certificate;
}


void request_t::set_option(certificate_file_t&& certificate)
{
    this->certificate = move(certificate);
}


void request_t::set_option(const revocation_lists_t& revoke)
{
    this->revoke = revoke;
}


void request_t::set_option(revocation_lists_t&& revoke)
{
    this->revoke = move(revoke);
}


void request_t::set_option(ssl_protocol_t ssl)
{
    this->ssl = ssl;
}


void request_t::set_option(const verify_peer_t& peer)
{
    this->verifypeer = verifypeer;
}


void request_t::set_option(verify_peer_t&& peer)
{
    this->verifypeer = verifypeer;
}


void request_t::set_option(const dns_cache_t& cache)
{
    this->cache = cache;
}


method_t request_t::get_method() const
{
    return method;
}


const url_t& request_t::get_url() const
{
    return url;
}


const parameters_t& request_t::get_parameters() const
{
    return parameters;
}


const header_t& request_t::get_header() const
{
    return header;
}


const timeout_t& request_t::get_timeout() const
{
    return timeout;
}


const digest_t& request_t::get_digest() const
{
    return digest;
}


const redirects_t& request_t::get_redirects() const
{
    return redirects;
}


const certificate_file_t& request_t::get_certificate_file() const
{
    return certificate;
}


const revocation_lists_t& request_t::get_revocation_lists() const
{
    return revoke;
}


ssl_protocol_t request_t::get_ssl_protocol() const
{
    return ssl;
}


const verify_peer_t& request_t::get_verify_peer() const
{
    return verifypeer;
}


const dns_cache_t request_t::get_dns_cache() const
{
    return cache;
}

PYCPP_END_NAMESPACE
