//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/casemap.h>
#include <pycpp/stringlib.h>
#include <pycpp/unicode.h>
#include <cstring>

PYCPP_BEGIN_NAMESPACE

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


static bool startswith_impl(const string_view& str, const string_view& sub)
{
    return str.substr(0, sub.size()) == sub;
}


static bool endswith_impl(const string_view& str, const string_view& sub)
{
    if (sub.size() > str.size()) {
        return false;
    }
    return str.substr(str.size() - sub.size()) == sub;
}


static void ltrim_impl(string_t& str, const string_view& characters)
{
    auto* ptr = characters.data();
    size_t num = characters.size();
    size_t index = 0;
    while (index < str.size() && memchr(ptr, str[index], num)) {
        ++index;
    }
    str.erase(0, index);
}


static void rtrim_impl(string_t& str, const string_view& characters)
{
    if (str.empty()) {
        return;
    }

    auto* ptr = characters.data();
    size_t num = characters.size();
    size_t index = str.size() - 1;
    while (index >= 0 && memchr(ptr, str[index], num)) {
        --index;
    }
    str.erase(index+1);
}


template <typename Iter, typename IsSplit, typename StoreCb>
static size_t split_impl(Iter first, Iter last, size_t maxsplit, IsSplit is_split, StoreCb store_cb)
{
    size_t length = 0;
    auto interval = first;
    for (; interval != last && maxsplit; ++interval) {
        if (is_split(*interval)) {
            store_cb(first, interval);
            ++length;
            --maxsplit;
            first = interval + 1;

            // last character is a delimiter, add and exit early
            if (first == last) {
                store_cb(first, last);
                return ++length;
            }
        }
    }

    if (first != last) {
        store_cb(first, last);
        ++length;
    }

    return length;
}


template <typename Iter, typename IsSplit, typename StoreCb>
static size_t rsplit_impl(Iter first, Iter last, size_t maxsplit, IsSplit is_split, StoreCb store_cb)
{
    typedef std::reverse_iterator<Iter> RIter;

    RIter rlast(first);
    RIter rfirst(last);

    size_t length = 0;
    auto interval = rfirst;
    for (; interval != rlast && maxsplit; ++interval) {
        if (is_split(*interval)) {
            store_cb(interval.base(), rfirst.base());
            ++length;
            --maxsplit;

            // last character is a delimiter, add and exit early
            rfirst = interval + 1;
            if (rfirst == rlast) {
                store_cb(rlast.base(), rfirst.base());
                return ++length;
            }
        }
    }

    if (rfirst != rlast) {
        store_cb(rlast.base(), rfirst.base());
        ++length;
    }

    return length;
}


template <typename List>
static string_t join_impl(const List& list, const string_view& sep)
{
    string_t string;
    for (const auto &item: list) {
        string.append(item.data(), item.size());
        string.append(sep.data(), sep.size());
    }

    if (!string.empty()) {
        string.erase(string.length()-sep.size());
    }

    return string;
}


static string_t capitalize_impl(const string_t& str)
{
    return utf8_capitalize(str);
}


static string_t lower_impl(const string_t& str)
{
    return utf8_tolower(str);
}


static string_t upper_impl(const string_t& str)
{
    return utf8_toupper(str);
}


string_t replace_impl(const string_view& str, const string_view& sub, const string_view& repl, size_t count)
{
    string_t string;
    string.reserve(str.size());
    for (auto it = str.begin(); it != str.end(); ) {
        auto distance = std::distance(it, str.end());
        if (count && distance >= sub.size() && std::equal(sub.begin(), sub.end(), it)) {
            string.append(repl.data(), repl.size());
            it += sub.size();
            --count;
        } else {
            string.push_back(*it++);
        }
    }

    return string;
}


string_t expandtabs_impl(const string_view& str, size_t tabsize)
{
    std::string sub("\t");
    std::string repl(tabsize, ' ');

    return replace_impl(str, sub, repl, SIZE_MAX);
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
        size_t index = str.substr(start).rfind(sub);
        return index;
    }

    size_t length = end - start;
    size_t index = str.substr(start, length).rfind(sub);
    return index;
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


string_t ltrim(const string_t& str, const string_t& characters)
{
    string_t copy(str);
    ltrim_impl(copy, characters);
    return copy;
}


string_t rtrim(const string_t& str, const string_t& characters)
{
    string_t copy(str);
    rtrim_impl(copy, characters);
    return copy;
}


string_t trim(const string_t& str, const string_t& characters)
{
    string_t copy(str);
    ltrim_impl(copy, characters);
    rtrim_impl(copy, characters);
    return copy;
}


bool startswith(const string_t& str, const string_t& sub)
{
    return startswith_impl(str, sub);
}

bool endswith(const string_t& str, const string_t& sub)
{
    return endswith_impl(str, sub);
}


