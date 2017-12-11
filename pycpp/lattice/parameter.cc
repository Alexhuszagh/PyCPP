//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lattice/parameter.h>
#include <pycpp/string/url.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


parameter_t::parameter_t(const char* key, const char* value):
    parameter_t(string_wrapper(key), string_wrapper(value))
{}


parameter_t::parameter_t(std::string&& key, std::string&& value):
    key(std::forward<std::string>(key)),
    value(std::forward<std::string>(value))
{}


parameter_t::parameter_t(const string_wrapper& key, const string_wrapper& value):
    key(key),
    value(value)
{}


parameters_t::parameters_t(std::initializer_list<parameter_t> list)
{
    for (auto it = list.begin(); it != list.end(); ++it) {
        add(*it);
    }
}


parameters_t & parameters_t::add(const parameter_t& parameter)
{
    // add delimiter
    if (!empty()) {
        append("&");
    }

    // add escape values
    if (parameter.value.empty()) {
        append(url_encode(parameter.key));
    } else {
        append(url_encode(parameter.key) + "=" + url_encode(parameter.value));
    }

    return *this;
}


std::string parameters_t::get() const
{
    if (empty()) {
        return static_cast<const std::string&>(*this);
    }
    return "?" + static_cast<const std::string&>(*this);
}


const std::string& parameters_t::post() const
{
    return static_cast<const std::string&>(*this);
}


parameters_t::operator bool() const
{
    return !empty();
}

PYCPP_END_NAMESPACE
