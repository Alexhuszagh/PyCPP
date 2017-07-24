//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lattice/response.h>
#include <pycpp/string/casemap.h>
#include <pycpp/string/getline.h>
#include <pycpp/string/string.h>
#include <cctype>
#include <cstring>
#include <sstream>
#include <string>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Parse status code from HTTP response line.
 *
 *  This is valid according to RFC 2616 [Section 6.1][reference].
 *
 *  [reference] http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6
 */
void response_t::parse_code(const std::string &line)
{
    const size_t start = 9;
    size_t end = start;
    while (end < line.size() && isdigit(line[end++]));

    // no code returned, give an ok status (non-standard conforming)
    const int number = start == end ? 200 : std::stoi(line.substr(start, end));
    status_ = static_cast<status_code_t>(number);
}


void response_t::parse_cookie(const std::string &string)
{
    size_t delimiter = string.find_first_of("=");
    size_t end = string.find_first_of(";", delimiter);
    size_t count = end - delimiter;
    cookies_.emplace(string.substr(0, delimiter), string.substr(delimiter, count));
}


void response_t::parse_transfer_encoding(std::string &string)
{
    string = ascii_tolower(string);
    auto encodings = split(string, ",");
    for (auto &encoding: encodings) {
        encoding = trim(encoding);
        if (encoding == "chunked") {
            transfer |= CHUNKED;
        } else if (encoding == "compress") {
            transfer |= COMPRESS;
        } else if (encoding == "deflate") {
            transfer |= DEFLATE;
        } else if (encoding == "gzip") {
            transfer |= GZIP;
        } else if (encoding == "identity") {
            transfer |= IDENTITY;
        }
    }
}


void response_t::parse_content_type(std::string &string)
{
    string = ascii_tolower(string);
    auto values = split(string, ";");
    if (values.size()) {
        // get type, subtype
        auto &front = values.front();
        parse_type(front);
        std::get<1>(mime) = front.substr(front.find("/") + 1);

        // get parameters
        for (auto it = values.begin() + 1; it != values.end(); ++it) {
            if (startswith(*it, "charset")) {
                charset = it->substr(8);
            } else {
                // generalized parameter
                headers_["content-type"] += *it + ";";
            }
        }
    }
}


void response_t::parse_type(const std::string &string)
{
    switch (string.front()) {
        case 'a': {
            std::get<0>(mime) = string[1] == 'p' ? APPLICATION : AUDIO;
            break;
        }
        case 'i': {
            std::get<0>(mime) = IMAGE;
            break;
        }
        case 'm': {
            std::get<0>(mime) = string[1] == 'e' ? MESSAGE : MULTIPART;
            break;
        }
        case 't': {
            std::get<0>(mime) = TEXT;
            break;
        }
        case 'v': {
            std::get<0>(mime) = VIDEO;
            break;
        }
        case 'x': {
            // custom token, must store
            std::get<0>(mime) = XTOKEN;
            headers_["x-token"] += string.substr(2, string.find("/") - 2);
            break;
        }
        default:
            break;
    }
}


/**
 *  The parsed code must, for HTTP/1.1, start with the status code.
 */
void response_t::parse_header_line(const std::string &line)
{
    if (startswith(line, "HTTP/")) {
        // this is valid
        parse_code(line);
    } else {
        // common headers
        size_t colon = line.find_first_of(":");
        std::string key = ascii_tolower(line.substr(0, colon));
        std::string value = line.substr(colon+1);
        value = trim(value);

        if (key == "set-cookie") {
            parse_cookie(value);
        } else if (key == "transfer-encoding") {
            parse_transfer_encoding(value);
        } else if (key == "content-type") {
            parse_content_type(value);
        } else if (key == "authorization") {
            // TODO
        } else {
            // fallthrough
            headers_.emplace(key, value);
        }
    }
}


void response_t::parse_header(const std::string &lines)
{
    std::istringstream stream(lines);
    std::string line;
    while (getline(stream, line)) {
        if (!line.empty()) {
            // data() is used to fix a compiler optimization bug in MSYS2
            parse_header_line(line.data());
        }
    }
}


const int response_t::status() const
{
    return static_cast<int>(status_);
}


const std::string& response_t::body() const
{
    return body_;
}


const header_t& response_t::headers() const
{
    return headers_;
}


const cookies_t& response_t::cookies() const
{
    return cookies_;
}


transfer_encoding_t response_t::transfer_encoding() const
{
    return transfer;
}


std::string response_t::content_encoding() const
{
    auto it = headers().find("content-encoding");
    if (it != headers().end()) {
        return it->second;
    }

    return "";
}


/**
 *  \warning This is necessarily slow, since the RFC 2616 can accept
 *  many different compression formats different content-encodings,
 *  transfer-encodings, or application types.
 */
