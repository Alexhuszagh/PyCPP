//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lattice/url.h>
#include <pycpp/string/punycode.h>
#include <pycpp/string/string.h>
#include <pycpp/string/unicode.h>
#include <assert.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


inline bool is_relative(const string_wrapper& str)
{
    return str.empty() || str.front() == '/';
}


inline bool is_punycode(const string_wrapper& str)
{
    return string_wrapper(str).startswith("xn--");
}


static std::string to_idna(const string_wrapper& str)
{
    return "xn--" + utf8_to_punycode(str);
}


static std::string from_idna(const string_wrapper& str)
{
    return punycode_to_utf8(str.substr(4));
}


static void set_service_impl(std::string& url, const string_wrapper& service)
{
    assert(!is_relative(url));
    size_t index = url.find("://");
    if (index != std::string::npos) {
        // replace service
        url.replace(0, index, service.data(), service.size());
    } else {
        // set a service
        std::string str(service);
        str += "://";
        url.insert(0, str);
    }
}


static void set_host_impl(std::string& url, const string_wrapper& host)
{
    size_t start = url.find("://");
    if (start == std::string::npos) {
        url.replace(0, url.find_first_of('/'), host.data(), host.size());
    } else {
        size_t end = url.find_first_of('/', start+4);
        url.replace(start+3, end-start-3, host.data(), host.size());
    }
}


static void set_path_impl(std::string& url, const string_wrapper& path)
{
    if (is_relative(url)) {
        url.assign(path.data(), path.size());
    } else {
        size_t separator = url.find("://");
        if (separator == std::string::npos) {
            url.replace(url.find_first_of('/'), std::string::npos, path.data(), path.size());
        } else {
            url.replace(url.find_first_of('/', separator+4), std::string::npos, path.data(), path.size());
        }
    }
}


static void set_directory_impl(std::string& url, const string_wrapper& directory)
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
        url.replace(start, length, directory.data(), directory.size());
    }
}


static void set_file_impl(std::string& url, const string_wrapper& file)
{
    size_t index = url.find_last_of('/');
    url.replace(index + 1, std::string::npos, file.data(), file.size());
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


punycode_idna_t::punycode_idna_t(const char* p, size_t s):
    punycode_idna_t(string_wrapper(p, s))
{}


punycode_idna_t::punycode_idna_t(const string_wrapper& string):
    url_impl_t(string.data(), string.size())
{
    punycode_encode_url(*this);
}


punycode_idna_t::punycode_idna_t(initializer_list<char> list):
    url_impl_t(list.begin(), list.size())
{
    punycode_encode_url(*this);
}


void punycode_idna_t::set_service(const string_wrapper& service)
{
    set_service_impl(*this, service);
}


void punycode_idna_t::set_host(const string_wrapper& host)
{
    if (is_unicode(host)) {
        set_host_impl(*this, to_idna(host));
    } else {
        set_host_impl(*this, host);
    }
}


void punycode_idna_t::set_path(const string_wrapper& path)
{
    set_path_impl(*this, path);
}


void punycode_idna_t::set_directory(const string_wrapper& directory)
{
    set_directory_impl(*this, directory);
}


void punycode_idna_t::set_file(const string_wrapper& file)
{
    set_file_impl(*this, file);
}


unicode_idna_t punycode_idna_t::to_unicode() const
{
    return unicode_idna_t(string_wrapper(data(), size()));
}


unicode_idna_t::unicode_idna_t(const char* p, size_t s):
    unicode_idna_t(string_wrapper(p, s))
{}


unicode_idna_t::unicode_idna_t(const string_wrapper& string):
    url_impl_t(string.data(), string.size())
{
    unicode_encode_url(*this);
}


unicode_idna_t::unicode_idna_t(initializer_list<char> list):
    url_impl_t(list.begin(), list.size())
{
    unicode_encode_url(*this);
}


void unicode_idna_t::set_service(const string_wrapper& service)
{
    set_service_impl(*this, service);
}


void unicode_idna_t::set_host(const string_wrapper& host)
{
    if (is_punycode(host)) {
        set_host_impl(*this, from_idna(host));
    } else {
        set_host_impl(*this, host);
    }
}


void unicode_idna_t::set_path(const string_wrapper& path)
{
    set_path_impl(*this, path);
}


void unicode_idna_t::set_directory(const string_wrapper& directory)
{
    set_directory_impl(*this, directory);
}


void unicode_idna_t::set_file(const string_wrapper& file)
{
    set_file_impl(*this, file);
}


punycode_idna_t unicode_idna_t::to_punycode() const
{
    return punycode_idna_t(string_wrapper(data(), size()));
}

PYCPP_END_NAMESPACE