string_list_t split(const string_t& str, split_function is_split, size_t maxsplit)
{
    typedef typename string_t::const_iterator iter;
    string_list_t data;

    split_impl(str.begin(), str.end(), maxsplit, is_split, [&](iter first, iter second) {
        return data.emplace_back(string_t(first, second));
    });

    return data;
}


string_list_t split(const string_t& str, const string_t& sep, size_t maxsplit)
{
    return split(str, [&](char c) {
        return sep.find(c) != sep.npos;
    }, maxsplit);
}


string_list_t rsplit(const string_t& str, split_function is_split, size_t maxsplit)
{
    typedef typename string_t::const_iterator iter;
    string_list_t data;

    rsplit_impl(str.begin(), str.end(), maxsplit, is_split, [&](iter first, iter second) {
        return data.emplace_back(string_t(first, second));
    });
    std::reverse(data.begin(), data.end());

    return data;
}


string_list_t rsplit(const string_t& str, const string_t& sep, size_t maxsplit)
{
    return rsplit(str, [&](char c) {
        return sep.find(c) != sep.npos;
    }, maxsplit);
}


string_t join(const string_list_t& list, const string_t& sep)
{
    return join_impl(list, string_view(sep));
}



string_t capitalize(const string_t& str)
{
    return capitalize_impl(str);
}


size_t find(const string_t&str, const string_t& sub, size_t start, size_t end)
{
    return find_impl(str, sub, start, end);
}


size_t rfind(const string_t&str, const string_t& sub, size_t start, size_t end)
{
    return rfind_impl(str, sub, start, end);
}


size_t index(const string_t&str, const string_t& sub, size_t start, size_t end)
{
    return index_impl(str, sub, start, end);
}


size_t rindex(const string_t&str, const string_t& sub, size_t start, size_t end)
{
    return rindex_impl(str, sub, start, end);
}


size_t count(const string_t&str, const string_t& sub, size_t start, size_t end)
{
    return count_impl(str, sub, start, end);
}


string_t lower(const string_t& str)
{
    return lower_impl(str);
}


string_t upper(const string_t& str)
{
    return upper_impl(str);
}


string_t replace(const string_t& str, const string_t& sub, const string_t& repl, size_t count)
{
    return replace_impl(string_view(str), string_view(sub), string_view(repl), count);
}


string_t expandtabs(const string_t& str, size_t tabsize)
{
    return expandtabs_impl(string_view(str), tabsize);
}


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


string_wrapper_list_t string_wrapper::split(split_function is_split, size_t maxsplit) const
{
    typedef typename string_wrapper::const_iterator iter;
    string_wrapper_list_t data;

    split_impl(begin(), end(), maxsplit, is_split, [&](iter first, iter second) {
        return data.emplace_back(string_wrapper(first, second));
    });

    return data;
}


string_wrapper_list_t string_wrapper::split(const string_wrapper& sep, size_t maxsplit) const
{
    return split([&](char c) {
        return sep.view().find(c) != sep.npos;
    }, maxsplit);
}


string_wrapper_list_t string_wrapper::rsplit(split_function is_split, size_t maxsplit) const
{
    typedef typename string_wrapper::const_iterator iter;
    string_wrapper_list_t data;

    rsplit_impl(begin(), end(), maxsplit, is_split, [&](iter first, iter second) {
        return data.emplace_back(string_wrapper(first, second));
    });
    std::reverse(data.begin(), data.end());

    return data;
}


string_wrapper_list_t string_wrapper::rsplit(const string_wrapper& sep, size_t maxsplit) const
{
    return rsplit([&](char c) {
        return sep.view().find(c) != sep.npos;
    }, maxsplit);
}


string_t string_wrapper::join(const string_wrapper_list_t& list)
{
    return join_impl(list, *this);
}


string_t string_wrapper::capitalize() const
{
    return capitalize_impl(string_t(*this));
}


string_t string_wrapper::lower() const
{
    return lower_impl(string_t(*this));
}


string_t string_wrapper::upper() const
{
    return upper_impl(string_t(*this));
}


string_t string_wrapper::replace(const string_wrapper& sub, const string_wrapper& repl, size_t count)
{
    return replace_impl(*this, sub, repl, count);
}


string_t string_wrapper::expandtabs(size_t tabsize)
{
    return expandtabs_impl(*this, tabsize);
}


string_t string_wrapper::ltrim(const string_wrapper& characters)
{
    string_t copy(*this);
    ltrim_impl(copy, characters);
    return copy;
}


string_t string_wrapper::rtrim(const string_wrapper& characters)
{
    string_t copy(*this);
    rtrim_impl(copy, characters);
    return copy;
}


string_t string_wrapper::trim(const string_wrapper& characters)
{
    string_t copy(*this);
    ltrim_impl(copy, characters);
    rtrim_impl(copy, characters);
    return copy;
}


bool string_wrapper::startswith(const string_wrapper& sub) const
{
    return startswith_impl(*this, sub);
}


bool string_wrapper::endswith(const string_wrapper& sub) const
{
    return endswith_impl(*this, sub);
}


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

PYCPP_END_NAMESPACE
