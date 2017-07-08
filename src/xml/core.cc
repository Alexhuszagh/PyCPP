//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <xml/core.h>
#include <multi_index_container.hpp>
#include <multi_index/hashed_index.hpp>
#include <multi_index/indexed_by.hpp>
#include <multi_index/mem_fun.hpp>
#include <multi_index/sequenced_index.hpp>
#include <stdexcept>

// ALIAS
// -----

struct name_tag {};


typedef multi_index_container<
    xml_node_t,
    multi_index::indexed_by<
        multi_index::sequenced<>,
        multi_index::hashed_non_unique<
            multi_index::tag<name_tag>,
            multi_index::const_mem_fun<xml_node_t, const xml_string_t&, &xml_node_t::get_tag>,
            std::hash<xml_string_t>
        >
    >
> xml_node_list_impl_t;

using xml_node_iterator_impl_t = typename xml_node_list_impl_t::iterator;


// PRIVATE
// -------


/**
 *  \brief Private implementation for an xml_node_t.
 *
 *  Hide away the interface to allow copying individual items
 *  to preserve the same identifier, and be essentially equivalent
 *  to copying a shared_ptr.
 */
struct xml_node_impl_t
{
    xml_string_t tag;
    xml_string_t text;
    xml_attr_t attrs;
    xml_node_list_t children;
    xml_node_list_t* parent = nullptr;
};

// OBJECTS
// -------


xml_node_iterator_t::xml_node_iterator_t(const self& other):
    ptr_((void*) new xml_node_iterator_impl_t(*(xml_node_iterator_impl_t*) other.ptr_))
{}


auto xml_node_iterator_t::operator=(const self& other) -> xml_node_iterator_t&
{
    delete (xml_node_iterator_impl_t*) ptr_;
    ptr_ = (void*) new xml_node_iterator_impl_t(*(xml_node_iterator_impl_t*) other.ptr_);
    return *this;
}


xml_node_iterator_t::xml_node_iterator_t(self&& other)
{
    swap(other);
}


auto xml_node_iterator_t::operator=(self&& other) -> xml_node_iterator_t&
{
    swap(other);
    return *this;
}


xml_node_list_t::~xml_node_list_t()
{
    delete (xml_node_list_impl_t*) ptr_;
}


bool xml_node_iterator_t::operator==(const self& other) const
{
    auto* l = (const xml_node_iterator_impl_t*) ptr_;
    auto* r = (const xml_node_iterator_impl_t*) other.ptr_;
    return *l == *r;
}


bool xml_node_iterator_t::operator!=(const self& other) const
{
    return !operator==(other);
}


auto xml_node_iterator_t::operator++() -> self&
{
    auto& it = *(xml_node_iterator_impl_t*) ptr_;
    ++it;
    return *this;
}


auto xml_node_iterator_t::operator++(int) -> self
{
    self copy(*this);
    operator++();
    return copy;
}


auto xml_node_iterator_t::operator--() -> self&
{
    auto& it = *(xml_node_iterator_impl_t*) ptr_;
    --it;
    return *this;
}


auto xml_node_iterator_t::operator--(int) -> self
{
    self copy(*this);
    operator--();
    return copy;
}


auto xml_node_iterator_t::operator*() -> reference
{
    auto& it = *(xml_node_iterator_impl_t*) ptr_;
    return const_cast<reference>(*it);
}


auto xml_node_iterator_t::operator*() const -> const_reference
{
    auto& it = *(xml_node_iterator_impl_t*) ptr_;
    return *it;
}


auto xml_node_iterator_t::operator->() -> pointer
{
    auto& it = *(xml_node_iterator_impl_t*) ptr_;
    return const_cast<pointer>(&*it);
}


auto xml_node_iterator_t::operator->() const -> const_pointer
{
    auto& it = *(xml_node_iterator_impl_t*) ptr_;
    return &*it;
}


void xml_node_iterator_t::swap(self& other)
{
    std::swap(ptr_, other.ptr_);
}


xml_node_iterator_t::xml_node_iterator_t()
{}


xml_node_iterator_t::~xml_node_iterator_t()
{
    delete (xml_node_iterator_impl_t*) ptr_;
}


xml_node_list_t::xml_node_list_t():
    ptr_(new xml_node_list_impl_t)
{}


auto xml_node_list_t::begin() -> iterator
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    xml_node_iterator_t it;
    it.ptr_ = new xml_node_iterator_impl_t(c.begin());
    return it;
}


auto xml_node_list_t::begin() const -> const_iterator
{
    return cbegin();
}


auto xml_node_list_t::cbegin() const -> const_iterator
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    xml_node_iterator_t it;
    it.ptr_ = new xml_node_iterator_impl_t(c.begin());
    return it;
}


auto xml_node_list_t::end() -> iterator
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    xml_node_iterator_t it;
    it.ptr_ = new xml_node_iterator_impl_t(c.end());
    return it;
}


auto xml_node_list_t::end() const -> const_iterator
{
    return cend();
}


auto xml_node_list_t::cend() const -> const_iterator
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    xml_node_iterator_t it;
    it.ptr_ = new xml_node_iterator_impl_t(c.end());
    return it;
}


