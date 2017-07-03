//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <json/array.h>
#include <json/dom.h>


// OBJECTS
// -------


json_array_t::~json_array_t()
{
    for (auto &item: list_) {
        delete item;
    }
}


auto json_array_t::size() const -> size_type
{
    return list_.size();
}


auto json_array_t::max_size() const -> size_type
{
    return list_.max_size();
}


auto json_array_t::capacity() const -> size_type
{
    return list_.capacity();
}


bool json_array_t::empty() const
{
    return list_.empty();
}


void json_array_t::reserve(size_type n)
{
    list_.reserve(n);
}


void json_array_t::shrink_to_fit()
{
    list_.shrink_to_fit();
}


auto json_array_t::operator[](size_type n) -> reference
{
    return *list_[n];
}


auto json_array_t::operator[](size_type n) const -> const_reference
{
    return *list_[n];
}


auto json_array_t::at(size_type n) -> reference
{
    return *list_.at(n);
}


auto json_array_t::at(size_type n) const -> const_reference
{
    return *list_.at(n);
}


auto json_array_t::front() -> reference
{
    return *list_.front();
}


auto json_array_t::front() const -> const_reference
{
    return *list_.front();
}


auto json_array_t::back() -> reference
{
    return *list_.back();
}


auto json_array_t::back() const -> const_reference
{
    return *list_.back();
}


void json_array_t::push_back(value_type* value)
{
    list_.push_back(value);
}


void json_array_t::pop_back()
{
    auto &item = list_.back();
    delete item;
    list_.pop_back();
}


void json_array_t::clear()
{
    for (auto &item: list_) {
        delete item;
    }
    list_.clear();
}
