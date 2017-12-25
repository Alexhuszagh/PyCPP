//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

// Include the source file so the implied declarations can interact.
#include <pycpp/re/new.h>
#include <pycpp/re/regex.cc>
#include <pycpp/stl/tuple.h>
#include <assert.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Implied base class for the regular expression match object.
 */
struct match_impl_t
{
    re2::RE2* re2 = nullptr;
    string_wrapper input;
    size_t pos;
    size_t endpos;
    size_t argc = 0;
    string_wrapper *groups = nullptr;

    bool operator==(const match_impl_t&) const noexcept;
};


bool match_impl_t::operator==(const match_impl_t& rhs) const noexcept
{
    return tie(re2, input, pos, endpos, argc, groups) == tie(rhs.re2, rhs.input, rhs.pos, rhs.endpos, rhs.argc, rhs.groups);
}


match_t::match_t(match_t&& rhs) noexcept:
    ptr_(move(rhs.ptr_))
{}


match_t & match_t::operator=(match_t&& rhs) noexcept
{
    swap(ptr_, rhs.ptr_);
    return *this;
}


match_t::~match_t() noexcept
{
    // get our string wrapper allocator
    using string_allocator = typename allocator_traits<allocator_type>::template rebind_alloc<string_wrapper>;
    using traits_type = allocator_traits<string_allocator>;

    // delete internal allocated memory
    if (ptr_ && ptr_->argc > 0) {
        string_allocator alloc(ptr_.get_deleter());
        for (size_t i = 0; i < ptr_->argc; ++i) {
            traits_type::destroy(alloc, &ptr_->groups[i]);
        }
        traits_type::deallocate(alloc, ptr_->groups, ptr_->argc);
    }
}


const string_wrapper& match_t::group(size_t index) const noexcept
{
    assert(index < ptr_->argc);
    return ptr_->groups[index];
}


match_groups match_t::groups() const
{
    match_groups list;
    list.reserve(ptr_->argc - 1);
    for (size_t i = 1; i < ptr_->argc; ++i) {
        list.emplace_back(group(i));
    }

    return list;
}


match_groupdict match_t::groupdict() const
{
    const auto& names = ptr_->re2->CapturingGroupNames();
    match_groupdict dict;
    for (const auto& pair: names) {
        dict[group(pair.first)] = string_wrapper(pair.second);
    }

    return dict;
}


size_t match_t::start(size_t index) const noexcept
{
    const string_wrapper& view = group(index);
    return view.data() - ptr_->input.data();
}


size_t match_t::end(size_t index) const noexcept
{
    const string_wrapper& view = group(index);
    return view.data() + view.size() - ptr_->input.data();
}


pair<size_t, size_t> match_t::span(size_t index) const noexcept
{
    return make_pair(start(index), end(index));
}


size_t match_t::pos() const noexcept
{
    return ptr_->pos;
}


size_t match_t::endpos() const noexcept
{
    return ptr_->endpos;
}


size_t match_t::lastindex() const noexcept
{
    // always have extra arg for group0
    return ptr_->argc - 1;
}


string_wrapper match_t::lastgroup() const noexcept
{
    const auto& names = ptr_->re2->CapturingGroupNames();
    assert(!names.empty());
    return string_wrapper(names.rbegin()->second);
}


const string_wrapper& match_t::str() const noexcept
{
    return ptr_->input;
}


bool match_t::operator==(const match_t& rhs) const noexcept
{
    if (*this && rhs) {
        return *ptr_ == *rhs.ptr_;
    }
    return ptr_ == rhs.ptr_;
}


bool match_t::operator!=(const match_t& rhs) const noexcept
{
    return !operator==(rhs);
}


match_t::operator bool() const noexcept
{
    return bool(ptr_);
}


match_t::match_t() noexcept:
    ptr_(nullptr, deleter_type(re_allocator()))
{}


match_t::match_t(regexp_t& regex, const string_wrapper& view, size_t pos, size_t endpos):
    ptr_(deleter_type::create(re_allocator()), deleter_type(re_allocator()))
{
    // get our string wrapper allocator
    using string_allocator = typename allocator_traits<allocator_type>::template rebind_alloc<string_wrapper>;
    using traits_type = allocator_traits<string_allocator>;
    string_allocator alloc(ptr_.get_deleter());

    // initialize dat
    ptr_->re2 = &regex.ptr_->re2;
    ptr_->input = view;
    ptr_->pos = pos;
    ptr_->endpos = endpos;
    ptr_->argc = regex.ptr_->argc;
    ptr_->groups = traits_type::allocate(alloc, ptr_->argc);
    for (size_t i = 0; i < ptr_->argc; ++i) {
        auto piece = regex.ptr_->piece[i];
        traits_type::construct(alloc, &ptr_->groups[i], piece.data(), piece.size());
    }

    // reset our regular expression
    regex.ptr_->clear();
    regex.ptr_->initialize();
}


match_iterator_t::match_iterator_t() noexcept
{}


match_iterator_t::match_iterator_t(regexp_t& regex, const string_wrapper& str):
    match_(allocate_shared<match_t>(re_allocator(), regex.search(str))),
    regex_(&regex),
    str_(str)
{}


match_iterator_t::~match_iterator_t() noexcept
{}


match_iterator_t::match_iterator_t(const self_t& rhs) noexcept:
    match_(rhs.match_),
    regex_(rhs.regex_),
    str_(rhs.str_)
{}


auto match_iterator_t::operator=(const self_t& rhs) noexcept -> self_t&
{
    match_ = rhs.match_;
    regex_ = rhs.regex_;
    str_ = rhs.str_;
    return *this;
}


match_iterator_t::match_iterator_t(self_t&& rhs) noexcept:
    match_(move(rhs.match_)),
    regex_(move(rhs.regex_)),
    str_(move(rhs.str_))
{}


auto match_iterator_t::operator=(self_t&& rhs) noexcept -> self_t&
{
    swap(rhs);
    return *this;
}


auto match_iterator_t::operator*() noexcept -> reference
{
    return *match_;
}


auto match_iterator_t::operator*() const noexcept -> const_reference
{
    return *match_;
}


auto match_iterator_t::operator->() noexcept -> pointer
{
    return &*match_;
}


auto match_iterator_t::operator->() const noexcept -> const_pointer
{
    return &*match_;
}


match_iterator_t& match_iterator_t::operator++()
{
    if (regex_ && match_ && *match_) {
        size_t pos = match_->end();
        match_ = allocate_shared<match_t>(re_allocator(), regex_->search(str_, pos));
        // no more match, reset
        if (!*match_) {
            match_.reset();
            regex_ = nullptr;
            str_ = string_wrapper();
        }
    }

    return *this;
}


match_iterator_t match_iterator_t::operator++(int)
{
    match_iterator_t copy(*this);
    operator++();
    return copy;
}


bool match_iterator_t::operator==(const match_iterator_t& rhs) const noexcept
{
    if (match_ && rhs.match_) {
        return tie(*match_, regex_, str_) == tie(*rhs.match_, rhs.regex_, rhs.str_);
    }
    return match_ == rhs.match_;
}


bool match_iterator_t::operator!=(const match_iterator_t& rhs) const noexcept
{
    return !operator==(rhs);
}


void match_iterator_t::swap(self_t& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    swap(match_, rhs.match_);
    swap(regex_, rhs.regex_);
    swap(str_, rhs.str_);
}

PYCPP_END_NAMESPACE
