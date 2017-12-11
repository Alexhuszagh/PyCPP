//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/multi_index/container.h>
#include <pycpp/multi_index/hashed_index.h>
#include <pycpp/multi_index/indexed_by.h>
#include <pycpp/multi_index/mem_fun.h>
#include <pycpp/multi_index/sequenced_index.h>
#include <pycpp/multi_index/tag.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/xml/core.h>
#include <pycpp/xml/dom.h>

PYCPP_BEGIN_NAMESPACE

// NOTICE
// The `tostring` is declared in the `dom.cc`
// source file, since this simplifies the definition.

// ALIAS
// -----

struct name_tag {};
struct id_tag {};


typedef multi_index_container<
    xml_node_t,
    indexed_by<
        sequenced<>,
        hashed_non_unique<
            tag<name_tag>,
            const_mem_fun<xml_node_t, const xml_string_t&, &xml_node_t::get_tag>,
            std::hash<xml_string_t>
        >,
        hashed_non_unique<
            tag<id_tag>,
            const_mem_fun<xml_node_t, uintptr_t, &xml_node_t::get_id>,
            std::hash<uintptr_t>
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

xml_node_iterator_t::xml_node_iterator_t()
{}


xml_node_iterator_t::xml_node_iterator_t(const self& other):
    ptr_((void*) new xml_node_iterator_impl_t(*(xml_node_iterator_impl_t*) other.ptr_))
{}


auto xml_node_iterator_t::operator=(const self& other) -> xml_node_iterator_t&
{
    if (this != &other) {
        delete (xml_node_iterator_impl_t*) ptr_;
        ptr_ = (void*) new xml_node_iterator_impl_t(*(xml_node_iterator_impl_t*) other.ptr_);
    }
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


xml_node_iterator_t::~xml_node_iterator_t()
{
    delete (xml_node_iterator_impl_t*) ptr_;
}


xml_node_list_t::xml_node_list_t():
    ptr_(new xml_node_list_impl_t)
{}


xml_node_list_t::xml_node_list_t(const self& other):
    ptr_(new xml_node_list_impl_t)
{
    auto& src = *(const xml_node_list_impl_t*) other.ptr_;
    auto& dst = *(xml_node_list_impl_t*) ptr_;
    for (const xml_node_t& node: src) {
        xml_node_impl_t* ptr = new xml_node_impl_t(*node.ptr_);
        ptr->parent = this;
        dst.emplace_back(xml_node_t(ptr));
    }
}


auto xml_node_list_t::operator=(const self& other) -> self&
{
    if (this != &other) {
        auto& src = *(const xml_node_list_impl_t*) other.ptr_;
        auto& dst = *(xml_node_list_impl_t*) ptr_;
        dst.clear();
        for (const xml_node_t& node: src) {
            xml_node_impl_t* ptr = new xml_node_impl_t(*node.ptr_);
            ptr->parent = this;
            dst.emplace_back(xml_node_t(ptr));
        }
    }

    return *this;
}


xml_node_list_t::xml_node_list_t(self&& other)
{
    swap(other);
}


auto xml_node_list_t::operator=(self&& other) -> self&
{
    swap(other);
    return *this;
}


xml_node_list_t::~xml_node_list_t()
{
    delete (xml_node_list_impl_t*) ptr_;
}


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


xml_string_list_t xml_node_list_t::tostringlist() const
{
    xml_string_list_t list;
    for (const xml_node_t &node: *this) {
        list.emplace_back(node.tostring());
    }

    return list;
}


xml_node_list_t xml_node_list_t::fromstringlist(const xml_string_list_t& strlist)
{
    xml_node_list_t list;
    for (const xml_string_t& str: strlist) {
        list.push_back(xml_node_t::fromstring(str));
    }

    return list;
}


bool xml_node_list_t::empty() const
{
    auto& c = *(const xml_node_list_impl_t*) ptr_;
    return c.empty();
}


size_t xml_node_list_t::size() const
{
    auto& c = *(const xml_node_list_impl_t*) ptr_;
    return c.size();
}


size_t xml_node_list_t::max_size() const
{
    auto& c = *(const xml_node_list_impl_t*) ptr_;
    return c.max_size();
}


auto xml_node_list_t::front() const -> const_reference
{
    auto& c = *(const xml_node_list_impl_t*) ptr_;
    return c.front();
}


auto xml_node_list_t::back() const -> const_reference
{
    auto& c = *(const xml_node_list_impl_t*) ptr_;
    return c.back();
}


auto xml_node_list_t::push_front(const value_type& x) -> std::pair<iterator, bool>
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    auto pair = c.push_front(x);
    if (pair.second) {
        // insertion happened, make sure to parent the item
        pair.first->ptr_->parent = this;
    }

    iterator it;
    it.ptr_ = new xml_node_iterator_impl_t(pair.first);
    return std::make_pair(std::move(it), pair.second);
}


