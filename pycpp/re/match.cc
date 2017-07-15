//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

// Include the source file so the implied declarations can interact.
#include <pycpp/re/regex.cc>
#include <cassert>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Implied base class for the regular expression match object.
 */
struct match_impl_t
{
    re2::RE2* re2 = nullptr;
    string_view input;
    size_t pos;
    size_t endpos;
    size_t argc = 0;
    string_view *groups = nullptr;
    ~match_impl_t();
};


match_impl_t::~match_impl_t()
{
    delete groups;
}


match_t::match_t(match_t&& rhs):
    ptr_(std::move(rhs.ptr_))
{}


match_t & match_t::operator=(match_t&& rhs)
{
    std::swap(ptr_, rhs.ptr_);
    return *this;
}


match_t::~match_t()
{}


const string_view& match_t::group(size_t index) const
{
    assert(index <= ptr_->argc);
    return ptr_->groups[index];
}


match_groups match_t::groups() const
{
    match_groups list;
    list.reserve(ptr_->argc);
    for (size_t i = 1; i <= ptr_->argc; ++i) {
        list.emplace_back(group(i));
    }

    return list;
}


match_groupdict match_t::groupdict() const
{
    const auto& names = ptr_->re2->CapturingGroupNames();
    match_groupdict dict;
    for (const auto& pair: names) {
        dict[group(pair.first)] = string_view(pair.second);
    }

    return dict;
}


size_t match_t::start(size_t index) const
{
    const string_view& view = group(index);
    return view.data() - ptr_->input.data() - ptr_->pos;
}


size_t match_t::end(size_t index) const
{
    const string_view& view = group(index);
    return view.data() + view.size() - ptr_->input.data() - ptr_->pos;
}


std::pair<size_t, size_t> match_t::span(size_t index) const
{
    return std::make_pair(start(index), end(index));
}


size_t match_t::pos() const
{
    return ptr_->pos;
}


size_t match_t::endpos() const
{
    return ptr_->endpos;
}


size_t match_t::lastindex() const
{
    return ptr_->argc;
}


string_view match_t::lastgroup() const
{
    const auto& names = ptr_->re2->CapturingGroupNames();
    assert(!names.empty());
    return string_view(names.rbegin()->second);
}


const string_view& match_t::string() const
{
    return ptr_->input;
}


match_t::operator bool() const
{
    return bool(ptr_);
}


match_t::match_t()
{}


match_t::match_t(regex_t& regex, const string_view& view, size_t pos, size_t endpos):
    ptr_(new match_impl_t)
{
    ptr_->re2 = &regex.ptr_->re2;
    ptr_->input = view;
    ptr_->pos = pos;
    ptr_->endpos = endpos;
    ptr_->argc = regex.ptr_->argc;
    ptr_->groups = new string_view[ptr_->argc + 1];
    for (size_t i = 0; i < ptr_->argc + 1; ++i) {
        auto piece = regex.ptr_->piece[i];
        ptr_->groups[i] = string_view(piece.data(), piece.size());
    }

    // reset our regular expression
    regex.ptr_->clear();
    regex.ptr_->initialize();
}


PYCPP_END_NAMESPACE
