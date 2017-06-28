//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "casemap.h"
#include "stringlib.h"
#include "unicode.h"

// CONSTANTS
// ---------

const string_t ascii_lowercase = "abcdefghijklmnopqrstuvwxyz";
const string_t ascii_uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string_t ascii_letters = ascii_lowercase + ascii_uppercase;
const string_t digits = "0123456789";
const string_t hexdigits = "0123456789abcdefABCDEF";
const string_t lowercase = ascii_lowercase;
const string_t uppercase = ascii_uppercase;
const string_t letters = lowercase + uppercase;
const string_t octdigits = "01234567";
const string_t punctuation = "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~";
const string_t whitespace = " \t\n\r\v\f";
const string_t printable = digits + letters + punctuation + whitespace;

// HELPERS
// -------


template <typename Iter, typename IsSep, typename Store>
static size_t split_impl(Iter first, Iter last, IsSep issep, Store store)
{
    size_t length = 0;
    auto interval = first;
    for (; first != last && interval != last; ++interval) {
        if (issep(*interval)) {
            store(first, interval);
            ++length;
            first = interval + 1;
        }
    }

    if (first != interval) {
        store(first, interval);
        ++length;
    }

    return length;
}


template <typename Iter>
static void capitalize_impl(Iter first, Iter last)
{
    auto it = first;
    if (first == last) {
        return;
    } else {
        if (is_ascii_byte(*first)) {
            *first = ascii_toupper(*first);
        } else {
            // we have a unicode block, find the first
            // non-continuation byte
            ++it;
            it = std::find_if(it, last, [](char c) {
                return !is_continuation_byte(c);
            });

            // get our uint32
            // TODO: here...
        }
    }
}


static size_t find_impl(const string_view& str, const string_view& sub, size_t start, size_t end)
{
    if (end < start) {
        return -1;
    }
    size_t length = end - start;
    return str.substr(start, length).find(sub);
}


// FUNCTIONS
// ---------


string_list_t split(const string_t& str, const string_t& sep)
{
    typedef typename string_t::const_iterator iterator;
    string_list_t data;

    auto issep = [&](char c)
    {
        return sep.find(c) != sep.npos;
    };
    auto store = [&](iterator first, iterator second)
    {
        return data.emplace_back(string_t(first, second));
    };

    split_impl(str.begin(), str.end(), issep, store);

    return data;
}


std::string capitalize(const std::string& str)
{
    std::string output(str);
    capitalize_impl(output.begin(), output.end());

    return output;
}


//std::string expandtabs(const std::string& str, size_t tabsize)
//{
//    // TODO: here...
//    // Need to replace characters...
//}


// OBJECTS
// -------


string_view& string_wrapper::view()
{
    return *this;
}


const string_view& string_wrapper::view() const
{
    return *this;
}


std::vector<string_wrapper> string_wrapper::split(const string_wrapper& sep) const
{
    typedef typename string_wrapper::const_iterator iterator;
    std::vector<string_wrapper> data;

    auto issep = [&](char c)
    {
        return sep.view().find(c) != sep.npos;
    };
    auto store = [&](iterator first, iterator second)
    {
        return data.emplace_back(string_wrapper(first, second));
    };

    split_impl(begin(), end(), issep, store);

    return data;
}


void string_wrapper::capitalize()
{
    capitalize_impl(begin(), end());
}


size_t string_wrapper::find(const string_wrapper& sub, size_t start, size_t end) const
{
    return find_impl(*this, sub, start, end);
}