auto xml_node_list_t::push_front(value_type&& x) -> std::pair<iterator, bool>
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    auto pair = c.push_front(std::forward<value_type>(x));
    if (pair.second) {
        // insertion happened, make sure to parent the item
        pair.first->ptr_->parent = this;
    }

    iterator it;
    it.ptr_ = new xml_node_iterator_impl_t(pair.first);
    return std::make_pair(std::move(it), pair.second);
}


void xml_node_list_t::pop_front()
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    auto& node = c.front();
    node.ptr_->parent = nullptr;

    c.pop_front();
}


auto xml_node_list_t::push_back(const value_type& x) -> std::pair<iterator, bool>
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    auto pair = c.push_back(x);
    if (pair.second) {
        // insertion happened, make sure to parent the item
        pair.first->ptr_->parent = this;
    }

    iterator it;
    it.ptr_ = new xml_node_iterator_impl_t(pair.first);
    return std::make_pair(std::move(it), pair.second);
}


auto xml_node_list_t::push_back(value_type&& x) -> std::pair<iterator, bool>
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    auto pair = c.push_back(std::forward<value_type>(x));
    if (pair.second) {
        // insertion happened, make sure to parent the item
        pair.first->ptr_->parent = this;
    }

    iterator it;
    it.ptr_ = new xml_node_iterator_impl_t(pair.first);
    return std::make_pair(std::move(it), pair.second);
}


void xml_node_list_t::pop_back()
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    auto& node = c.back();
    node.ptr_->parent = nullptr;

    c.pop_back();
}


auto xml_node_list_t::insert(iterator position, const value_type& x) -> std::pair<iterator, bool>
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    auto pos = *(xml_node_iterator_impl_t*) position.ptr_;
    auto pair = c.insert(pos, x);
    if (pair.second) {
        // insertion happened, make sure to parent the item
        pair.first->ptr_->parent = this;
    }

    iterator it;
    it.ptr_ = new xml_node_iterator_impl_t(pair.first);
    return std::make_pair(std::move(it), pair.second);
}


auto xml_node_list_t::insert(iterator position, value_type&& x) -> std::pair<iterator, bool>
{
    auto& c = *(xml_node_list_impl_t*) ptr_;
    auto pos = *(xml_node_iterator_impl_t*) position.ptr_;
    auto pair = c.insert(pos, std::forward<value_type>(x));
    if (pair.second) {
        // insertion happened, make sure to parent the item
        pair.first->ptr_->parent = this;
    }

    iterator it;
    it.ptr_ = new xml_node_iterator_impl_t(pair.first);
    return std::make_pair(std::move(it), pair.second);
}


void xml_node_list_t::clear()
{
    auto& c = *(xml_node_list_impl_t*) ptr_;

    // clear the parents
    for (auto it = c.begin(); it != c.end(); ++it) {
        it->ptr_->parent = nullptr;
    }
    c.clear();
}


void xml_node_list_t::swap(self& other)
{
    std::swap(ptr_, other.ptr_);
}


xml_node_t::xml_node_t():
    ptr_(new xml_node_impl_t)
{}


xml_node_t::xml_node_t(xml_node_impl_t*ptr):
    ptr_(ptr)
{}


xml_node_t xml_node_t::fromstring(const xml_string_t& str)
{
    xml_node_t node;
    {
        std::istringstream stream(str);
        xml_stream_reader reader;
        xml_dom_handler handler(node);
        reader.set_handler(handler);
        reader.open(stream);
    }

    // we want the first child, the actual element parsed
    auto& child = *node.begin();
    child.ptr_->parent = nullptr;
    return child;
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


uintptr_t xml_node_t::get_id() const
{
    return (uintptr_t) ptr_.get();
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
        container.modify(it, [&tag](xml_node_t &n) {
            n.ptr_->tag = tag;
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


void xml_node_t::set_tag(xml_string_t&& tag)
{
    if (ptr_->parent) {
        // get an iterator to our element in constant time
        auto &container = *(xml_node_list_impl_t*) ptr_->parent->ptr_;
        auto it = container.iterator_to(*this);
        if (it == container.end()) {
            throw std::runtime_error("Node has no visible parent.");
        }

        // update the underlying container
        container.modify(it, [&tag](xml_node_t &n) {
            std::swap(n.ptr_->tag, tag);
        });

    } else {
        std::swap(ptr_->tag, tag);
    }
}


void xml_node_t::set_text(xml_string_t&& text)
{
    std::swap(ptr_->text, text);
}


void xml_node_t::set_attrs(xml_attr_t&& attrs)
{
    std::swap(ptr_->attrs, attrs);
}


void xml_node_t::set_children(xml_node_list_t&& children)
{
    std::swap(ptr_->children, children);
}


bool xml_node_t::operator==(const self& other) const
{
    return ptr_.get() == other.ptr_.get();
}


bool xml_node_t::operator!=(const self& other) const
{
    return !operator==(other);
}


void xml_node_t::swap(self& other)
{
    std::swap(ptr_, other.ptr_);
}

PYCPP_END_NAMESPACE
