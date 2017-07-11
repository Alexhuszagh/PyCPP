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


inline bool is_relative(const std::string &string) noexcept
{
    return string.empty() || string.front() == '/';
}


void puny_encoded_url(url_t& url)
{
    if (url.absolute()) {
        auto names = split(url.host(), ".");
        for (auto &name: names) {
            if (is_unicode(name)) {
                name = "xn--" + utf8_to_punycode(name);
            }
        }
        url.set_host(join(names, "."));
    }
}


// OBJECTS
// -------


url_t::url_t(const char *cstring):
    std::string(cstring)
{
    puny_encoded_url(*this);
}


url_t::url_t(const char *array, size_t size):
    std::string(array, size)
{
    puny_encoded_url(*this);
}


url_t::url_t(const std::string &string):
    std::string(string.data(), string.size())
{
    puny_encoded_url(*this);
}


url_t::url_t(std::initializer_list<char> list):
    std::string(list.begin(), list.size())
{
    puny_encoded_url(*this);
}


std::string url_t::service() const noexcept
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


std::string url_t::host() const noexcept
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


std::string url_t::path() const noexcept
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


std::string url_t::directory() const noexcept
{
    auto data = path();
    const size_t separator = data.find_last_of('/');
    if (separator == std::string::npos) {
        return data.substr(1);
    }

    return data.substr(1, separator - 1);
}


std::string url_t::file() const noexcept
{
    auto data = path();
    const size_t separator = data.find_last_of('/');
    if (separator == std::string::npos) {
        return data;
    }

    return data.substr(separator + 1);
}


void url_t::set_service(const std::string &service)
{
    assert(absolute());
    size_t index = find("://");
    if (index != std::string::npos) {
        // replace service
        replace(0, index, service);
    } else {
        // set a service
        insert(0, service + "://");
    }
}


void url_t::set_host(const std::string &host)
{
    size_t start = find("://");
    if (start == std::string::npos) {
        replace(0, find_first_of('/'), host);
    } else {
        size_t end = find_first_of('/', start+4);
        replace(start+3, end-start-3, host);
    }
}


void url_t::set_path(const std::string &path)
{
    if (relative()) {
        assign(path);
    } else {
        size_t separator = find("://");
        if (separator == std::string::npos) {
            replace(find_first_of('/'), std::string::npos, path);
        } else {
            replace(find_first_of('/', separator+4), std::string::npos, path);
        }
    }
}


void url_t::set_directory(const std::string &directory)
{
    size_t separator, start, end;
    end = find_last_of('/');
    if ((separator = find("://")) != std::string::npos) {
        start = find_first_of('/', separator+4);
    } else {
        start = find_first_of('/');
    }

    if (start != std::string::npos && ++start < end) {
        const size_t length = end - start;
        replace(start, length, directory);
    }
}


void url_t::set_file(const std::string &file)
{
    size_t index = find_last_of('/');
    replace(index + 1, std::string::npos, file);
}



bool url_t::relative() const noexcept
{
    return is_relative(*this);
}


bool url_t::absolute() const noexcept
{
    return !relative();
}


url_t::operator bool() const
{
    return !empty();
}

PYCPP_END_NAMESPACE
