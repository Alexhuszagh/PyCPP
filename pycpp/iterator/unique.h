//  :copyright: (c) Alex Huszagh 2017.
//  :license: MIT,see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Lazily transform iterable to only return unique values.
 *
 *  Creates an input-iterable range from an iterator pair, returning
 *  only unique values from the range. If the underlying iterator
 *  pair is a forward, bidirectional, or random-access iterator, then
 *  no value copies are introduced. If the underlying iterator pair
 *  is an input iterator, then a copy of each iterator `value_type` is
 *  stored for each value traversed. Therefore, at the end of the range,
 *  for input iterators, the range contains a copy of each unique value
 *  in the range.
 *
 *  An input-iterator is used since incrementing each iterator is costly:
 *  the value must be extracted from the iterator, hashed, potentially
 *  copied, and potentially inserted into an `unordered_set`. Furthermore,
 *  supporting multi-passes would require the duplication of an
 *  `unordered_set` with the first item, a similarly costly task. Since
 *  many STL algorithms optimize for forward iterators by using
 *  `std::distance`, ensure we disable this optimization by disabling
 *  multi-pass iteration.
 *
 *  Copying the range is like copying an individual iterator: it does
 *  not reset the internal memo, and therefore only supports a single
 *  pass. You should therefore only call `begin()` once on the range,
 *  since iterators are invalidated as they are incremented, even on
 *  copies of the range. If the underlying iterator is a forward,
 *  bidirectional, or random-access iterator, then you may store
 *  different versions of the range, each of which will support a
 *  full unique range.
 *
 *  \code
 *      using vector = std::vector<int>;
 *      using range = unique_range<typename vector::const_iterator>;
 *      vector v = {1, 2, 3, 1, 4, 2, 5};
 *      range r1(v.begin(), v.end());       // all unique values
 *      range r2(v.begin(), v.end());       // all unique values
 *      range r3 = r1;                      // shared with r1
 *      range r3 = r2;                      // shared with r2
 */

#include <pycpp/iterator/category.h>
#include <functional>
#include <memory>
#include <unordered_set>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

template <
    typename Iterator,
    typename Hash = std::hash<typename std::iterator_traits<Iterator>::value_type>
>
struct unique_iterator;

template <
    typename Iterator,
    typename Hash = std::hash<typename std::iterator_traits<Iterator>::value_type>
>
struct unique_range;

namespace unique_detail
{
// ALIAS
// -----

template <typename Iterator>
using iterator_value_type = typename std::iterator_traits<Iterator>::value_type;

template <typename Iterator>
using range_key_type = typename std::conditional<
    is_input_iterator<Iterator>::value,
    iterator_value_type<Iterator>,
    std::reference_wrapper<const iterator_value_type<Iterator>>
>::type;

template <typename Iterator, typename Hash>
using memo_type = std::unordered_set<range_key_type<Iterator>, Hash>;

}   /* unique_detail */

// DECLARATION
// -----------

/**
 *  \brief Unique adaptor for an iterator.
 *
 *  Stores a pointer to the range object, the underlying iterator,
 *  and a an iterator to the current value in the memo.
 */