bool response_t::compressed() const
{
    // fast check if the transer-encoding was set
    auto transit = (
        (transfer & COMPRESS) |
        (transfer & DEFLATE) |
        (transfer & GZIP)
    );
    if (!!transit) {
        return true;
    }

    // check if the content-encoding was set and a transformation applied
    auto it = headers().find("content-encoding");
    if (it != headers().end() && it->second != "identity") {
        return true;
    }

    // check if the mime type is a known compressed format
    auto &format = std::get<1>(mime);
    return (
        format == "x-compress" ||
        format == "x-gzip" ||
        format == "x-bzip" ||
        format == "x-bzip2" ||
        format == "x-7z-compressed" ||
        format == "x-ace-compressed" ||
        format == "x-rar-compressed"
    );
}


/**
 *  \warning Compress is deprecated and should not appear in
 *  real applications.
 */
bool response_t::compress() const
{
    if (!!(transfer & COMPRESS) || std::get<1>(mime) == "x-compress") {
        return true;
    }

    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "compress"));
}


bool response_t::deflate() const
{
    if (!!(transfer & DEFLATE)) {
        return true;
    }

    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "deflate"));
}


bool response_t::exi() const
{
    if (std::get<1>(mime) == "exi") {
        return true;
    }

    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "exi"));
}


/**
 *  Return whether the "Transfer-Encoding", or "Content-Encoding" is
 *  set the gzip.
 */
bool response_t::gzip() const
{
    if (!!(transfer & GZIP) || std::get<1>(mime) == "x-gzip") {
        return true;
    }

    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "gzip"));
}


bool response_t::pack200Gzip() const
{
    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "pack200-gzip"));
}


bool response_t::br() const
{
    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "br"));
}


bool response_t::bzip() const
{
    return std::get<1>(mime) == "x-bzip";
}


bool response_t::bzip2() const
{
    if (std::get<1>(mime) == "x-bzip2") {
        return true;
    }

    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "bzip2"));
}


bool response_t::lzma() const
{
    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "lzma"));
}


bool response_t::peerdist() const
{
    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "peerdist"));
}


bool response_t::xpress() const
{
    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "xpress"));
}


bool response_t::xz() const
{
    auto it = headers().find("content-encoding");
    return ((it != headers().end() && it->second == "xz"));
}


bool response_t::_7z() const
{
    return std::get<1>(mime) == "x-7z-compressed";
}


bool response_t::ace() const
{
    return std::get<1>(mime) == "x-ace-compressed";
}


bool response_t::rar() const
{
    return std::get<1>(mime) == "x-rar-compressed";
}


auto response_t::type() const -> const mime_t&
{
    return mime;
}


bool response_t::application() const
{
    return std::get<0>(mime) == APPLICATION;
}


bool response_t::audio() const
{
    return std::get<0>(mime) == AUDIO;
}


bool response_t::image() const
{
    return std::get<0>(mime) == IMAGE;
}


bool response_t::message() const
{
    return std::get<0>(mime) == MESSAGE;
}


bool response_t::multipart() const
{
    return std::get<0>(mime) == MULTIPART;
}


bool response_t::text() const
{
    return std::get<0>(mime) == TEXT;
}

bool response_t::video() const
{
    return std::get<0>(mime) == VIDEO;
}


bool response_t::xtoken() const
{
    return std::get<0>(mime) == XTOKEN;
}


bool response_t::json() const
{
    return application() && std::get<1>(mime) == "json";
}


bool response_t::xml() const
{
    return application() && std::get<1>(mime) == "xml";
}


std::string response_t::encoding() const
{
    return charset;
}


bool response_t::ok() const
{
    return status_ == status_code_t::OK;
}


bool response_t::unauthorized() const
{
    return status_ == status_code_t::UNAUTHORIZED;
}


/**
 *  Found should **not** automatically redirect except for GET/HEAD
 *  requests.
 */
method_t response_t::redirect(method_t method) const
{
    switch (status_) {
        case status_code_t::MOVED_PERMANENTLY:
            /* fallthrough */
        case status_code_t::TEMPORARY_REDIRECT:
            /* fallthrough */
        case status_code_t::PERMANENT_REDIRECT:
            /* fallthrough */
        case status_code_t::FOUND:
            if (method == GET || method == HEAD) {
                return method;
            }
            return STOP;
        case status_code_t::SEE_OTHER:
            return GET;
            /* fallthrough */
        default:
            return STOP;
    }
}


bool response_t::permanent_redirect() const
{
    return status_ == status_code_t::PERMANENT_REDIRECT;
}


response_t::operator bool() const
{
    return (
        status() ||
        headers() ||
        cookies() ||
        int_t(transfer) ||
        !charset.empty() ||
        !body().empty()
    );
}

PYCPP_END_NAMESPACE
