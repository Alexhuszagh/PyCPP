//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <casemap.h>
#include <stringlib.h>
#include <unicode.h>

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
static size_t split_impl(Iter first, Iter last, size_t maxsplit, IsSep issep, Store store)
{
    size_t length = 0;
    auto interval = first;
    for (; first != last && interval != last && maxsplit; ++interval, --maxsplit) {
        if (issep(*interval)) {
            store(first, interval);
            ++length;
            first = interval + 1;
        }
    }

    if (first != interval) {
        store(first, last);
        ++length;
    }

    return length;
}


template <typename Iter, typename IsSep, typename Store>
static size_t rsplit_impl(Iter first, Iter last, size_t maxsplit, IsSep issep, Store store)
{
    // TODO: here...
//    size_t length = 0;
//    auto interval = first;
//    for (; first != last && interval != last && maxsplit; ++interval, --maxsplit) {
//        if (issep(*interval)) {
//            store(first, interval);
//            ++length;
//            first = interval + 1;
//        }
//    }
//
//    if (first != interval) {
//        store(first, last);
//        ++length;
//    }
//
//    return length;

    return 0;
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
    } else if (end == SIZE_MAX) {
        return str.substr(start).find(sub);
    }

    size_t length = end - start;
    return str.substr(start, length).find(sub);
}


static size_t rfind_impl(const string_view& str, const string_view& sub, size_t start, size_t end)
{
    if (end < start) {
        return -1;
    } else if (end == SIZE_MAX) {
        return str.substr(start).rfind(sub);
    }

    size_t length = end - start;
    return str.substr(start, length).rfind(sub);
}


static size_t index_impl(const string_view& str, const string_view& sub, size_t start, size_t end)
{
    size_t i = find_impl(str, sub, start, end);
    if (i == SIZE_MAX) {
        throw std::out_of_range("Substring not found in wrapper.");
    }
    return i;
}


static size_t rindex_impl(const string_view& str, const string_view& sub, size_t start, size_t end)
{
    size_t i = rfind_impl(str, sub, start, end);
    if (i == SIZE_MAX) {
        throw std::out_of_range("Substring not found in wrapper.");
    }
    return i;
}


static size_t count_impl(const string_view& str, const string_view& sub, size_t start, size_t end)
{
    string_view substr;
    if (end < start) {
        return 0;
    } else if (end == SIZE_MAX) {
        substr = str.substr(start);
    } else {
        substr = str;
    }

    // our extracted substring is too small, cannot contain item
    if (substr.size() < sub.size()) {
        return 0;
    }

    // count elements
    size_t count = 0;
    size_t diff = substr.size() - sub.size();
    for (auto it = str.begin(); it <= str.begin()+diff; ++it) {
        count += std::equal(it, str.end(), sub.begin());
    }

    return count;
}


// FUNCTIONS
// ---------


string_list_t split(const string_t& str, const string_t& sep, size_t maxsplit)
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

    split_impl(str.begin(), str.end(), maxsplit, issep, store);

    return data;
}


string_list_t rsplit(const string_t& str, const string_t& sep, size_t maxsplit)
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

    rsplit_impl(str.begin(), str.end(), maxsplit, issep, store);

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


size_t find(const std::string&str, const std::string& sub, size_t start, size_t end)
{
    return find_impl(str, sub, start, end);
}


size_t rfind(const std::string&str, const std::string& sub, size_t start, size_t end)
{
    return rfind_impl(str, sub, start, end);
}


size_t index(const std::string&str, const std::string& sub, size_t start, size_t end)
{
    return index_impl(str, sub, start, end);
}


size_t rindex(const std::string&str, const std::string& sub, size_t start, size_t end)
{
    return rindex_impl(str, sub, start, end);
}


size_t count(const std::string&str, const std::string& sub, size_t start, size_t end)
{
    return count_impl(str, sub, start, end);
}


//std::string lower(const std::string& str)
//{
//    lower_impl(str);
//}
//
//
//std::string upper(const std::string& str)
//{
//    upper_impl(str);
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


std::vector<string_wrapper> string_wrapper::split(const string_wrapper& sep, size_t maxsplit) const
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

    split_impl(begin(), end(), maxsplit, issep, store);

    return data;
}


std::vector<string_wrapper> string_wrapper::rsplit(const string_wrapper& sep, size_t maxsplit) const
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

    rsplit_impl(begin(), end(), maxsplit, issep, store);

    return data;
}


void string_wrapper::capitalize()
{
    capitalize_impl(begin(), end());
}


//void string_wrapper::lower()
//{
//    lower_impl(*this);
//}

//
//void string_wrapper::upper()
//{
//    upper_impl(*this);
//}


size_t string_wrapper::find(const string_wrapper& sub, size_t start, size_t end) const
{
    return find_impl(*this, sub, start, end);
}


size_t string_wrapper::rfind(const string_wrapper& sub, size_t start, size_t end) const
{
    return rfind_impl(*this, sub, start, end);
}


size_t string_wrapper::index(const string_wrapper& sub, size_t start, size_t end) const
{
    return index_impl(*this, sub, start, end);
}


size_t string_wrapper::rindex(const string_wrapper& sub, size_t start, size_t end) const
{
    return rindex_impl(*this, sub, start, end);
}


size_t string_wrapper::count(const string_wrapper& sub, size_t start, size_t end) const
{
    return count_impl(*this, sub, start, end);
}
