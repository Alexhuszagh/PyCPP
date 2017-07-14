//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

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
    string_view input;
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


size_t match_t::start(size_t index) const
{
    return group(index).data() - ptr_->input.data();
}


size_t match_t::end(size_t index) const
{
    const string_view& view = group(index);
    return view.data() + view.size() - ptr_->input.data();
}


match_t::operator bool() const
{
    return bool(ptr_);
}


match_t::match_t()
{}


match_t::match_t(regex_t& regex, const string_view& view):
    ptr_(new match_impl_t)
{
    ptr_->input = view;
    ptr_->argc = regex.ptr_->argc;
    ptr_->groups = new string_view[ptr_->argc + 1];
    for (size_t i = 0; i < ptr_->argc + 1; ++i) {
        auto piece = regex.ptr_->piece[i];
        ptr_->groups[i] = string_view(piece.data(), piece.size());
    }
}


PYCPP_END_NAMESPACE