template <typename Iterator, typename Hash>
struct unique_iterator:
    std::iterator<
        std::input_iterator_tag,
        typename std::iterator_traits<Iterator>::value_type,
        typename std::iterator_traits<Iterator>::difference_type,
        typename std::iterator_traits<Iterator>::pointer,
        typename std::iterator_traits<Iterator>::reference
    >
{
    static_assert(!is_output_iterator<Iterator>::value, "Cannot have output iterator.");

public:
    // MEMBER TYPES
    // ------------
    using self_t = unique_iterator<Iterator, Hash>;
    using base_t = std::iterator<
        std::input_iterator_tag,
        typename std::iterator_traits<Iterator>::value_type,
        typename std::iterator_traits<Iterator>::difference_type,
        typename std::iterator_traits<Iterator>::pointer,
        typename std::iterator_traits<Iterator>::reference
    >;
    using range_t = unique_range<Iterator, Hash>;
    using memo_t = unique_detail::memo_type<Iterator, Hash>;
    using value_type = unique_detail::iterator_value_type<Iterator>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using typename base_t::difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    unique_iterator(range_t* range = nullptr, Iterator it = Iterator(), memo_t* memo = nullptr);
    unique_iterator(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    unique_iterator(self_t&&) = default;
    self_t& operator=(self_t&&) = default;

    // OPERATORS
    self_t& operator++();
    self_t operator++(int);
    const_pointer operator->() const;
    const_reference operator*() const;

    // OTHER
    void swap(self_t&);

    // RELATIONAL
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;

private:
    template <typename I, typename H>
    friend struct unique_range;

    using memo_iterator = typename memo_t::const_iterator;

    range_t* range_;
    Iterator it_;
    std::shared_ptr<memo_t> memo_;
    memo_iterator mit_;
};


/**
 *  \brief Range adaptor to return only unique elements.
 *
 *  Creates an input iterator begin/end pair from any non-output iterator.
 */
template <typename Iterator, typename Hash>
struct unique_range
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = unique_range<Iterator, Hash>;
    using iterator = unique_iterator<Iterator, Hash>;
    using value_type = typename iterator::value_type;
    using reference = typename iterator::reference;
    using const_reference = typename iterator::const_reference;
    using pointer = typename iterator::pointer;
    using const_pointer = typename iterator::const_pointer;
    using difference_type = typename iterator::difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    unique_range(Iterator first = Iterator(), Iterator last = Iterator());
    unique_range(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    unique_range(self_t&&) = default;
    self_t& operator=(self_t&&) = default;

    // ITERATORS
    iterator begin();
    iterator end();

    // OTHER
    void swap(self_t&);

private:
    template <typename I, typename H>
    friend struct unique_iterator;

    using memo_t = unique_detail::memo_type<Iterator, Hash>;

    void memoize(iterator& first);

    iterator first_;
    iterator last_;
};

// IMPLEMENTATION
// --------------


template <typename Iterator, typename Hash>
unique_iterator<Iterator, Hash>::unique_iterator(range_t* range, Iterator it, memo_t* memo):
    range_(range),
    it_(it),
    memo_(memo)
{}


template <typename Iterator, typename Hash>
auto unique_iterator<Iterator, Hash>::operator++() -> self_t&
{
    ++it_;
    range_->memoize(*this);
    return *this;
}


template <typename Iterator, typename Hash>
auto unique_iterator<Iterator, Hash>::operator++(int) -> self_t
{
    self_t copy(*this);
    operator++();
    return copy;
}


template <typename Iterator, typename Hash>
auto unique_iterator<Iterator, Hash>::operator->() const -> const_pointer
{
    return &operator*();
}


template <typename Iterator, typename Hash>
auto unique_iterator<Iterator, Hash>::operator*() const -> const_reference
{
    return *mit_;
}


template <typename Iterator, typename Hash>
void unique_iterator<Iterator, Hash>::swap(self_t& rhs)
{
    std::swap(range_, rhs.range_);
    std::swap(it_, rhs.it_);
    std::swap(memo_, rhs.memo_);
    std::swap(mit_, rhs.mit_);
}


template <typename Iterator, typename Hash>
bool unique_iterator<Iterator, Hash>::operator==(const self_t& rhs) const
{
    return it_ == rhs.it_;
}


template <typename Iterator, typename Hash>
bool unique_iterator<Iterator, Hash>::operator!=(const self_t& rhs) const
{
    return !operator==(rhs);
}


template <typename Iterator, typename Hash>
unique_range<Iterator, Hash>::unique_range(Iterator first, Iterator last):
    first_(this, first, new memo_t),
    last_(this, last, nullptr)
{
    last_.mit_ = first_.memo_->end();
    memoize(first_);
}


template <typename Iterator, typename Hash>
auto unique_range<Iterator, Hash>::begin() -> iterator
{
    return first_;
}


template <typename Iterator, typename Hash>
auto unique_range<Iterator, Hash>::end() -> iterator
{
    return last_;
}


template <typename Iterator, typename Hash>
void unique_range<Iterator, Hash>::swap(self_t& rhs)
{
    std::swap(first_, rhs.first_);
    std::swap(last_, rhs.last_);
}


template <typename Iterator, typename Hash>
void unique_range<Iterator, Hash>::memoize(iterator& first)
{
    // Memoize value
    // Keep incrementing until first_'s value is not found in the memo.
    while (first.it_ != last_.it_) {
        auto pair = first.memo_->emplace(*first.it_);
        if (pair.second) {
            first.mit_ = pair.first;
            return;
        } else {
            ++first.it_;
        }
    }
    first.mit_ = first.memo_->end();
}

PYCPP_END_NAMESPACE
