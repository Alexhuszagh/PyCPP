//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/punycode.h>
#include <pycpp/stringlib.h>
#include <pycpp/unicode.h>
#include <pycpp/lattice/url.h>
#include <cassert>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


inline bool is_relative(const std::string &str)
{
    return str.empty() || str.front() == '/';
}


inline bool is_punycode(const std::string& str)
{
    return string_wrapper(str).startswith("xn--");
}


static std::string to_idna(const std::string& str)
{
    return "xn--" + utf8_to_punycode(str);
}


static std::string from_idna(const std::string& str)
{
    return punycode_to_utf8(str.substr(4));
}


static void set_service_impl(std::string& url, const std::string& service)
{
    assert(!is_relative(url));
    size_t index = url.find("://");
    if (index != std::string::npos) {
        // replace service
        url.replace(0, index, service);
    } else {
        // set a service
        url.insert(0, service + "://");
    }
}


static void set_host_impl(std::string& url, const std::string& host)
{
    size_t start = url.find("://");
    if (start == std::string::npos) {
        url.replace(0, url.find_first_of('/'), host);
    } else {
        size_t end = url.find_first_of('/', start+4);
        url.replace(start+3, end-start-3, host);
    }
}


static void set_path_impl(std::string& url, const std::string& path)
{
    if (is_relative(url)) {
        url.assign(path);
    } else {
        size_t separator = url.find("://");
        if (separator == std::string::npos) {
            url.replace(url.find_first_of('/'), std::string::npos, path);
        } else {
            url.replace(url.find_first_of('/', separator+4), std::string::npos, path);
        }
    }
}


static void set_directory_impl(std::string& url, const std::string& directory)
{
    size_t separator, start, end;
    end = url.find_last_of('/');
    if ((separator = url.find("://")) != std::string::npos) {
        start = url.find_first_of('/', separator+4);
    } else {
        start = url.find_first_of('/');
    }

    if (start != std::string::npos && ++start < end) {
        const size_t length = end - start;
        url.replace(start, length, directory);
    }
}


static void set_file_impl(std::string& url, const std::string& file)
{
    size_t index = url.find_last_of('/');
    url.replace(index + 1, std::string::npos, file);
}


void punycode_encode_url(punycode_idna_t& url)
{
    if (url.absolute()) {
        auto names = split(url.host(), ".");
        for (auto &name: names) {
            if (is_unicode(name)) {
                name = to_idna(name);
            }
        }
        set_host_impl(url, join(names, "."));
    }
}


void unicode_encode_url(unicode_idna_t& url)
{
    if (url.absolute()) {
        auto names = split(url.host(), ".");
        for (auto &name: names) {
            if (is_punycode(name)) {
                name = from_idna(name);
            }
        }
        set_host_impl(url, join(names, "."));
    }
}


// OBJECTS
// -------



std::string url_impl_t::service() const noexcept
{
    assert(absolute());
    size_t index = find("://");
    if (index == std::string::npos) {
#ifdef PYCPP_HAVE_SSL
        return "https";
#else
        return "http";
#endif
    }
    return substr(0, index).data();
}


std::string url_impl_t::host() const noexcept
{
    assert(absolute());
    size_t start = find("://");
    if (start == std::string::npos) {
        return substr(0, find_first_of('/')).data();
    }
    size_t end = find_first_of('/', start+4);
    if (end == std::string::npos) {
        substr(start+3).data();
    }
    return substr(start+3, end-start-3).data();
}


std::string url_impl_t::path() const noexcept
{
    if (relative()) {
        return *this;
    }

    const size_t separator = find("://");
    size_t start;
    if (separator == std::string::npos) {
        start = find_first_of('/');
    } else {
        start = find_first_of('/', separator+4);
    }
    if (start == std::string::npos) {
        return "/";
    }
    return substr(start).data();
}


std::string url_impl_t::directory() const noexcept
{
    auto data = path();
    const size_t separator = data.find_last_of('/');
    if (separator == std::string::npos) {
        return data.substr(1);
    }

    return data.substr(1, separator - 1);
}


std::string url_impl_t::file() const noexcept
{
    auto data = path();
    const size_t separator = data.find_last_of('/');
    if (separator == std::string::npos) {
        return data;
    }

    return data.substr(separator + 1);
}


bool url_impl_t::relative() const noexcept
{
    return is_relative(*this);
}


bool url_impl_t::absolute() const noexcept
{
    return !relative();
}


url_impl_t::operator bool() const
{
    return !empty();
}


punycode_idna_t::punycode_idna_t(const char *cstring):
    url_impl_t(cstring)
{
    punycode_encode_url(*this);
}


punycode_idna_t::punycode_idna_t(const char *array, size_t size):
    url_impl_t(array, size)
{
    punycode_encode_url(*this);
}


punycode_idna_t::punycode_idna_t(const std::string &string):
    url_impl_t(string.data(), string.size())
{
    punycode_encode_url(*this);
}


punycode_idna_t::punycode_idna_t(std::initializer_list<char> list):
    url_impl_t(list.begin(), list.size())
{
    punycode_encode_url(*this);
}


void punycode_idna_t::set_service(const std::string &service)
{
    set_service_impl(*this, service);
}


void punycode_idna_t::set_host(const std::string &host)
{
    if (is_unicode(host)) {
        set_host_impl(*this, to_idna(host));
    } else {
        set_host_impl(*this, host);
    }
}


void punycode_idna_t::set_path(const std::string &path)
{
    set_path_impl(*this, path);
}


void punycode_idna_t::set_directory(const std::string &directory)
{
    set_directory_impl(*this, directory);
}


void punycode_idna_t::set_file(const std::string &file)
{
    set_file_impl(*this, file);
}


unicode_idna_t punycode_idna_t::to_unicode() const
{
    return unicode_idna_t(data(), size());
}


unicode_idna_t::unicode_idna_t(const char *cstring):
    url_impl_t(cstring)
{
    unicode_encode_url(*this);
}


unicode_idna_t::unicode_idna_t(const char *array, size_t size):
    url_impl_t(array, size)
{
    unicode_encode_url(*this);
}


unicode_idna_t::unicode_idna_t(const std::string &string):
    url_impl_t(string.data(), string.size())
{
    unicode_encode_url(*this);
}


unicode_idna_t::unicode_idna_t(std::initializer_list<char> list):
    url_impl_t(list.begin(), list.size())
{
    unicode_encode_url(*this);
}


void unicode_idna_t::set_service(const std::string &service)
{
    set_service_impl(*this, service);
}


void unicode_idna_t::set_host(const std::string &host)
{
    if (is_punycode(host)) {
        set_host_impl(*this, from_idna(host));
    } else {
        set_host_impl(*this, host);
    }
}


void unicode_idna_t::set_path(const std::string &path)
{
    set_path_impl(*this, path);
}


void unicode_idna_t::set_directory(const std::string &directory)
{
    set_directory_impl(*this, directory);
}


void unicode_idna_t::set_file(const std::string &file)
{
    set_file_impl(*this, file);
}


punycode_idna_t unicode_idna_t::to_punycode() const
{
    return punycode_idna_t(data(), size());
}

PYCPP_END_NAMESPACE
