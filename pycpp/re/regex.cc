//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

// Include the source file so the implied declarations can interact.
#include <pycpp/re/match.h>
#include <pycpp/re/regex.h>
#include <re2/re2.h>

PYCPP_BEGIN_NAMESPACE

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
    if (argc) {
        // named capture groups
        argv = new re2::RE2::Arg[argc];
        argp = new re2::RE2::Arg*[argc];
        piece = new re2::StringPiece[argc+1];

        for (size_t i = 0; i < argc; ++i) {
            // always have the 0-index piece as the whole group
            argv[i] = &piece[i+1];
            argp[i] = &argv[i];
        }
    } else {
        // always store 0 index for start/end matching
        piece = new re2::StringPiece[1];
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


regex_t::regex_t(const string_view& view):
    ptr_(new regex_impl_t(view))
{}


regex_t::regex_t(regex_t&& rhs):
    ptr_(std::move(rhs.ptr_))
{}


regex_t & regex_t::operator=(regex_t&& rhs)
{
    std::swap(ptr_, rhs.ptr_);
    return *this;
}


regex_t::~regex_t()
{}


match_t regex_t::search(const string_view& str, size_t start, size_t endpos)
{
    auto view = str.substr(start, endpos);
    re2::StringPiece input(view.data(), view.size());

    // TODO: verify this is accurate...
    if (ptr_->argc) {
        // capturing groups
        if (re2::RE2::PartialMatchN(input, ptr_->re2, ptr_->argp, ptr_->argc)) {
            return match_t(*this, view);
        }
    } else {
        // no capturing groups
        if (re2::RE2::PartialMatch(input, ptr_->re2, ptr_->piece)) {
            return match_t(*this, view);
        }
    }

    return match_t();
}


match_t regex_t::match(const string_view& str, size_t start, size_t endpos)
{
    auto view = str.substr(start, endpos);
    re2::StringPiece input(view.data(), view.size());

    // TODO: verify this is accurate...
    if (ptr_->argc) {
        // capturing groups
        if (re2::RE2::FullMatchN(input, ptr_->re2, ptr_->argp, ptr_->argc)) {
            return match_t(*this, view);
        }
    } else {
        // no capturing groups
        if (re2::RE2::FullMatch(input, ptr_->re2, ptr_->piece)) {
            return match_t(*this, view);
        }
    }

    return match_t();
}


PYCPP_END_NAMESPACE
