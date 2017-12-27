//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/re/match.h>
#include <pycpp/re/regex.h>
#include <re2/re2.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

static std::string add_group0(const string_wrapper& view)
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
    // MEMBER VARIABLES
    // ----------------
    re2::RE2 re2;
    re2::RE2 sub;
    int argc = 0;
    re2::RE2::Arg* argv = nullptr;
    re2::RE2::Arg** argp = nullptr;
    re2::StringPiece* piece = nullptr;

    // MEMBER TYPES
    // ------------
    using allocator_type = byte_allocator;
    using argv_allocator = typename allocator_traits<allocator_type>::template rebind_alloc<re2::RE2::Arg>;
    using argp_allocator = typename allocator_traits<allocator_type>::template rebind_alloc<re2::RE2::Arg*>;
    using piece_allocator = typename allocator_traits<allocator_type>::template rebind_alloc<re2::StringPiece>;

    // MEMBER FUNCTIONS
    // ----------------
    regex_impl_t(const string_wrapper& view);
    ~regex_impl_t();
    void initialize();
    void clear();
};


regex_impl_t::regex_impl_t(const string_wrapper& view):
    re2(re2::StringPiece(add_group0(view))),
    sub(re2::StringPiece(view.data(), view.size()))
{
    initialize();
}


regex_impl_t::~regex_impl_t()
{
    clear();
}


void regex_impl_t::initialize()
{
    // get allocators
    argv_allocator argv_alloc(re_allocator());
    argp_allocator argp_alloc(re_allocator());
    piece_allocator piece_alloc(re_allocator());

    // allocate memory
    argc = re2.NumberOfCapturingGroups();
    argv = argv_alloc.allocate(argc);
    argp = argp_alloc.allocate(argc);
    piece = piece_alloc.allocate(argc);

    // initialize memory
    for (size_t i = 0; i < argc; ++i) {
        // always have the 0-index piece as the whole group
        new (&piece[i]) re2::StringPiece();
        argv[i] = &piece[i];
        argp[i] = &argv[i];
    }
}


void regex_impl_t::clear()
{
    using piece_traits = allocator_traits<piece_allocator>;

    // get allocators
    argv_allocator argv_alloc(re_allocator());
    argp_allocator argp_alloc(re_allocator());
    piece_allocator piece_alloc(re_allocator());

    if (argc > 0) {
        // destroy memory
        for (size_t i = 0; i < argc; ++i) {
            piece_traits::destroy(piece_alloc, &piece[i]);
        }

        // deallocate memory
        argv_alloc.deallocate(argv, argc);
        argp_alloc.deallocate(argp, argc);
        piece_alloc.deallocate(piece, argc);
    }

    // nullify
    argv = nullptr;
    argp = nullptr;
    piece = nullptr;
    argc = 0;
}


regexp_t::regexp_t(const string_wrapper& view):
    ptr_(deleter_type::create_static(re_allocator(), view), deleter_type(re_allocator()))
{
    if (!ptr_->re2.ok()) {
        throw runtime_error("Invalid regular expression pattern.");
    }
}


regexp_t::regexp_t(regexp_t&& rhs) noexcept:
    ptr_(move(rhs.ptr_))
{}


regexp_t & regexp_t::operator=(regexp_t&& rhs) noexcept
{
    swap(ptr_, rhs.ptr_);
    return *this;
}


regexp_t::~regexp_t() noexcept
{}


match_t regexp_t::search(const string_wrapper& str, size_t pos, size_t endpos)
{
    auto view = str.substr(pos, endpos);
    re2::StringPiece input(view.data(), view.size());

    if (re2::RE2::PartialMatchN(input, ptr_->re2, ptr_->argp, ptr_->argc)) {
        return match_t(*this, str, pos, endpos);
    }

    return match_t();
}


match_t regexp_t::match(const string_wrapper& str, size_t pos, size_t endpos)
{
    auto view = str.substr(pos, endpos);
    re2::StringPiece input(view.data(), view.size());

    if (re2::RE2::FullMatchN(input, ptr_->re2, ptr_->argp, ptr_->argc)) {
        return match_t(*this, str, pos, endpos);
    }

    return match_t();
}


match_groups regexp_t::split(const string_wrapper& str, size_t maxsplit)
{
    match_groups groups;
    size_t pos = 0;
    auto r = finditer(str);
    for (auto &match: r) {
        if (!maxsplit) {
            break;
        }
        groups.emplace_back(str.substr(pos, match.start() - pos));
        pos = match.end();
        --maxsplit;
    }
    groups.emplace_back(str.substr(pos));

    return groups;
}


match_groups regexp_t::findall(const string_wrapper& str, size_t pos, size_t endpos)
{
    match_groups groups;
    for (auto& match: finditer(str, pos, endpos)) {
        groups.emplace_back(match.group());
    }
    return groups;
}


match_range regexp_t::finditer(const string_wrapper& str, size_t pos, size_t endpos)
{
    auto view = str.substr(pos, endpos);
    return match_range(match_iterator_t(*this, view));
}


string regexp_t::sub(const string_wrapper& repl, const string_wrapper& str)
{
    std::string data(str);
    re2::StringPiece repl_(repl.data(), repl.size());
    re2::RE2::GlobalReplace(&data, ptr_->sub, repl_);

    return string(data.data(), data.size());
}


size_t regexp_t::groups() const
{
    // always have extra arg for group0
    return ptr_->argc - 1;
}


const match_group_indexes& regexp_t::group_indexes() const
{
    return ptr_->re2.NamedCapturingGroups();
}


const match_group_names& regexp_t::group_names() const
{
    return ptr_->re2.CapturingGroupNames();
}


PYCPP_END_NAMESPACE
