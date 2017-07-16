//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/re/match.h>
#include <pycpp/re/regex.h>
#include <re2/re2.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

static std::string add_group0(const string_view& view)
{
    // DESCRIPTION
    // We need to add our "group 0" to the regular expression.
    // Normally, RE2 doesn't store string information unless
    // you **explicitly** provide it.

    // There are two "patches" here: One is the addition of a capture
    // group for the entire string.

    // The other is ".*", which allows trailing characters for
    // `FullMatchN`. This means `FullMatchN` requires specificity
    // at the start of a string, but not at the end.

    std::string str = "(";
    str.append(view.data(), view.size());
    str += ").*";

    return str;
}

// OBJECTS
// -------

/**
 *  \brief Implied base class for the regular expression object.
 */
struct regex_impl_t
{
    re2::RE2 re2;
    size_t argc = 0;
    re2::RE2::Arg* argv = nullptr;
    re2::RE2::Arg** argp = nullptr;
    re2::StringPiece* piece = nullptr;

    regex_impl_t(const string_view& view);
    ~regex_impl_t();
    void initialize();
    void clear();
};


regex_impl_t::regex_impl_t(const string_view& view):
    re2(re2::StringPiece(view.data(), view.size()))
{
    initialize();
}


regex_impl_t::~regex_impl_t()
{
    clear();
}


void regex_impl_t::initialize()
{
    argc = re2.NumberOfCapturingGroups();
    // named capture groups
    argv = new re2::RE2::Arg[argc];
    argp = new re2::RE2::Arg*[argc];
    piece = new re2::StringPiece[argc];

    for (size_t i = 0; i < argc; ++i) {
        // always have the 0-index piece as the whole group
        argv[i] = &piece[i];
        argp[i] = &argv[i];
    }
}


void regex_impl_t::clear()
{
    delete[] argv;
    delete[] argp;
    delete[] piece;
    argv = nullptr;
    argp = nullptr;
    piece = nullptr;
    argc = 0;
}


regexp_t::regexp_t(const string_view& view):
    ptr_(new regex_impl_t(add_group0(view)))
{
    if (!ptr_->re2.ok()) {
        throw std::runtime_error("Invalid regular expression pattern.");
    }
}


regexp_t::regexp_t(regexp_t&& rhs):
    ptr_(std::move(rhs.ptr_))
{}


regexp_t & regexp_t::operator=(regexp_t&& rhs)
{
    std::swap(ptr_, rhs.ptr_);
    return *this;
}


regexp_t::~regexp_t()
{}


match_t regexp_t::search(const string_view& str, size_t pos, size_t endpos)
{
    auto view = str.substr(pos, endpos);
    re2::StringPiece input(view.data(), view.size());

    if (re2::RE2::PartialMatchN(input, ptr_->re2, ptr_->argp, ptr_->argc)) {
        return match_t(*this, str, pos, endpos);
    }

    return match_t();
}


match_t regexp_t::match(const string_view& str, size_t pos, size_t endpos)
{
    auto view = str.substr(pos, endpos);
    re2::StringPiece input(view.data(), view.size());

    if (re2::RE2::FullMatchN(input, ptr_->re2, ptr_->argp, ptr_->argc)) {
        return match_t(*this, str, pos, endpos);
    }

    return match_t();
}


match_groups regexp_t::findall(const string_view& str, size_t pos, size_t endpos)
{
    match_groups groups;
    for (auto& match: finditer(str, pos, endpos)) {
        groups.emplace_back(match.group());
    }
    return groups;
}


match_range regexp_t::finditer(const string_view& str, size_t pos, size_t endpos)
{
    auto view = str.substr(pos, endpos);
    return match_range(match_iterator_t(*this, view));
}


PYCPP_END_NAMESPACE