auto xml_node_list_t::rbegin() -> reverse_iterator
{
    return reverse_iterator(end());
}


auto xml_node_list_t::rbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


auto xml_node_list_t::crbegin() const -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


auto xml_node_list_t::rend() -> reverse_iterator
{
    return reverse_iterator(begin());
}


auto xml_node_list_t::rend() const -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


auto xml_node_list_t::crend() const -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


bool xml_node_list_t::operator==(const self& other) const
{
    auto* l = (const xml_node_list_impl_t*) ptr_;
    auto* r = (const xml_node_list_impl_t*) other.ptr_;
    return *l == *r;
}


bool xml_node_list_t::operator!=(const self& other) const
{
    return !operator==(other);
}


auto xml_node_list_t::find(const xml_string_t& tag) const -> iterator
{
    // get iterator to element
    auto& container = *(xml_node_list_impl_t*) ptr_;
    auto& view = container.get<name_tag>();
    auto impl = view.find(tag);
    if (impl == view.end()) {
        return end();
    }

    // wrap iterator to element
    auto& v = *impl;
    iterator it;
    it.ptr_ = new xml_node_iterator_impl_t(container.iterator_to(v));

    return it;
}


auto xml_node_list_t::findall(const xml_string_t& tag) const -> std::pair<iterator, iterator>
{
    auto pair = std::make_pair(iterator(), iterator());

    // get iterators to elements
    auto& c = *(xml_node_list_impl_t*) ptr_;
    auto& v = c.get<name_tag>();
    auto impl = v.equal_range(tag);
    if (impl.first == v.end()) {
        pair.first = end();
        pair.second = end();
    } else if (impl.second == v.end()) {
        pair.first.ptr_ = new xml_node_iterator_impl_t(c.iterator_to(*impl.first));
        pair.second = end();
    } else {
        pair.first.ptr_ = new xml_node_iterator_impl_t(c.iterator_to(*impl.first));
        pair.second.ptr_ = new xml_node_iterator_impl_t(c.iterator_to(*impl.second));
    }

    return pair;
}


void xml_node_list_t::clear()
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    c.clear();
}


void xml_node_list_t::swap(self& other)
{
    std::swap(ptr_, other.ptr_);
}


xml_node_t::xml_node_t():
    ptr_(new xml_node_impl_t)
{}


xml_node_t::xml_node_t(xml_node_list_t& parent):
    ptr_(new xml_node_impl_t)
{
    ptr_->parent = &parent;
}


auto xml_node_t::begin() -> iterator
{
    return get_children().begin();
}


auto xml_node_t::begin() const -> const_iterator
{
    return get_children().begin();
}


auto xml_node_t::cbegin() const -> const_iterator
{
    return get_children().cbegin();
}


auto xml_node_t::end() -> iterator
{
    return get_children().end();
}


auto xml_node_t::end() const -> const_iterator
{
    return get_children().end();
}


auto xml_node_t::cend() const -> const_iterator
{
    return get_children().cend();
}


auto xml_node_t::rbegin() -> reverse_iterator
{
    return get_children().rbegin();
}


auto xml_node_t::rbegin() const -> const_reverse_iterator
{
    return get_children().rbegin();
}


auto xml_node_t::crbegin() const -> const_reverse_iterator
{
    return get_children().crbegin();
}


auto xml_node_t::rend() -> reverse_iterator
{
    return get_children().rend();
}


auto xml_node_t::rend() const -> const_reverse_iterator
{
    return get_children().rend();
}


auto xml_node_t::crend() const -> const_reverse_iterator
{
    return get_children().crend();
}


const xml_string_t& xml_node_t::get_tag() const
{
    return ptr_->tag;
}


const xml_string_t& xml_node_t::get_text() const
{
    return ptr_->text;
}


xml_attr_t& xml_node_t::get_attrs()
{
    return ptr_->attrs;
}


const xml_attr_t& xml_node_t::get_attrs() const
{
    return ptr_->attrs;
}


xml_node_list_t& xml_node_t::get_children()
{
    return ptr_->children;
}


const xml_node_list_t& xml_node_t::get_children() const
{
    return ptr_->children;
}


void xml_node_t::set_tag(const xml_string_t& tag)
{
    if (ptr_->parent) {
        // get an iterator to our element in constant time
        auto &container = *(xml_node_list_impl_t*) ptr_->parent->ptr_;
        auto it = container.iterator_to(*this);
        if (it == container.end()) {
            throw std::runtime_error("Node has no visible parent.");
        }

        // update the underlying container
        container.modify(it, [&tag](xml_node_t &node) {
            node.ptr_->tag = tag;
        });

    } else {
        ptr_->tag = tag;
    }
}


void xml_node_t::set_text(const xml_string_t& text)
{
    ptr_->text = text;
}


void xml_node_t::set_attrs(const xml_attr_t& attrs)
{
    ptr_->attrs = attrs;
}


void xml_node_t::set_children(const xml_node_list_t& children)
{
    ptr_->children = children;
}


bool xml_node_t::operator==(const self& other) const
{
    return ptr_.get() == other.ptr_.get();
}


bool xml_node_t::operator!=(const self& other) const
{
    return !operator==(other);
}
