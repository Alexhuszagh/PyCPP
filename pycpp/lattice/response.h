//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief HTTP response object.
 */

#pragma once

#include <pycpp/lattice/cookie.h>
#include <pycpp/lattice/header.h>
#include <pycpp/lattice/method.h>
#include <pycpp/lattice/redirect.h>
#include <pycpp/lattice/transfer.h>
#include <pycpp/lattice/url.h>
#include <pycpp/stl/tuple.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct response_t;

// OBJECTS
// -------


/**
 *  \brief HTTP status codes.
 */
enum status_code_t
{
    // INFORMATIONAL
    CONTINUE                            = 100,
    SWITCH_PROTOCOLS                    = 101,
    PROCESSING                          = 102,

    // SUCCESS
    OK                                  = 200,
    CREATED                             = 201,
    ACCEPTED                            = 202,
    MODIFIED                            = 203,
    NO_CONTENT                          = 204,
    RESET_CONTENT                       = 205,
    PARTIAL_CONTENT                     = 206,
    MULTI_STATUS                        = 207,
    ALREADY_REPORTED                    = 208,
    IM_USED                             = 226,

    // REDIRECTION
    MULTIPLE_CHOICES                    = 300,
    MOVED_PERMANENTLY                   = 301,
    FOUND                               = 302,
    SEE_OTHER                           = 303,
    NOT_MODIFIED                        = 304,
    USE_PROXY                           = 305,
    SWITCH_PROXY                        = 306,
    TEMPORARY_REDIRECT                  = 307,
    PERMANENT_REDIRECT                  = 308,

    // CLIENT ERROR
    BAD_REQUEST                         = 400,
    UNAUTHORIZED                        = 401,
    PAYMENT_REQUIRED                    = 402,
    FORBIDDEN                           = 403,
    NOT_FOUND                           = 404,
    METHOD_NOT_ALLOWED                  = 405,
    NOT_ACCEPTABLE                      = 406,
    PROXY_AUTHENTIFICATION_REQUIRED     = 407,
    REQUEST_TIMEOUT                     = 408,
    CONFLICT                            = 409,
    GONE                                = 410,
    LENGTH_REQUIRED                     = 411,
    PRECONDITION_FAILED                 = 412,
    PAYLOAD_TOO_LARGE                   = 413,
    URI_TOO_LONG                        = 414,
    UNSUPPORTED_MEDIA_TYPE              = 415,
    RANGE_NOT_SATISFIABLE               = 416,
    EXPECTATION_FAILED                  = 417,
    IM_A_TEAPOT                         = 418,
    MISDIRECTED_REQUEST                 = 421,
    UNPROCESSABLE_ENTITY                = 422,
    LOCKED                              = 423,
    FAILED_DEPENDENCY                   = 424,
    UPGRADE_REQUIRED                    = 426,
    PRECONDITION_REQUIRED               = 428,
    TOO_MANY_REQUESTS                   = 429,
    REQUEST_HEADERS_TOO_LARGE           = 431,
    UNAVAILABLE_FOR_LEGAL_REASONS       = 451,

    // SERVER ERROR
    INTERNAL_SERVER_ERROR               = 500,
    NOT_IMPLEMENTED                     = 501,
    BAD_GATEWAY                         = 502,
    SERVICE_UNAVAILABLE                 = 503,
    GATEWAY_TIMEOUT                     = 504,
    HTTP_VERSION_ERROR                  = 505,
    CONTENT_NEGOTIATES                  = 506,
    INSUFFICIENT_STORAGE                = 507,
    LOOP_DETECTED                       = 508,
    NOT_EXTENDED                        = 510,
    AUTHENTIFICATION_REQUIRED           = 511,
};


/**
 *  \brief Registered types for "Content-Types".
 */
enum content_t
{
    APPLICATION     = 0,
    AUDIO           = 1,
    IMAGE           = 2,
    MESSAGE         = 3,
    MULTIPART       = 4,
    TEXT            = 5,
    VIDEO           = 6,
    XTOKEN          = 7,
};


using mime_t = std::tuple<content_t, std::string>;

template <typename T>
using is_response = std::is_same<T, response_t>;

template <typename T>
using disable_if_response = typename std::enable_if<!is_response<T>::value>::type;


/**
 *  \brief Generate response from a request.
 */
struct response_t
{
    response_t() = default;
    response_t(const response_t&) = default;
    response_t & operator=(const response_t&) = default;
    response_t(response_t&&) = default;
    response_t & operator=(response_t&&) = default;

    template <typename Connection, typename = disable_if_response<Connection>>
    response_t(Connection &connection);

    // DATA
    const int status() const;
    const std::string& body() const;
    const header_t& headers() const;
    const cookies_t& cookies() const;

    // DATA ENCODING
    transfer_encoding_t transfer_encoding() const;
    std::string content_encoding() const;

    // COMPRESSION
    bool compressed() const;
    bool compress() const;
    bool deflate() const;
    bool exi() const;
    bool gzip() const;
    bool pack200Gzip() const;
    bool br() const;
    bool bzip() const;
    bool bzip2() const;
    bool lzma() const;
    bool peerdist() const;
    bool xpress() const;
    bool xz() const;

    // COMPRESSED FILES
    bool _7z() const;
    bool ace() const;
    bool rar() const;

    // TYPE
    const mime_t& type() const;
    bool application() const;
    bool audio() const;
    bool image() const;
    bool message() const;
    bool multipart() const;
    bool text() const;
    bool video() const;
    bool xtoken() const;
    bool json() const;
    bool xml() const;
    std::string encoding() const;

    // STATUS
    bool ok() const;
    bool unauthorized() const;
    method_t redirect(method_t method) const;
    bool permanent_redirect() const;

    explicit operator bool() const;

protected:
    status_code_t status_ = static_cast<status_code_t>(0);
    header_t headers_;
    cookies_t cookies_;
    transfer_encoding_t transfer = static_cast<transfer_encoding_t>(0);
    mime_t mime;
    std::string charset;
    std::string body_;

    void parse_code(const string_wrapper& line);
    void parse_cookie(const string_wrapper& string);
    void parse_transfer_encoding(const string_wrapper& string);
    void parse_content_type(const string_wrapper& string);
    void parse_type(const string_wrapper& string);
    void parse_header_line(const string_wrapper& line);
    void parse_header(const string_wrapper& lines);
};


// IMPLEMENTATION
// --------------


/**
 *  Initializes the response, first parsing the headers and parsing
 *  them based on RFC 2616 [Section 4.4][reference].
 *
 *  If the transfer encoding is set, and not identity, assume the data
 *  is chunked, unless the connection was closed.
 *
 *  [reference] https://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4
 */
template <typename Connection, typename>
response_t::response_t(Connection& connection)
{
    parse_header(connection.headers());
    if (!!transfer && !(transfer & IDENTITY)) {
        // connection has the transfer set and is not identity
        body_ = connection.chunked();
    } else if (headers().find("content-length") != headers().end()) {
        body_ = connection.body(std::stol(headers().at("content-length")));
    } else {
        // no content-length or chunked storage, just read
        body_ = connection.read();
    }
}

PYCPP_END_NAMESPACE
