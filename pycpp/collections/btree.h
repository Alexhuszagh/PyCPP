//  :copyright: (c) 2013 Google Inc. All Rights Reserved.
//  :copyright: Modified (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief B-tree base of STL set and map interfaces.
 *
 *  A btree implementation of the STL set and map interfaces. A btree is
 *  both smaller and faster than STL set/map. The red-black tree
 *  implementation of STL set/map has an overhead of 3 pointers (left,
 *  right and parent) plus the node color information for each stored
 *  value. So a set<int32> consumes 20 bytes for each value stored.
 *  This btree implementation stores multiple values on fixed size nodes
 *  (usually 256 bytes) and doesn't store child pointers for leaf nodes.
 *  The result is that a btree_set<int32> may use much less memory per
 *  stored value. For the random insertion benchmark in btree_test.cc,
 *  a btree_set<int32> with node-size of 256 uses 4.9 bytes per stored
 *  value.
 *
 *  The packing of multiple values on to each node of a btree has another
 *  effect besides better space utilization: better cache locality due
 *  to fewer cache lines being accessed. Better cache locality translates
 *  into faster operations.
 *
 *  CAVEATS
 *
 *  Insertions and deletions on a btree can cause splitting, merging or
 *  rebalancing of btree nodes. And even without these operations,
 *  insertions and deletions on a btree will move values around within
 *  a node. In both cases, the result is that insertions and deletions
 *  can invalidate iterators pointing to values other than the one being
 *  inserted/deleted. This is notably different from STL set/map which
 *  takes care to not invalidate iterators on insert/erase except, of
 *  course, for iterators pointing to the value being erased.  A partial
 *  workaround when erasing is available: erase() returns an iterator
 *  pointing to the item just after the one that was erased (or end() if
 *  none exists).
 *
 *  PERFORMANCE
 *
 *      btree_bench --benchmarks=. 2>&1 | ./benchmarks.awk
 *
 *  Run on pmattis-warp.nyc (4 X 2200 MHz CPUs); 2010/03/04-15:23:06
 *  Benchmark                 STL(ns) B-Tree(ns) @    <size>
 *  --------------------------------------------------------
 *  BM_set_int32_insert        1516      608  +59.89%  <256>    [40.0,  5.2]
 *  BM_set_int32_lookup        1160      414  +64.31%  <256>    [40.0,  5.2]
 *  BM_set_int32_fulllookup     960      410  +57.29%  <256>    [40.0,  4.4]
 *  BM_set_int32_delete        1741      528  +69.67%  <256>    [40.0,  5.2]
 *  BM_set_int32_queueaddrem   3078     1046  +66.02%  <256>    [40.0,  5.5]
 *  BM_set_int32_mixedaddrem   3600     1384  +61.56%  <256>    [40.0,  5.3]
 *  BM_set_int32_fifo           227      113  +50.22%  <256>    [40.0,  4.4]
 *  BM_set_int32_fwditer        158       26  +83.54%  <256>    [40.0,  5.2]
 *  BM_map_int32_insert        1551      636  +58.99%  <256>    [48.0, 10.5]
 *  BM_map_int32_lookup        1200      508  +57.67%  <256>    [48.0, 10.5]
 *  BM_map_int32_fulllookup     989      487  +50.76%  <256>    [48.0,  8.8]
 *  BM_map_int32_delete        1794      628  +64.99%  <256>    [48.0, 10.5]
 *  BM_map_int32_queueaddrem   3189     1266  +60.30%  <256>    [48.0, 11.6]
 *  BM_map_int32_mixedaddrem   3822     1623  +57.54%  <256>    [48.0, 10.9]
 *  BM_map_int32_fifo           151      134  +11.26%  <256>    [48.0,  8.8]
 *  BM_map_int32_fwditer        161       32  +80.12%  <256>    [48.0, 10.5]
 *  BM_set_int64_insert        1546      636  +58.86%  <256>    [40.0, 10.5]
 *  BM_set_int64_lookup        1200      512  +57.33%  <256>    [40.0, 10.5]
 *  BM_set_int64_fulllookup     971      487  +49.85%  <256>    [40.0,  8.8]
 *  BM_set_int64_delete        1745      616  +64.70%  <256>    [40.0, 10.5]
 *  BM_set_int64_queueaddrem   3163     1195  +62.22%  <256>    [40.0, 11.6]
 *  BM_set_int64_mixedaddrem   3760     1564  +58.40%  <256>    [40.0, 10.9]
 *  BM_set_int64_fifo           146      103  +29.45%  <256>    [40.0,  8.8]
 *  BM_set_int64_fwditer        162       31  +80.86%  <256>    [40.0, 10.5]
 *  BM_map_int64_insert        1551      720  +53.58%  <256>    [48.0, 20.7]
 *  BM_map_int64_lookup        1214      612  +49.59%  <256>    [48.0, 20.7]
 *  BM_map_int64_fulllookup     994      592  +40.44%  <256>    [48.0, 17.2]
 *  BM_map_int64_delete        1778      764  +57.03%  <256>    [48.0, 20.7]
 *  BM_map_int64_queueaddrem   3189     1547  +51.49%  <256>    [48.0, 20.9]
 *  BM_map_int64_mixedaddrem   3779     1887  +50.07%  <256>    [48.0, 21.6]
 *  BM_map_int64_fifo           147      145   +1.36%  <256>    [48.0, 17.2]
 *  BM_map_int64_fwditer        162       41  +74.69%  <256>    [48.0, 20.7]
 *  BM_set_string_insert       1989     1966   +1.16%  <256>    [64.0, 44.5]
 *  BM_set_string_lookup       1709     1600   +6.38%  <256>    [64.0, 44.5]
 *  BM_set_string_fulllookup   1573     1529   +2.80%  <256>    [64.0, 35.4]
 *  BM_set_string_delete       2520     1920  +23.81%  <256>    [64.0, 44.5]
 *  BM_set_string_queueaddrem  4706     4309   +8.44%  <256>    [64.0, 48.3]
 *  BM_set_string_mixedaddrem  5080     4654   +8.39%  <256>    [64.0, 46.7]
 *  BM_set_string_fifo          318      512  -61.01%  <256>    [64.0, 35.4]
 *  BM_set_string_fwditer       182       93  +48.90%  <256>    [64.0, 44.5]
 *  BM_map_string_insert       2600     2227  +14.35%  <256>    [72.0, 55.8]
 *  BM_map_string_lookup       2068     1730  +16.34%  <256>    [72.0, 55.8]
 *  BM_map_string_fulllookup   1859     1618  +12.96%  <256>    [72.0, 44.0]
 *  BM_map_string_delete       3168     2080  +34.34%  <256>    [72.0, 55.8]
 *  BM_map_string_queueaddrem  5840     4701  +19.50%  <256>    [72.0, 59.4]
 *  BM_map_string_mixedaddrem  6400     5200  +18.75%  <256>    [72.0, 57.8]
 *  BM_map_string_fifo          398      596  -49.75%  <256>    [72.0, 44.0]
 *  BM_map_string_fwditer       243      113  +53.50%  <256>    [72.0, 55.8]
 */

#pragma once

#include <pycpp/preprocessor/architecture.h>
#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/iostream.h>
#include <pycpp/stl/iterator.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/memory.h>
#include <pycpp/stl/new.h>
#include <pycpp/stl/ostream.h>
#include <pycpp/stl/string.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/utility.h>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <sys/types.h>

PYCPP_BEGIN_NAMESPACE

namespace btree_detail
{
// DETAIL
// ------

// We need a cross-platform ssize_t, which should be the same
// size as size_t. Don't use `ptrdiff_t`, since it
// may be too large on segmented memory models.
#if MEMORY_ARCHITECTURE == 16
    using btree_ssize_t = int16_t;
#elif MEMORY_ARCHITECTURE == 32
    using btree_ssize_t = int32_t;
#elif MEMORY_ARCHITECTURE == 64
    using btree_ssize_t = int64_t;
#elif MEMORY_ARCHITECTURE == 128
    using btree_ssize_t = int128_t;
#else
#   error "Unsupported memory architecture."
#endif

// Types small_ and big_ are promise that sizeof(small_) < sizeof(big_)
typedef char small_;

struct big_
{
    char dummy[2];
};

// Inside a btree method, if we just call swap(), it will choose the
// btree::swap method, which we don't want. And we can't say ::swap
// because then MSVC won't pickup any std::swap() implementations. We
// can't just use std::swap() directly because then we don't get the
// specialization for types outside the std namespace. So the solution
// is to have a special swap helper function whose name doesn't
// collide with other swap functions defined by the btree classes.
template <typename T>
inline void btree_swap_helper(T& a, T& b)
{
    std::swap(a, b);
}

// A helper type used to indicate that a key-compare-to functor has been
// provided. A user can specify a key-compare-to functor by doing:
//
//  struct MyStringComparer
//      : public util::btree::btree_key_compare_to_tag {
//    int operator()(const string& a, const string& b) const {
//      return a.compare(b);
//    }
//  };
//
// Note that the return type is an int and not a bool. There is a
// COMPILE_ASSERT which enforces this return type.
struct btree_key_compare_to_tag
{};

// A helper class that indicates if the Compare parameter is derived from
// btree_key_compare_to_tag.
template <typename Compare>
struct btree_is_key_compare_to: public is_convertible<Compare, btree_key_compare_to_tag>
{};

// A helper class to convert a boolean comparison into a three-way
// "compare-to" comparison that returns a negative value to indicate
// less-than, zero to indicate equality and a positive value to
// indicate greater-than. This helper class is specialized for
// less<string> and greater<string>. The btree_key_compare_to_adapter
// class is provided so that btree users automatically get the more
// efficient compare-to code when using common google string types
// with common comparison functors.
template <typename Compare>
struct btree_key_compare_to_adapter: Compare
{
    btree_key_compare_to_adapter()
    {}

    btree_key_compare_to_adapter(const Compare& c):
        Compare(c)
    {}

    btree_key_compare_to_adapter(const btree_key_compare_to_adapter<Compare>& c):
        Compare(c)
    {}
};


template <>
struct btree_key_compare_to_adapter<less<std::string>>: public btree_key_compare_to_tag
{
    btree_key_compare_to_adapter()
    {}

    btree_key_compare_to_adapter(const less<std::string>&)
    {}

    btree_key_compare_to_adapter(const btree_key_compare_to_adapter<less<std::string>>&)
    {}

    int operator()(const std::string& a, const std::string& b) const
    {
        return a.compare(b);
    }
};


template <>
struct btree_key_compare_to_adapter<greater<std::string>>: public btree_key_compare_to_tag
{
    btree_key_compare_to_adapter()
    {}

    btree_key_compare_to_adapter(const greater<std::string>&)
    {}

    btree_key_compare_to_adapter(const btree_key_compare_to_adapter<greater<std::string>>&)
    {}

    int operator()(const std::string& a, const std::string& b) const
    {
        return b.compare(a);
    }
};


// A helper class that allows a compare-to functor to behave like a plain
// compare functor. This specialization is used when we do not have a
// compare-to functor.
template <typename Key, typename Compare, bool HaveCompareTo>
struct btree_key_comparer
{
    btree_key_comparer()
    {}

    btree_key_comparer(Compare c):
        comp(c)
    {}

    static bool bool_compare(const Compare& comp, const Key& x, const Key& y)
    {
        return comp(x, y);
    }

    bool operator()(const Key& x, const Key& y) const
    {
        return bool_compare(comp, x, y);
    }

    Compare comp;
};

// A specialization of btree_key_comparer when a compare-to functor is
// present. We need a plain (boolean) comparison in some parts of the
// btree code, such as insert-with-hint.
template <typename Key, typename Compare>
struct btree_key_comparer<Key, Compare, true>
{
    btree_key_comparer()
    {}

    btree_key_comparer(Compare c):
        comp(c)
    {}

    static bool bool_compare(const Compare& comp, const Key& x, const Key& y)
    {
        return comp(x, y) < 0;
    }

    bool operator()(const Key& x, const Key& y) const
    {
        return bool_compare(comp, x, y);
    }

    Compare comp;
};


// A helper function to compare to keys using the specified compare
// functor. This dispatches to the appropriate btree_key_comparer
// comparison, depending on whether we have a compare-to functor or not
// (which depends on whether Compare is derived from
// btree_key_compare_to_tag).
template <typename Key, typename Compare>
static bool btree_compare_keys(const Compare& comp, const Key& x, const Key& y)
{
    using key_comparer = btree_key_comparer<Key, Compare, btree_is_key_compare_to<Compare>::value>;
    return key_comparer::bool_compare(comp, x, y);
}


template <
    typename Key,
    typename Compare,
    typename Alloc,
    int TargetNodeSize,
    int ValueSize
>
struct btree_common_params
{
    // If Compare is derived from btree_key_compare_to_tag then use
    // it as the key_compare type. Otherwise, use
    // btree_key_compare_to_adapter<> which will fall-back to
    // Compare if we don't have an appropriate specialization.
    using key_compare = typename conditional<
        btree_is_key_compare_to<Compare>::value,
        Compare,
        btree_key_compare_to_adapter<Compare>
    >::type;
    // A type which indicates if we have a key-compare-to functor or
    // a plain old key-compare functor.
    using is_key_compare_to = btree_is_key_compare_to<key_compare>;
    using allocator_type = Alloc;
    using key_type = Key;
    using size_type = btree_ssize_t;
    using difference_type = ptrdiff_t;

    enum {
        target_node_size = TargetNodeSize,

        // Available space for values.  This is largest for leaf nodes,
        // which has overhead no fewer than two pointers.
        node_value_soace = TargetNodeSize - 2 * sizeof(void*),
    };

    // This is an integral type large enough to hold as many
    // ValueSize-values as will fit a node of TargetNodeSize bytes.
    using node_count_type = typename conditional<
        (node_value_soace / ValueSize) >= 256,
        uint16_t,
        uint8_t
    >::type;
};

// A parameters structure for holding the type parameters for a
// btree_map.
template <
    typename Key,
    typename Data,
    typename Compare,
    typename Alloc,
    int TargetNodeSize
>
struct btree_map_params: public btree_common_params<Key, Compare, Alloc, TargetNodeSize, sizeof(Key) + sizeof(Data)>
{
    using data_type = Data;
    using mapped_type = Data;
    using value_type = pair<const Key, data_type>;
    using mutable_value_type = pair<Key, data_type>;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;

    enum {
        value_size = sizeof(Key) + sizeof(data_type),
    };

    static const Key& key(const value_type& x)
    {
        return x.first;
    }

    static const Key& key(const mutable_value_type& x)
    {
        return x.first;
    }


    static void swap(mutable_value_type* a, mutable_value_type* b)
    {
        btree_swap_helper(a->first, b->first);
        btree_swap_helper(a->second, b->second);
    }
};

// A parameters structure for holding the type parameters for a
// btree_set.
template <typename Key, typename Compare, typename Alloc, int TargetNodeSize>
struct btree_set_params: public btree_common_params<Key, Compare, Alloc, TargetNodeSize,sizeof(Key)>
{
    using data_type = false_type;
    using mapped_type = false_type;
    using value_type = Key;
    using mutable_value_type = value_type;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;

    enum {
        value_size = sizeof(Key),
    };

    static const Key& key(const value_type& x)
    {
        return x;
    }

    static void swap(mutable_value_type* a, mutable_value_type* b)
    {
        btree_swap_helper<mutable_value_type>(*a, *b);
    }
};

// An adapter class that converts a lower-bound compare into an
// upper-bound compare.
template <typename Key, typename Compare>
struct btree_upper_bound_adapter: public Compare
{
    btree_upper_bound_adapter(Compare c):
        Compare(c)
    {}

    bool operator()(const Key& a, const Key& b) const
    {
        return !static_cast<const Compare&>(*this)(b, a);
    }
};

template <typename Key, typename CompareTo>
struct btree_upper_bound_compare_to_adapter: public CompareTo
{
    btree_upper_bound_compare_to_adapter(CompareTo c):
        CompareTo(c)
    {}

    int operator()(const Key& a, const Key& b) const
    {
        return static_cast<const CompareTo&>(*this)(b, a);
    }
};

// Dispatch helper class for using linear search with plain compare.
template <typename K, typename N, typename Compare>
struct btree_linear_search_plain_compare
{
    static int lower_bound(const K& k, const N& n, Compare comp)
    {
        return n.linear_search_plain_compare(k, 0, n.count(), comp);
    }

    static int upper_bound(const K& k, const N& n, Compare comp)
    {
        using upper_compare = btree_upper_bound_adapter<K, Compare>;
        return n.linear_search_plain_compare(k, 0, n.count(), upper_compare(comp));
    }
};

// Dispatch helper class for using linear search with compare-to
template <typename K, typename N, typename CompareTo>
struct btree_linear_search_compare_to
{
    static int lower_bound(const K& k, const N& n, CompareTo comp)
    {
        return n.linear_search_compare_to(k, 0, n.count(), comp);
    }

    static int upper_bound(const K& k, const N& n, CompareTo comp)
    {
        using upper_compare = btree_upper_bound_adapter<K,btree_key_comparer<K, CompareTo, true>>;
        return n.linear_search_plain_compare(k, 0, n.count(), upper_compare(comp));
    }
};

// Dispatch helper class for using binary search with plain compare.
template <typename K, typename N, typename Compare>
struct btree_binary_search_plain_compare
{
    static int lower_bound(const K& k, const N& n, Compare comp)
    {
        return n.binary_search_plain_compare(k, 0, n.count(), comp);
    }

    static int upper_bound(const K& k, const N& n, Compare comp)
    {
        using upper_compare = btree_upper_bound_adapter<K, Compare>;
        return n.binary_search_plain_compare(k, 0, n.count(), upper_compare(comp));
    }
};

// Dispatch helper class for using binary search with compare-to.
template <typename K, typename N, typename CompareTo>
struct btree_binary_search_compare_to
{
    static int lower_bound(const K& k, const N& n, CompareTo comp)
    {
        return n.binary_search_compare_to(k, 0, n.count(), CompareTo());
    }

    static int upper_bound(const K& k, const N& n, CompareTo comp)
    {
        using upper_compare = btree_upper_bound_adapter<K,
        btree_key_comparer<K, CompareTo, true>>;
        return n.linear_search_plain_compare(k, 0, n.count(), upper_compare(comp));
    }
};

// BTREE

// A node in the btree holding. The same node type is used for both internal
// and leaf nodes in the btree, though the nodes are allocated in such a way
// that the children array is only valid in internal nodes.
template <typename Params>
class btree_node
{
public:
    using params_type = Params;
    using self_type = btree_node<Params>;
    using key_type = typename Params::key_type;
    using data_type = typename Params::data_type;
    using value_type = typename Params::value_type;
    using mutable_value_type = typename Params::mutable_value_type;
    using pointer = typename Params::pointer;
    using const_pointer = typename Params::const_pointer;
    using reference = typename Params::reference;
    using const_reference = typename Params::const_reference;
    using key_compare = typename Params::key_compare;
    using size_type = typename Params::size_type;
    using difference_type = typename Params::difference_type;
    // Typedefs for the various types of node searches.
    using linear_search_plain_compare_type = btree_linear_search_plain_compare<key_type, self_type, key_compare>;
    using linear_search_compare_to_type = btree_linear_search_compare_to<key_type, self_type, key_compare>;
    using binary_search_plain_compare_type = btree_binary_search_plain_compare<key_type, self_type, key_compare>;
    using binary_search_compare_to_type = btree_binary_search_compare_to<key_type, self_type, key_compare>;
    // If we have a valid key-compare-to type, use
    // linear_search_compare_to, otherwise use
    // linear_search_plain_compare.
    using linear_search_type = typename conditional<
        Params::is_key_compare_to::value,
        linear_search_compare_to_type,
        linear_search_plain_compare_type
    >::type;
    // If we have a valid key-compare-to type, use
    // binary_search_compare_to, otherwise use
    // binary_search_plain_compare.
    using binary_search_type = typename conditional<
        Params::is_key_compare_to::value,
        binary_search_compare_to_type,
        binary_search_plain_compare_type
    >::type;
    // If the key is an integral or floating point type, use linear
    // search which is faster than binary search for such types.
    // Might be wise to also configure linear search based on
    // node-size.
    using search_type = typename conditional<
        is_integral<key_type>::value || is_floating_point<key_type>::value,
        linear_search_type,
        binary_search_type
    >::type;

    struct base_fields {
        using field_type = typename Params::node_count_type;

        // A boolean indicating whether the node is a leaf or not.
        bool leaf;
        // The position of the node in the node's parent.
        field_type position;
        // The maximum number of values the node can hold.
        field_type max_count;
        // The count of the number of values in the node.
        field_type count;
        // A pointer to the node's parent.
        btree_node* parent;
    };

    enum {
        value_size = params_type::value_size,
        target_node_size = params_type::target_node_size,

        // Compute how many values we can fit onto a leaf node.
        node_target_values = (target_node_size - sizeof(base_fields)) / value_size,
        // We need a minimum of 3 values per internal node in order
        // to perform splitting (1 value for the two nodes involved in
        // the split and 1 value propagated to the parent as the
        // delimiter for the split).
        node_values = node_target_values >= 3 ? node_target_values : 3,

        exact_match = 1 << 30,
        match_mask = exact_match - 1,
    };

    struct leaf_fields : public base_fields
    {
        // The array of values. Only the first count of these values
        // have been constructed and are valid.
        mutable_value_type values[node_values];
    };

    struct internal_fields : public leaf_fields {
        // The array of child pointers. The keys in children_[i] are
        // all less than key(i). The keys in children_[i + 1] are all
        // greater than key(i). There are always count + 1 children.
        btree_node* children[node_values + 1];
    };

    struct root_fields : public internal_fields
    {
        btree_node* rightmost;
        size_type size;
    };

    // Getter/setter for whether this is a leaf node or not. This value
    // doesn't change after the node is created.
    bool leaf() const
    {
        return fields_.leaf;
    }

    // Getter for the position of this node in its parent.
    int position() const
    {
        return fields_.position;
    }

    void set_position(int v)
    {
        fields_.position = v;
    }

    // Getter/setter for the number of values stored in this node.
    int count() const
    {
        return fields_.count;
    }

    void set_count(int v)
    {
        fields_.count = v;
    }

    int max_count() const
    {
        return fields_.max_count;
    }

    // Getter for the parent of this node.
    btree_node* parent() const
    {
        return fields_.parent;
    }

    // Getter for whether the node is the root of the tree. The parent
    // of the root of the tree is the leftmost node in the tree which
    // is guaranteed to be a leaf.
    bool is_root() const
    {
        return parent()->leaf();
    }

    void make_root()
    {
        assert(parent()->is_root());
        fields_.parent = fields_.parent->parent();
    }

    // Getter for the rightmost root node field. Only valid on the root
    // node.
    btree_node* rightmost() const
    {
        return fields_.rightmost;
    }

    btree_node** mutable_rightmost()
    {
        return &fields_.rightmost;
    }

    // Getter for the size root node field. Only valid on the root node.
    size_type size() const
    {
        return fields_.size;
    }

    size_type* mutable_size()
    {
        return &fields_.size;
    }

    // Getters for the key/value at position i in the node.
    const key_type& key(int i) const
    {
        return params_type::key(fields_.values[i]);
    }

    reference value(int i)
    {
        return reinterpret_cast<reference>(fields_.values[i]);
    }

    const_reference value(int i) const
    {
        return reinterpret_cast<const_reference>(fields_.values[i]);
    }

    mutable_value_type* mutable_value(int i)
    {
        return &fields_.values[i];
    }

    // Swap value i in this node with value j in node x.
    void value_swap(int i, btree_node* x, int j)
    {
        params_type::swap(mutable_value(i), x->mutable_value(j));
    }

    // Getters/setter for the child at position i in the node.
    btree_node* child(int i) const
    {
        return fields_.children[i];
    }

    btree_node** mutable_child(int i)
    {
        return &fields_.children[i];
    }

    void set_child(int i, btree_node* c)
    {
        *mutable_child(i) = c;
        c->fields_.parent = this;
        c->fields_.position = i;
    }

    // Returns the position of the first value whose key is not less
    // than k.
    template <typename Compare>
    int lower_bound(const key_type& k, const Compare& comp) const
    {
        return search_type::lower_bound(k, *this, comp);
    }

    // Returns the position of the first value whose key is greater
    // than k.
    template <typename Compare>
    int upper_bound(const key_type& k, const Compare& comp) const
    {
        return search_type::upper_bound(k, *this, comp);
    }

    // Returns the position of the first value whose key is not less
    // than k using linear search performed using plain compare.
    template <typename Compare>
    int linear_search_plain_compare(const key_type& k, int s, int e, const Compare& comp) const
    {
        while (s < e) {
            if (!btree_compare_keys(comp, key(s), k)) {
                break;
            }
            ++s;
        }
        return s;
    }

    // Returns the position of the first value whose key is not less
    // than k using linear search performed using compare-to.
    template <typename Compare>
    int linear_search_compare_to(const key_type& k, int s, int e, const Compare& comp) const
    {
        while (s < e) {
            int c = comp(key(s), k);
            if (c == 0) {
                return s | exact_match;
            } else if (c > 0) {
                break;
            }
            ++s;
        }
        return s;
    }

    // Returns the position of the first value whose key is not less
    // than k using binary search performed using plain compare.
    template <typename Compare>
    int binary_search_plain_compare(const key_type& k, int s, int e, const Compare& comp) const
    {
        while (s != e) {
            int mid = (s + e) / 2;
            if (btree_compare_keys(comp, key(mid), k)) {
                s = mid + 1;
            } else {
                e = mid;
            }
        }
        return s;
    }

    // Returns the position of the first value whose key is not less
    // than k using binary search performed using compare-to.
    template <typename CompareTo>
    int binary_search_compare_to(const key_type& k, int s, int e, const CompareTo& comp) const
    {
        while (s != e) {
            int mid = (s + e) / 2;
            int c = comp(key(mid), k);
            if (c < 0) {
                s = mid + 1;
            } else if (c > 0) {
                e = mid;
            } else {
                // Need to return the first value whose key is not less
                // than k, which requires continuing the binary search.
                // Note that we are guaranteed that the result is an
                // exact match because if "key(mid-1) < k" the call to
                // binary_search_compare_to() will return "mid".
                s = binary_search_compare_to(k, s, mid, comp);
                return s | exact_match;
            }
        }
        return s;
    }

    // Inserts the value x at position i, shifting all existing values
    // and children at positions >= i to the right by 1.
    template <typename ... Ts>
    void insert_value(int i, Ts&&... ts);

    // Removes the value at position i, shifting all existing values
    // and children at positions > i to the left by 1.
    void remove_value(int i);

    // Rebalances a node with its right sibling.
    void rebalance_right_to_left(btree_node* sibling, int to_move);
    void rebalance_left_to_right(btree_node* sibling, int to_move);

    // Splits a node, moving a portion of the node's values to its
    // right sibling.
    void split(btree_node* sibling, int insert_position);

    // Merges a node with its right sibling, moving all of the values
    // and the delimiting key in the parent node onto itself.
    void merge(btree_node* sibling);

    // Swap the contents of "this" and "src".
    void swap(btree_node* src);

    // Node allocation/deletion routines.
    static btree_node* init_leaf(leaf_fields* f, btree_node* parent, int max_count)
    {
        btree_node* n = reinterpret_cast<btree_node*>(f);
        f->leaf = 1;
        f->position = 0;
        f->max_count = max_count;
        f->count = 0;
        f->parent = parent;
#ifndef NDEBUG
        memset(&f->values, 0, max_count * sizeof(value_type));
#endif
        return n;
    }

    static btree_node* init_internal(internal_fields* f, btree_node* parent)
    {
        btree_node* n = init_leaf(f, parent, node_values);
        f->leaf = 0;
#ifndef NDEBUG
        memset(f->children, 0, sizeof(f->children));
#endif
        return n;
    }

    static btree_node* init_root(root_fields* f, btree_node* parent)
    {
        btree_node* n = init_internal(f, parent);
        f->rightmost = parent;
        f->size = parent->count();
        return n;
    }

    void destroy()
    {
        for (int i = 0; i < count(); ++i) {
            value_destroy(i);
        }
    }

private:
    template <typename ... Ts>
    void value_init(int i, Ts&&... ts)
    {
        new (&fields_.values[i]) mutable_value_type(std::forward<Ts>(ts)...);
    }

    void value_destroy(int i)
    {
        fields_.values[i].~mutable_value_type();
    }

    root_fields fields_;

    btree_node(const btree_node&);
    void operator=(const btree_node&);
};


template <typename Node, typename Reference, typename Pointer>
struct btree_iterator
{
    using key_type = typename Node::key_type;
    using size_type = typename Node::size_type;
    using difference_type = typename Node::difference_type;
    using params_type = typename Node::params_type;

    using node_type = Node;
    using normal_node = typename remove_const<Node>::type;
    using const_node = const Node;
    using value_type = typename params_type::value_type;
    using normal_pointer = typename params_type::pointer;
    using normal_reference = typename params_type::reference;
    using const_pointer = typename params_type::const_pointer;
    using const_reference = typename params_type::const_reference;

    using pointer = Pointer;
    using reference = Reference;
    using iterator_category = bidirectional_iterator_tag;

    using iterator = btree_iterator<normal_node, normal_reference, normal_pointer>;
    using const_iterator = btree_iterator<const_node, const_reference, const_pointer>;
    using self_type = btree_iterator<Node, Reference, Pointer>;

    btree_iterator():
        node(nullptr),
        position(-1)
    {}

    btree_iterator(Node* n, int p):
        node(n),
        position(p)
    {}

    btree_iterator(const iterator& x):
        node(x.node),
        position(x.position)
    {}

    // Increment/decrement the iterator.
    void increment()
    {
        if (node->leaf() && ++position < node->count()) {
            return;
        }
        increment_slow();
    }

    void increment_by(int count);
    void increment_slow();

    void decrement()
    {
        if (node->leaf() && --position >= 0) {
            return;
        }
        decrement_slow();
    }

    void decrement_slow();

    bool operator==(const const_iterator& x) const
    {
        return node == x.node && position == x.position;
    }

    bool operator!=(const const_iterator& x) const
    {
        return node != x.node || position != x.position;
    }

    // Accessors for the key/value the iterator is pointing at.
    const key_type& key() const
    {
        return node->key(position);
    }

    reference operator*() const
    {
        return node->value(position);
    }

    pointer operator->() const
    {
        return &node->value(position);
    }

    self_type& operator++()
    {
        increment();
        return *this;
    }

    self_type& operator--()
    {
        decrement();
        return *this;
    }

    self_type operator++(int)
    {
        self_type tmp = *this;
        ++*this;
        return tmp;
    }

    self_type operator--(int)
    {
        self_type tmp = *this;
        --*this;
        return tmp;
    }

    // The node in the tree the iterator is pointing at.
    Node* node;
    // The position within the node of the tree the iterator is pointing at.
    int position;
};


// Dispatch helper class for using btree::internal_locate
// with plain compare.
struct btree_internal_locate_plain_compare
{
    template <typename K, typename T, typename Iter>
    static pair<Iter, int> dispatch(const K& k, const T& t, Iter iter)
    {
        return t.internal_locate_plain_compare(k, iter);
    }
};

// Dispatch helper class for using btree::internal_locate
// with compare-to.
struct btree_internal_locate_compare_to
{
    template <typename K, typename T, typename Iter>
    static pair<Iter, int> dispatch(const K& k, const T& t, Iter iter)
    {
        return t.internal_locate_compare_to(k, iter);
    }
};


template <typename Params>
class btree : public Params::key_compare
{
    using self_type = btree<Params>;
    using node_type = btree_node<Params>;
    using base_fields = typename node_type::base_fields;
    using leaf_fields = typename node_type::leaf_fields;
    using internal_fields = typename node_type::internal_fields;
    using root_fields = typename node_type::root_fields;
    using is_key_compare_to = typename Params::is_key_compare_to;

    friend class btree_internal_locate_plain_compare;
    friend class btree_internal_locate_compare_to;

    using internal_locate_type = typename conditional<
        is_key_compare_to::value,
        btree_internal_locate_compare_to,
        btree_internal_locate_plain_compare
    >::type;

    enum {
        node_values = node_type::node_values,
        min_node_values = node_values / 2,
        value_size = node_type::value_size,
        exact_match = node_type::exact_match,
        match_mask = node_type::match_mask,
    };

    // A helper class to get the empty base class optimization for
    // 0-size allocators. Base is internal_allocator_type.
    // (e.g. empty_base_handle<internal_allocator_type, node_type*>).
    // If Base is 0-size, the compiler doesn't have to reserve any
    // space for it and sizeof(empty_base_handle) will simply be
    // sizeof(Data). Google [empty base class optimization] for more
    // details.
    template <typename Base, typename Data>
    struct empty_base_handle: public Base
    {
        empty_base_handle(const Base& b, const Data& d):
            Base(b),
            data(d)
        {}

        Data data;
    };

    struct node_stats
    {
        node_stats(btree_ssize_t l, btree_ssize_t i):
            leaf_nodes(l),
            internal_nodes(i)
        {}

        node_stats& operator+=(const node_stats& x)
        {
            leaf_nodes += x.leaf_nodes;
            internal_nodes += x.internal_nodes;
            return *this;
        }

        btree_ssize_t leaf_nodes;
        btree_ssize_t internal_nodes;
    };

public:
    using params_type = Params;
    using key_type = typename Params::key_type;
    using data_type = typename Params::data_type;
    using mapped_type = typename Params::mapped_type;
    using value_type = typename Params::value_type;
    using mutable_value_type = typename Params::mutable_value_type;
    using key_compare = typename Params::key_compare;
    using pointer = typename Params::pointer;
    using const_pointer = typename Params::const_pointer;
    using reference = typename Params::reference;
    using const_reference = typename Params::const_reference;
    using size_type = typename Params::size_type;
    using difference_type = typename Params::difference_type;
    using iterator = btree_iterator<node_type, reference, pointer>;
    using const_iterator = typename iterator::const_iterator;
    using const_reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<const_iterator>;
    using reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<iterator>;

    using allocator_type = typename Params::allocator_type;
    using internal_allocator_type = typename allocator_traits<allocator_type>::template rebind_alloc<char>;

    // Default constructor.
    btree(const allocator_type& alloc);
    btree(const key_compare& comp, const allocator_type& alloc);

    // Copy constructor.
    btree(const self_type& x);
    btree(const self_type& x, const allocator_type& alloc);

    // Move constructor.
    btree(self_type&& x);
    btree(self_type&& x, const allocator_type& alloc);

    // Destructor.
    ~btree()
    {
        clear();
    }

    // Iterator routines.
    iterator begin()
    {
        return iterator(leftmost(), 0);
    }

    const_iterator begin() const
    {
        return const_iterator(leftmost(), 0);
    }

    iterator end()
    {
        return iterator(rightmost(), rightmost() ? rightmost()->count() : 0);
    }

    const_iterator end() const
    {
        return const_iterator(rightmost(), rightmost() ? rightmost()->count() : 0);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    // Finds the first element whose key is not less than key.
    iterator lower_bound(const key_type& key)
    {
        return internal_end(internal_lower_bound(key, iterator(root(), 0)));
    }

    const_iterator lower_bound(const key_type& key) const
    {
        return internal_end(internal_lower_bound(key, const_iterator(root(), 0)));
    }

    // Finds the first element whose key is greater than key.
    iterator upper_bound(const key_type& key)
    {
        return internal_end(internal_upper_bound(key, iterator(root(), 0)));
    }

    const_iterator upper_bound(const key_type& key) const
    {
        return internal_end(internal_upper_bound(key, const_iterator(root(), 0)));
    }

    // Finds the range of values which compare equal to key. The first
    // member of the returned pair is equal to lower_bound(key). The
    // second member pair of the pair is equal to upper_bound(key).
    pair<iterator, iterator> equal_range(const key_type& key)
    {
        return std::make_pair(lower_bound(key), upper_bound(key));
    }

    pair<const_iterator, const_iterator> equal_range(const key_type& key) const
    {
        return std::make_pair(lower_bound(key), upper_bound(key));
    }

    // Finds if an element already exists in the tree. Boolean
    // indicates whether insertion can continue, and the
    // iterator if the boolean is false is the position
    // of the value in the map, otherwise, it is the position
    // to insert.
    pair<iterator, bool> find_insert_unique(const key_type& key);

    // Inserts a value into the btree only if it does not already
    // exist. The boolean return value indicates whether insertion
    // succeeded or failed. The ValuePointer type is used to avoid
    // instatiating the value unless the key is being inserted.
    // Value is not dereferenced if the key already exists in
    // the btree. See btree_map::operator[].
    template <typename ValuePointer>
    pair<iterator, bool> insert_unique(const key_type& key, ValuePointer value);

    // Inserts a value into the btree only if it does not already
    // exist. The boolean return value indicates whether insertion
    // succeeded or failed.
    pair<iterator,bool> insert_unique(const value_type& v)
    {
        return insert_unique(params_type::key(v), &v);
    }

    // Construct value in-place as if by insert_unique
    template <typename ... Ts>
    pair<iterator, bool> emplace_unique(Ts&&... ts);

    // Find insertion position and insert condition with hint.
    pair<iterator, bool> find_insert_unique_hint(iterator position, const key_type& key);

    // Insert with hint. Check to see if the value should be placed
    // immediately before position in the tree. If it does, then the
    // insertion will take amortized constant time. If not, the
    // insertion will take amortized logarithmic time as if a call
    // to insert_unique(v) were made.
    iterator insert_unique_hint(iterator position, const value_type& v);

    // Construct value in-place as if by insert_unique_hint
    template <typename ... Ts>
    iterator emplace_unique_hint(iterator position, Ts&&... ts);

    // Insert a range of values into the btree.
    template <typename InputIterator>
    void insert_unique_range(InputIterator b, InputIterator e);

    // Get iterator to internal insertion point.
    iterator find_insert_multi(const key_type& key);

    // Inserts a value into the btree. The ValuePointer type is used
    // to avoid instatiating the value unless the key is being
    // inserted. Value is not dereferenced if the key already exists
    // in the btree. See btree_map::operator[].
    template <typename ValuePointer>
    iterator insert_multi(const key_type& key, ValuePointer value);

    // Inserts a value into the btree.
    iterator insert_multi(const value_type& v)
    {
        return insert_multi(params_type::key(v), &v);
    }

    // Construct value in-place as if by insert_multi
    template <typename ... Ts>
    iterator emplace_multi(Ts&&... ts);

    // Find insertion position and insert condition with hint.
    iterator find_insert_multi_hint(iterator position, const key_type& key);

    // Insert with hint. Check to see if the value should be placed
    // immediately before position in the tree. If it does, then the
    // insertion will take amortized constant time. If not, the
    // insertion will take amortized logarithmic time as if a call
    // to insert_multi(v) were made.
    iterator insert_multi_hint(iterator position, const value_type& v);

    // Construct value in-place as if by insert_multi_hint
    template <typename ... Ts>
    iterator emplace_multi_hint(iterator position, Ts&&... ts);

    // Insert a range of values into the btree.
    template <typename InputIterator>
    void insert_multi_range(InputIterator b, InputIterator e);

    void assign(const self_type& x);

    // Erase the specified iterator from the btree. The iterator must
    // be valid (i.e. not equal to end()).  Return an iterator pointing
    // to the node after the one that was erased (or end() if none
    // exists).
    iterator erase(iterator iter);

    // Erases range. Returns the number of keys erased.
    int erase(iterator begin, iterator end);

    // Erases the specified key from the btree. Returns 1 if an element
    // was erased and 0 otherwise.
    int erase_unique(const key_type& key);

    // Erases all of the entries matching the specified key from the
    // btree. Returns the number of elements erased.
    int erase_multi(const key_type& key);

    // Finds the iterator corresponding to a key or returns end() if
    // the key is not present.
    iterator find_unique(const key_type& key)
    {
        return internal_end(internal_find_unique(key, iterator(root(), 0)));
    }
    const_iterator find_unique(const key_type& key) const
    {
        return internal_end(internal_find_unique(key, const_iterator(root(), 0)));
    }
    iterator find_multi(const key_type& key)
    {
        return internal_end(internal_find_multi(key, iterator(root(), 0)));
    }
    const_iterator find_multi(const key_type& key) const
    {
        return internal_end(internal_find_multi(key, const_iterator(root(), 0)));
    }

    // Returns a count of the number of times the key appears in
    // the btree.
    size_type count_unique(const key_type& key) const
    {
        const_iterator begin = internal_find_unique(key, const_iterator(root(), 0));
        if (!begin.node) {
            // The key doesn't exist in the tree.
            return 0;
        }
        return 1;
    }

    // Returns a count of the number of times the key appears in
    // the btree.
    size_type count_multi(const key_type& key) const
    {
        return distance(lower_bound(key), upper_bound(key));
    }

    // Clear the btree, deleting all of the values it contains.
    void clear();

    // Swap the contents of *this and x.
    void swap(self_type& x);

    // Assign the contents of x to *this.
    self_type& operator=(const self_type& x)
    {
        if (this != &x) {
            assign(x);
        }

        return *this;
    }

    key_compare* mutable_key_comp()
    {
        return this;
    }

    const key_compare& key_comp() const
    {
        return *this;
    }

    bool compare_keys(const key_type& x, const key_type& y) const
    {
        return btree_compare_keys(key_comp(), x, y);
    }

    // Dump the btree to the specified ostream. Requires that
    // operator<< is defined for Key and Value.
    void dump(std::ostream& os) const
    {
        if (root() != nullptr) {
            internal_dump(os, root(), 0);
        }
    }

    // Verifies the structure of the btree.
    void verify() const;

    // Size routines. Note that empty() is slightly faster than
    // doing size()==0.
    size_type size() const
    {
        if (empty()) {
            return 0;
        }
        if (root()->leaf()) {
            return root()->count();
        }
        return root()->size();
    }

    size_type max_size() const
    {
        return numeric_limits<size_type>::max();
    }

    bool empty() const
    {
        return root() == nullptr;
    }

    // The height of the btree. An empty tree will have height 0.
    size_type height() const
    {
        size_type h = 0;
        if (root()) {
            // Count the length of the chain from the leftmost node up
            // to the root. We actually count from the root back around
            // to the level below the root, but the calculation is the
            // same because of the circularity of that traversal.
            const node_type* n = root();
            do {
                ++h;
                n = n->parent();
            } while (n != root());
        }
        return h;
    }

    // The number of internal, leaf and total nodes used by the btree.
    size_type leaf_nodes() const
    {
        return internal_stats(root()).leaf_nodes;
    }

    size_type internal_nodes() const
    {
        return internal_stats(root()).internal_nodes;
    }

    size_type nodes() const
    {
        node_stats stats = internal_stats(root());
        return stats.leaf_nodes + stats.internal_nodes;
    }

    // The total number of bytes used by the btree.
    size_type bytes_used() const
    {
        node_stats stats = internal_stats(root());
        if (stats.leaf_nodes == 1 && stats.internal_nodes == 0) {
            return sizeof(*this) +
                sizeof(base_fields) + root()->max_count() * sizeof(value_type);
        } else {
            return sizeof(*this) +
                sizeof(root_fields) - sizeof(internal_fields) +
                stats.leaf_nodes * sizeof(leaf_fields) +
                stats.internal_nodes * sizeof(internal_fields);
        }
    }

    // The average number of bytes used per value stored in the btree.
    static double average_bytes_per_value()
    {
        // Returns the number of bytes per value on a leaf node that
        // is 75% full. Experimentally, this matches up nicely with
        // the computed number of bytes per value in trees that had
        // their values inserted in random order.
        return sizeof(leaf_fields) / (node_values * 0.75);
    }

    // The fullness of the btree. Computed as the number of elements
    // in the btree divided by the maximum number of elements a tree
    // with the current number of nodes could hold. A value of 1
    // indicates perfect space utilization. Smaller values indicate
    // space wastage.
    double fullness() const
    {
        return double(size()) / (nodes() * node_values);
    }

    // The overhead of the btree structure in bytes per node. Computed
    // as the total number of bytes used by the btree minus the number
    // of bytes used for storing elements divided by the number of
    // elements.
    double overhead() const
    {
        if (empty()) {
            return 0.0;
        }
        return (bytes_used() - size() * value_size) / double(size());
    }

private:
    // Internal accessor routines.
    node_type* root()
    {
        return root_.data;
    }

    const node_type* root() const
    {
        return root_.data;
    }

    node_type** mutable_root()
    {
        return &root_.data;
    }

    // The rightmost node is stored in the root node.
    node_type* rightmost()
    {
        return (!root() || root()->leaf()) ? root() : root()->rightmost();
    }

    const node_type* rightmost() const
    {
        return (!root() || root()->leaf()) ? root() : root()->rightmost();
    }

    node_type** mutable_rightmost()
    {
        return root()->mutable_rightmost();
    }

    // The leftmost node is stored as the parent of the root node.
    node_type* leftmost()
    {
        return root() ? root()->parent() : nullptr;
    }

    const node_type* leftmost() const
    {
        return root() ? root()->parent() : nullptr;
    }

    // The size of the tree is stored in the root node.
    size_type* mutable_size()
    {
        return root()->mutable_size();
    }

    // Allocator routines.
    internal_allocator_type* mutable_internal_allocator()
    {
        return static_cast<internal_allocator_type*>(&root_);
    }

    const internal_allocator_type& internal_allocator() const
    {
        return *static_cast<const internal_allocator_type*>(&root_);
    }

    // Node creation/deletion routines.
    node_type* new_internal_node(node_type* parent)
    {
        internal_fields* p = reinterpret_cast<internal_fields*>(mutable_internal_allocator()->allocate(sizeof(internal_fields)));
        return node_type::init_internal(p, parent);
    }

    node_type* new_internal_root_node()
    {
        root_fields* p = reinterpret_cast<root_fields*>(mutable_internal_allocator()->allocate(sizeof(root_fields)));
        return node_type::init_root(p, root()->parent());
    }

    node_type* new_leaf_node(node_type* parent)
    {
        leaf_fields* p = reinterpret_cast<leaf_fields*>(mutable_internal_allocator()->allocate(sizeof(leaf_fields)));
        return node_type::init_leaf(p, parent, node_values);
    }

    node_type* new_leaf_root_node(int max_count)
    {
        leaf_fields* p = reinterpret_cast<leaf_fields*>(mutable_internal_allocator()->allocate(sizeof(base_fields) + max_count * sizeof(value_type)));
        return node_type::init_leaf(p, reinterpret_cast<node_type*>(p), max_count);
    }

    void delete_internal_node(node_type* node)
    {
        node->destroy();
        assert(node != root());
        mutable_internal_allocator()->deallocate(reinterpret_cast<char*>(node), sizeof(internal_fields));
    }

    void delete_internal_root_node()
    {
        root()->destroy();
        mutable_internal_allocator()->deallocate(reinterpret_cast<char*>(root()), sizeof(root_fields));
    }

    void delete_leaf_node(node_type* node)
    {
        node->destroy();
        mutable_internal_allocator()->deallocate(reinterpret_cast<char*>(node),sizeof(base_fields) + node->max_count() * sizeof(value_type));
    }

    // Rebalances or splits the node iter points to.
    void rebalance_or_split(iterator* iter);

    // Merges the values of left, right and the delimiting key on their
    // paren onto left, removing the delimiting key and deleting right.
    void merge_nodes(node_type* left, node_type* right);

    // Tries to merge node with its left or right sibling, and failing
    // that, rebalance with its left or right sibling. Returns true if
    // a merge occurred, at which point it is no longer valid to access
    // node. Returns false if no merging took place.
    bool try_merge_or_rebalance(iterator* iter);

    // Tries to shrink the height of the tree by 1.
    void try_shrink();

    iterator internal_end(iterator iter)
    {
        return iter.node ? iter : end();
    }

    const_iterator internal_end(const_iterator iter) const
    {
        return iter.node ? iter : end();
    }

    // Inserts a value into the btree immediately before iter. Requires
    // that key(v) <= iter.key() and (--iter).key() <= key(v).
    template <typename ... Ts>
    iterator internal_insert(iterator iter, Ts&&... ts);

    // Returns an iterator pointing to the first value >= the value "iter"
    // is pointing at. Note that "iter" might be pointing to an invalid
    // location as iter.position == iter.node->count(). This routine
    // simply moves iter up in the tree to a valid location.
    template <typename IterType>
    static IterType internal_last(IterType iter);

    // Returns an iterator pointing to the leaf position at which key
    // would reside in the tree. We provide 2 versions of
    // internal_locate. The first version (internal_locate_plain_compare)
    // always returns 0 for the second field of the pair. The second
    // version (internal_locate_compare_to) is for the key-compare-to
    // specialization and returns either exact_match (if the key was
    // found in the tree) or -exact_match (if it wasn't) in the second
    // field of the pair. The compare_to specialization allows the
    // caller to avoid a subsequent comparison to determine if an exact
    // match was made, speeding up string keys.
    template <typename IterType>
    pair<IterType, int> internal_locate(const key_type& key, IterType iter) const;

    template <typename IterType>
    pair<IterType, int> internal_locate_plain_compare(const key_type& key, IterType iter) const;

    template <typename IterType>
    pair<IterType, int> internal_locate_compare_to(const key_type& key, IterType iter) const;

    // Internal routine which implements lower_bound().
    template <typename IterType>
    IterType internal_lower_bound(const key_type& key, IterType iter) const;

    // Internal routine which implements upper_bound().
    template <typename IterType>
    IterType internal_upper_bound(const key_type& key, IterType iter) const;

    // Internal routine which implements find_unique().
    template <typename IterType>
    IterType internal_find_unique(const key_type& key, IterType iter) const;

    // Internal routine which implements find_multi().
    template <typename IterType>
    IterType internal_find_multi(const key_type& key, IterType iter) const;

    // Deletes a node and all of its children.
    void internal_clear(node_type* node);

    // Dumps a node and all of its children to the specified ostream.
    void internal_dump(std::ostream& os, const node_type* node, int level) const;

    // Verifies the tree structure of node.
    int internal_verify(const node_type* node, const key_type* lo, const key_type* hi) const;

    node_stats internal_stats(const node_type* node) const
    {
        if (!node) {
            return node_stats(0, 0);
        }
        if (node->leaf()) {
            return node_stats(1, 0);
        }
        node_stats res(0, 1);
        for (int i = 0; i <= node->count(); ++i) {
            res += internal_stats(node->child(i));
        }
        return res;
    }

    empty_base_handle<internal_allocator_type, node_type*> root_;

    // A never instantiated helper function that returns big_ if we
    // have a key-compare-to functor or if R is bool and small_
    // otherwise.
    template <typename R>
    static typename conditional<
        conditional<
            is_key_compare_to::value,
            is_same<R, int>,
            is_same<R, bool>
        >::type::value,
        big_,
        small_
    >::type key_compare_checker(R);

    // A never instantiated helper function that returns the key
    // comparison functor.
    static key_compare key_compare_helper();

    // Verify that key_compare returns a bool. This is similar to the
    // way is_convertible in base/type_traits.h works. Note that
    // key_compare_checker is never actually invoked. The compiler
    // will select which key_compare_checker() to instantiate and then
    // figure out the size of the return type of key_compare_checker()
    // at compile time which we then check against the sizeof of big_.
    static_assert(
        sizeof(key_compare_checker(key_compare_helper()(key_type(), key_type()))) == sizeof(big_),
        "key_comparison_function_must_return_bool"
    );

    // Note: We insist on kTargetValues, which is computed from
    // Params::target_node_size, must fit the base_fields::field_type.
    static_assert(
        node_values < (1 << (8 * sizeof(typename base_fields::field_type))),
        "target_node_size_too_large"
    );

    // Test the assumption made in setting kNodeValueSpace.
    static_assert(
        sizeof(base_fields) >= 2 * sizeof(void*),
        "node_space_assumption_incorrect"
    );
};


// BTREE CONTAINER

// A common base class for btree_set, btree_map, btree_multiset and
// btree_multimap.
template <typename Tree>
class btree_container
{
    using self_type = btree_container<Tree>;

public:
    using params_type = typename Tree::params_type;
    using key_type = typename Tree::key_type;
    using value_type = typename Tree::value_type;
    using key_compare = typename Tree::key_compare;
    using allocator_type = typename Tree::allocator_type;
    using pointer = typename Tree::pointer;
    using const_pointer = typename Tree::const_pointer;
    using reference = typename Tree::reference;
    using const_reference = typename Tree::const_reference;
    using size_type = typename Tree::size_type;
    using difference_type = typename Tree::difference_type;
    using iterator = typename Tree::iterator;
    using const_iterator = typename Tree::const_iterator;
    using reverse_iterator = typename Tree::reverse_iterator;
    using const_reverse_iterator = typename Tree::const_reverse_iterator;

    // Default constructor.
    btree_container(const allocator_type& alloc):
        tree_(alloc)
    {}

    btree_container(const key_compare& comp, const allocator_type& alloc):
        tree_(comp, alloc)
    {}

    // Copy constructor.
    btree_container(const self_type& x):
        tree_(x.tree_)
    {}

    btree_container(const self_type& x, const allocator_type& alloc):
        tree_(x.tree_, alloc)
    {}

    // Move constructor
    btree_container(self_type&& x):
        tree_(std::move(x.tree_))
    {}

    btree_container(self_type&& x, const allocator_type& alloc):
        tree_(std::move(x.tree_), alloc)
    {}

    // Iterator routines.
    iterator begin()
    {
        return tree_.begin();
    }

    const_iterator begin() const
    {
        return tree_.begin();
    }

    iterator end()
    {
        return tree_.end();
    }

    const_iterator end() const
    {
        return tree_.end();
    }

    reverse_iterator rbegin()
    {
        return tree_.rbegin();
    }

    const_reverse_iterator rbegin() const
    {
        return tree_.rbegin();
    }

    reverse_iterator rend()
    {
        return tree_.rend();
    }

    const_reverse_iterator rend() const
    {
        return tree_.rend();
    }

    // Lookup routines.
    iterator lower_bound(const key_type& key)
    {
        return tree_.lower_bound(key);
    }

    const_iterator lower_bound(const key_type& key) const
    {
        return tree_.lower_bound(key);
    }

    iterator upper_bound(const key_type& key)
    {
        return tree_.upper_bound(key);
    }

    const_iterator upper_bound(const key_type& key) const
    {
        return tree_.upper_bound(key);
    }

    pair<iterator,iterator> equal_range(const key_type& key)
    {
        return tree_.equal_range(key);
    }

    pair<const_iterator,const_iterator> equal_range(const key_type& key) const
    {
        return tree_.equal_range(key);
    }

    // Utility routines.
    void clear()
    {
        tree_.clear();
    }

    void swap(self_type& x)
    {
        tree_.swap(x.tree_);
    }

    void dump(std::ostream& os) const
    {
        tree_.dump(os);
    }

    void verify() const
    {
        tree_.verify();
    }

    // Size routines.
    size_type size() const
    {
        return tree_.size();
    }

    size_type max_size() const
    {
        return tree_.max_size();
    }

    bool empty() const
    {
        return tree_.empty();
    }

    size_type height() const
    {
        return tree_.height();
    }

    size_type internal_nodes() const
    {
        return tree_.internal_nodes();
    }

    size_type leaf_nodes() const
    {
        return tree_.leaf_nodes();
    }

    size_type nodes() const
    {
        return tree_.nodes();
    }

    size_type bytes_used() const
    {
        return tree_.bytes_used();
    }

    static double average_bytes_per_value()
    {
        return Tree::average_bytes_per_value();
    }

    double fullness() const
    {
        return tree_.fullness();
    }

    double overhead() const
    {
        return tree_.overhead();
    }

    bool operator==(const self_type& x) const
    {
        if (size() != x.size()) {
            return false;
        }
        for (const_iterator i = begin(), xi = x.begin(); i != end(); ++i, ++xi) {
            if (*i != *xi) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const self_type& other) const
    {
        return !operator==(other);
    }

protected:
    Tree tree_;
};


// A common base class for btree_set and safe_btree_set.
template <typename Tree>
class btree_unique_container: public btree_container<Tree>
{
    using self_type = btree_unique_container<Tree>;
    using super_type = btree_container<Tree>;

public:
    using key_type = typename Tree::key_type;
    using value_type = typename Tree::value_type;
    using size_type = typename Tree::size_type;
    using key_compare = typename Tree::key_compare;
    using allocator_type = typename Tree::allocator_type;
    using iterator = typename Tree::iterator;
    using const_iterator = typename Tree::const_iterator;

    // Default constructor.
    btree_unique_container(const allocator_type& alloc):
        super_type(alloc)
    {}

    btree_unique_container(const key_compare& comp = key_compare(),
                           const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {}

    // Copy constructor.
    btree_unique_container(const self_type& x):
        super_type(x)
    {}

    btree_unique_container(const self_type& x, const allocator_type& alloc):
        super_type(x, alloc)
    {}

    // Move constructor
    btree_unique_container(self_type&& x):
        super_type(std::forward<self_type>(x))
    {}

    btree_unique_container(self_type&& x, const allocator_type& alloc):
        super_type(std::forward<self_type>(x), alloc)
    {}

    // Range constructor.
    template <typename InputIterator>
    btree_unique_container(InputIterator b, InputIterator e,
                           const key_compare& comp = key_compare(),
                           const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {
        insert(b, e);
    }

    template <typename InputIterator>
    btree_unique_container(InputIterator b, InputIterator e,
                           const allocator_type& alloc):
        super_type(alloc)
    {
        insert(b, e);
    }

    // Initializer list constructor
    btree_unique_container(initializer_list<value_type> list,
                           const key_compare& comp = key_compare(),
                           const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {
        insert(list.begin(), list.end());
    }

    btree_unique_container(initializer_list<value_type> list,
                           const allocator_type& alloc = allocator_type()):
        super_type(alloc)
    {
        insert(list.begin(), list.end());
    }

    // Lookup routines.
    iterator find(const key_type& key)
    {
        return this->tree_.find_unique(key);
    }

    const_iterator find(const key_type& key) const
    {
        return this->tree_.find_unique(key);
    }

    size_type count(const key_type& key) const
    {
        return this->tree_.count_unique(key);
    }

    // Emplace routines
    template <typename... Ts>
    pair<iterator,bool> emplace(Ts&&... ts)
    {
        return this->tree_.emplace_unique(std::forward<Ts>(ts)...);
    }

    template <typename... Ts>
    iterator emplace_hint(const_iterator hint, Ts&&... ts)
    {
        return this->tree_.emplace_unique_hint(hint, std::forward<Ts>(ts)...);
    }

    // Insertion routines.
    pair<iterator,bool> insert(const value_type &x)
    {
        return this->tree_.insert_unique(x);
    }

    iterator insert(iterator position, const value_type &x)
    {
        return this->tree_.insert_unique_hint(position, x);
    }

    template <typename InputIterator>
    void insert(InputIterator b, InputIterator e)
    {
        this->tree_.insert_unique_range(b, e);
    }

    // Deletion routines.
    int erase(const key_type& key)
    {
        return this->tree_.erase_unique(key);
    }

    // Erase the specified iterator from the btree. The iterator must
    // be valid (i.e. not equal to end()).  Return an iterator pointing
    // to the node after the one that was erased (or end() if none
    // exists).
    iterator erase(const iterator& iter)
    {
        return this->tree_.erase(iter);
    }

    void erase(const iterator& first, const iterator& last)
    {
        this->tree_.erase(first, last);
    }
};


// A common base class for btree_map and safe_btree_map.
template <typename Tree>
class btree_map_container: public btree_unique_container<Tree>
{
    using self_type = btree_map_container<Tree>;
    using super_type = btree_unique_container<Tree>;

public:
    using key_type = typename Tree::key_type;
    using data_type = typename Tree::data_type;
    using value_type = typename Tree::value_type;
    using mapped_type = typename Tree::mapped_type;
    using key_compare = typename Tree::key_compare;
    using allocator_type = typename Tree::allocator_type;

private:
    // A pointer-like object which only generates its value when
    // dereferenced. Used by operator[] to avoid constructing an
    // empty data_type if the key already exists in the map.
    struct generate_value
    {
        generate_value(const key_type& k):
            key(k)
        {}

        value_type operator*() const
        {
            return std::make_pair(key, data_type());
        }

        const key_type& key;
    };

public:
    // Default constructor.
    btree_map_container(const allocator_type& alloc):
        super_type(alloc)
    {}

    btree_map_container(const key_compare& comp = key_compare(),
                        const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {}

    // Copy constructor.
    btree_map_container(const self_type& x):
        super_type(x)
    {}

    btree_map_container(const self_type& x, const allocator_type& alloc):
        super_type(x, alloc)
    {}

    // Move constructor
    btree_map_container(self_type&& x):
        super_type(std::forward<self_type>(x))
    {}

    btree_map_container(self_type&& x, const allocator_type& alloc):
        super_type(std::forward<self_type>(x), alloc)
    {}

    // Range constructor.
    template <typename InputIterator>
    btree_map_container(InputIterator b, InputIterator e,
                        const key_compare& comp = key_compare(),
                        const allocator_type& alloc = allocator_type()):
        super_type(b, e, comp, alloc)
    {}

    template <typename InputIterator>
    btree_map_container(InputIterator b, InputIterator e,
                        const allocator_type& alloc):
        super_type(b, e, alloc)
    {}

    // Initalizer list constructor
    btree_map_container(initializer_list<value_type> list,
                        const key_compare& comp = key_compare(),
                        const allocator_type& alloc = allocator_type()):
        super_type(list.begin(), list.end(), comp, alloc)
    {}

    btree_map_container(initializer_list<value_type> list,
                        const allocator_type& alloc = allocator_type()):
        super_type(list.begin(), list.end(), alloc)
    {}

    // Insertion routines.
    data_type& operator[](const key_type& key)
    {
        return this->tree_.insert_unique(key, generate_value(key)).first->second;
    }
};


// A common base class for btree_multiset and btree_multimap.
template <typename Tree>
class btree_multi_container: public btree_container<Tree>
{
    using self_type = btree_multi_container<Tree>;
    using super_type = btree_container<Tree>;

public:
    using key_type = typename Tree::key_type;
    using value_type = typename Tree::value_type;
    using size_type = typename Tree::size_type;
    using key_compare = typename Tree::key_compare;
    using allocator_type = typename Tree::allocator_type;
    using iterator = typename Tree::iterator;
    using const_iterator = typename Tree::const_iterator;

    // Default constructor.
    btree_multi_container(const allocator_type& alloc):
        super_type(alloc)
    {}

    btree_multi_container(const key_compare& comp = key_compare(),
                          const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {}

    // Copy constructor.
    btree_multi_container(const self_type& x):
        super_type(x)
    {}

    btree_multi_container(const self_type& x, const allocator_type& alloc):
        super_type(x, alloc)
    {}

    // Move constructor
    btree_multi_container(self_type&& x):
        super_type(std::forward<self_type>(x))
    {}

    btree_multi_container(self_type&& x, const allocator_type& alloc):
        super_type(std::forward<self_type>(x), alloc)
    {}

    // Range constructor.
    template <typename InputIterator>
    btree_multi_container(InputIterator b, InputIterator e,
                          const key_compare& comp = key_compare(),
                          const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {
        insert(b, e);
    }

    template <typename InputIterator>
    btree_multi_container(InputIterator b, InputIterator e,
                          const allocator_type& alloc = allocator_type()):
        super_type(alloc)
    {
        insert(b, e);
    }

    // Initializer list constructor
    btree_multi_container(initializer_list<value_type> list,
                          const key_compare& comp = key_compare(),
                          const allocator_type& alloc = allocator_type()):
        super_type(comp, alloc)
    {
        insert(list.begin(), list.end());
    }

    btree_multi_container(initializer_list<value_type> list,
                          const allocator_type& alloc = allocator_type()):
        super_type(alloc)
    {
        insert(list.begin(), list.end());
    }

    // Lookup routines.
    iterator find(const key_type& key)
    {
        return this->tree_.find_multi(key);
    }

    const_iterator find(const key_type& key) const
    {
        return this->tree_.find_multi(key);
    }

    size_type count(const key_type& key) const
    {
        return this->tree_.count_multi(key);
    }

    // Emplace routines
    template <typename... Ts>
    iterator emplace(Ts&&... ts)
    {
        return this->tree_.emplace_multi(std::forward<Ts>(ts)...);
    }

    template <typename... Ts>
    iterator emplace_hint(const_iterator hint, Ts&&... ts)
    {
        return this->tree_.emplace_multi_hint(hint, std::forward<Ts>(ts)...);
    }

    // Insertion routines.
    iterator insert(const value_type &x)
    {
        return this->tree_.insert_multi(x);
    }

    iterator insert(iterator position, const value_type &x)
    {
        return this->tree_.insert_multi_hint(position, x);
    }

    template <typename InputIterator>
    void insert(InputIterator b, InputIterator e)
    {
        this->tree_.insert_multi_range(b, e);
    }

    // Deletion routines.
    int erase(const key_type& key)
    {
        return this->tree_.erase_multi(key);
    }

    // Erase the specified iterator from the btree. The iterator must
    // be valid (i.e. not equal to end()).  Return an iterator pointing
    // to the node after the one that was erased (or end() if none
    // exists).
    iterator erase(const iterator& iter)
    {
        return this->tree_.erase(iter);
    }

    void erase(const iterator& first, const iterator& last)
    {
        this->tree_.erase(first, last);
    }
};

// IMPLEMENTATION
// --------------

// BTREE

// btree_node methods
template <typename P>
template <typename ... Ts>
inline void btree_node<P>::insert_value(int i, Ts&&... ts)
{
    assert(i <= count());
    value_init(count(), std::forward<Ts>(ts)...);
    for (int j = count(); j > i; --j) {
        value_swap(j, this, j - 1);
    }
    set_count(count() + 1);

    if (!leaf()) {
        ++i;
        for (int j = count(); j > i; --j) {
            *mutable_child(j) = child(j - 1);
            child(j)->set_position(j);
        }
        *mutable_child(i) = nullptr;
    }
}


template <typename P>
inline void btree_node<P>::remove_value(int i)
{
    if (!leaf()) {
        assert(child(i + 1)->count() == 0);
        for (int j = i + 1; j < count(); ++j) {
            *mutable_child(j) = child(j + 1);
            child(j)->set_position(j);
        }
        *mutable_child(count()) = nullptr;
    }

    set_count(count() - 1);
    for (; i < count(); ++i) {
        value_swap(i, this, i + 1);
    }
    value_destroy(i);
}


template <typename P>
void btree_node<P>::rebalance_right_to_left(btree_node* src, int to_move)
{
    assert(parent() == src->parent());
    assert(position() + 1 == src->position());
    assert(src->count() >= count());
    assert(to_move >= 1);
    assert(to_move <= src->count());

    // Make room in the left node for the new values.
    for (int i = 0; i < to_move; ++i) {
        value_init(i + count());
    }

    // Move the delimiting value to the left node and the new
    // delimiting value from the right node.
    value_swap(count(), parent(), position());
    parent()->value_swap(position(), src, to_move - 1);

    // Move the values from the right to the left node.
    for (int i = 1; i < to_move; ++i) {
        value_swap(count() + i, src, i - 1);
    }

    // Shift the values in the right node to their correct position.
    for (int i = to_move; i < src->count(); ++i) {
        src->value_swap(i - to_move, src, i);
    }
    for (int i = 1; i <= to_move; ++i) {
        src->value_destroy(src->count() - i);
    }

    if (!leaf()) {
        // Move the child pointers from the right to the left node.
        for (int i = 0; i < to_move; ++i) {
            set_child(1 + count() + i, src->child(i));
        }
        for (int i = 0; i <= src->count() - to_move; ++i) {
            assert(i + to_move <= src->max_count());
            src->set_child(i, src->child(i + to_move));
            *src->mutable_child(i + to_move) = nullptr;
        }
    }

    // Fixup the counts on the src and dest nodes.
    set_count(count() + to_move);
    src->set_count(src->count() - to_move);
}


template <typename P>
void btree_node<P>::rebalance_left_to_right(btree_node* dest, int to_move)
{
    assert(parent() == dest->parent());
    assert(position() + 1 == dest->position());
    assert(count() >= dest->count());
    assert(to_move >= 1);
    assert(to_move <= count());

    // Make room in the right node for the new values.
    for (int i = 0; i < to_move; ++i) {
        dest->value_init(i + dest->count());
    }
    for (int i = dest->count() - 1; i >= 0; --i) {
        dest->value_swap(i, dest, i + to_move);
    }

    // Move the delimiting value to the right node and the new
    // delimiting value from the left node.
    dest->value_swap(to_move - 1, parent(), position());
    parent()->value_swap(position(), this, count() - to_move);
    value_destroy(count() - to_move);

    // Move the values from the left to the right node.
    for (int i = 1; i < to_move; ++i) {
        value_swap(count() - to_move + i, dest, i - 1);
        value_destroy(count() - to_move + i);
    }

    if (!leaf()) {
        // Move the child pointers from the left to the right node.
        for (int i = dest->count(); i >= 0; --i) {
            dest->set_child(i + to_move, dest->child(i));
            *dest->mutable_child(i) = nullptr;
        }
        for (int i = 1; i <= to_move; ++i) {
            dest->set_child(i - 1, child(count() - to_move + i));
            *mutable_child(count() - to_move + i) = nullptr;
        }
    }

    // Fixup the counts on the src and dest nodes.
    set_count(count() - to_move);
    dest->set_count(dest->count() + to_move);
}


template <typename P>
void btree_node<P>::split(btree_node* dest, int insert_position)
{
    assert(dest->count() == 0);

    // We bias the split based on the position being inserted. If
    // we're inserting at the beginning of the left node then bias
    // the split to put more values on the right node. If we're
    // inserting at the end of the right node then bias the split
    // to put more values on the left node.
    if (insert_position == 0) {
        dest->set_count(count() - 1);
    } else if (insert_position == max_count()) {
        dest->set_count(0);
    } else {
        dest->set_count(count() / 2);
    }
    set_count(count() - dest->count());
    assert(count() >= 1);

    // Move values from the left sibling to the right sibling.
    for (int i = 0; i < dest->count(); ++i) {
        dest->value_init(i);
        value_swap(count() + i, dest, i);
        value_destroy(count() + i);
    }

    // The split key is the largest value in the left sibling.
    set_count(count() - 1);
    parent()->insert_value(position(), value_type());
    value_swap(count(), parent(), position());
    value_destroy(count());
    parent()->set_child(position() + 1, dest);

    if (!leaf()) {
        for (int i = 0; i <= dest->count(); ++i) {
            assert(child(count() + i + 1) != nullptr);
            dest->set_child(i, child(count() + i + 1));
            *mutable_child(count() + i + 1) = nullptr;
        }
    }
}


template <typename P>
void btree_node<P>::merge(btree_node* src)
{
    assert(parent() == src->parent());
    assert(position() + 1 == src->position());

    // Move the delimiting value to the left node.
    value_init(count());
    value_swap(count(), parent(), position());

    // Move the values from the right to the left node.
    for (int i = 0; i < src->count(); ++i) {
        value_init(1 + count() + i);
        value_swap(1 + count() + i, src, i);
        src->value_destroy(i);
    }

    if (!leaf()) {
        // Move the child pointers from the right to the left node.
        for (int i = 0; i <= src->count(); ++i) {
            set_child(1 + count() + i, src->child(i));
            *src->mutable_child(i) = nullptr;
        }
    }

    // Fixup the counts on the src and dest nodes.
    set_count(1 + count() + src->count());
    src->set_count(0);

    // Remove the value on the parent node.
    parent()->remove_value(position());
}


template <typename P>
void btree_node<P>::swap(btree_node* x)
{
    assert(leaf() == x->leaf());

    // Swap the values.
    for (int i = count(); i < x->count(); ++i) {
        value_init(i);
    }
    for (int i = x->count(); i < count(); ++i) {
        x->value_init(i);
    }
    int n = std::max(count(), x->count());
    for (int i = 0; i < n; ++i) {
        value_swap(i, x, i);
    }
    for (int i = count(); i < x->count(); ++i) {
        x->value_destroy(i);
    }
    for (int i = x->count(); i < count(); ++i) {
        value_destroy(i);
    }

    if (!leaf()) {
        // Swap the child pointers.
        for (int i = 0; i <= n; ++i) {
            btree_swap_helper(*mutable_child(i), *x->mutable_child(i));
        }
        for (int i = 0; i <= count(); ++i) {
            x->child(i)->fields_.parent = x;
        }
        for (int i = 0; i <= x->count(); ++i) {
            child(i)->fields_.parent = this;
        }
    }

    // Swap the counts.
    btree_swap_helper(fields_.count, x->fields_.count);
}


// btree_iterator methods
template <typename N, typename R, typename P>
void btree_iterator<N, R, P>::increment_slow()
{
    if (node->leaf()) {
        assert(position >= node->count());
        self_type save(*this);
        while (position == node->count() && !node->is_root()) {
            assert(node->parent()->child(node->position()) == node);
            position = node->position();
            node = node->parent();
        }
        if (position == node->count()) {
            *this = save;
        }
    } else {
        assert(position < node->count());
        node = node->child(position + 1);
        while (!node->leaf()) {
            node = node->child(0);
        }
        position = 0;
    }
}


template <typename N, typename R, typename P>
void btree_iterator<N, R, P>::increment_by(int count)
{
    while (count > 0) {
        if (node->leaf()) {
            int rest = node->count() - position;
            position += std::min(rest, count);
            count = count - rest;
            if (position < node->count()) {
                return;
            }
        } else {
            --count;
        }
        increment_slow();
    }
}


template <typename N, typename R, typename P>
void btree_iterator<N, R, P>::decrement_slow()
{
    if (node->leaf()) {
        assert(position <= -1);
        self_type save(*this);
        while (position < 0 && !node->is_root()) {
            assert(node->parent()->child(node->position()) == node);
            position = node->position() - 1;
            node = node->parent();
        }
        if (position < 0) {
            *this = save;
        }
    } else {
        assert(position >= 0);
        node = node->child(position);
        while (!node->leaf()) {
            node = node->child(node->count());
        }
        position = node->count() - 1;
    }
}


// btree methods
template <typename P>
btree<P>::btree(const allocator_type& alloc):
    root_(alloc, nullptr)
{}


template <typename P>
btree<P>::btree(const key_compare &comp, const allocator_type& alloc):
    key_compare(comp),
    root_(alloc, nullptr)
{}


template <typename P>
btree<P>::btree(const self_type& x):
    key_compare(x.key_comp()),
    root_(x.internal_allocator(), nullptr)
{
    assign(x);
}


template <typename P>
btree<P>::btree(const self_type& x, const allocator_type& alloc):
    key_compare(x.key_comp()),
    root_(alloc, nullptr)
{
    assign(x);
}


template <typename P>
btree<P>::btree(self_type&& x):
    key_compare(x.key_comp()),
    root_(x.internal_allocator(), nullptr)
{
    swap(x);
}


template <typename P>
btree<P>::btree(self_type&& x, const allocator_type& alloc):
    key_compare(x.key_comp()),
    root_(alloc, nullptr)
{
    swap(x);
}


template <typename P>
pair<typename btree<P>::iterator, bool>
btree<P>::find_insert_unique(const key_type& key)
{
    if (empty()) {
        *mutable_root() = new_leaf_root_node(1);
    }

    pair<iterator, int> res = internal_locate(key, iterator(root(), 0));
    iterator &iter = res.first;
    if (res.second == exact_match) {
        // The key already exists in the tree, do nothing.
        return std::make_pair(internal_last(iter), false);
    } else if (!res.second) {
        iterator last = internal_last(iter);
        if (last.node && !compare_keys(key, last.key())) {
            // The key already exists in the tree, do nothing.
            return std::make_pair(last, false);
        }
    }

    return std::make_pair(iter, true);
}


template <typename P> template <typename ValuePointer>
pair<typename btree<P>::iterator, bool>
btree<P>::insert_unique(const key_type& key, ValuePointer value)
{
    pair<iterator, bool> p = find_insert_unique(key);
    if (p.second) {
        return std::make_pair(internal_insert(p.first, *value), p.second);
    }
    return p;
}


template <typename P>
template <typename ... Ts>
pair<typename btree<P>::iterator, bool>
btree<P>::emplace_unique(Ts&&... ts)
{
    mutable_value_type v(std::forward<Ts>(ts)...);
    const auto& key = params_type::key(v);

    pair<iterator, bool> p = find_insert_unique(key);
    if (p.second) {
        return std::make_pair(internal_insert(p.first, std::move(v)), p.second);
    }
    return p;
}


template <typename P>
inline pair<typename btree<P>::iterator, bool>
btree<P>::find_insert_unique_hint(iterator position, const key_type& key)
{
    if (position == end() || compare_keys(key, position.key())) {
        iterator prev = position;
        if (position == begin() || compare_keys((--prev).key(), key)) {
            // prev.key() < key < position.key()
            return std::make_pair(position, true);
        }
    } else if (compare_keys(position.key(), key)) {
        iterator next = position;
        ++next;
        if (next == end() || compare_keys(key, next.key())) {
            // position.key() < key < next.key()
            return std::make_pair(next, true);
        }
    } else {
        // position.key() == key
        return std::make_pair(position, false);
    }
}


template <typename P>
inline typename btree<P>::iterator
btree<P>::insert_unique_hint(iterator position, const value_type& v)
{
    if (!empty()) {
        auto p = find_insert_unique_hint(position, params_type::key(v));
        if (p.second) {
            return internal_insert(p.first, v);
        }
        return p.first;
    }
    return insert_unique(v).first;
}


template <typename P>
template <typename ... Ts>
inline typename btree<P>::iterator
btree<P>::emplace_unique_hint(iterator position, Ts&&... ts)
{
    if (!empty()) {
        mutable_value_type v(std::forward<Ts>(ts)...);
        auto p = find_insert_unique_hint(position, params_type::key(v));
        if (p.second) {
            return internal_insert(p.first, std::move(v));
        }
        return p.first;
    }
    return emplace_unique(std::forward<Ts>(ts)...).first;
}


template <typename P> template <typename InputIterator>
void btree<P>::insert_unique_range(InputIterator b, InputIterator e)
{
    for (; b != e; ++b) {
        insert_unique(end(), *b);
    }
}


template <typename P>
typename btree<P>::iterator
btree<P>::find_insert_multi(const key_type &key)
{
    if (empty()) {
        *mutable_root() = new_leaf_root_node(1);
    }

    iterator iter = internal_upper_bound(key, iterator(root(), 0));
    if (!iter.node) {
        iter = end();
    }
    return iter;
}


template <typename P> template <typename ValuePointer>
typename btree<P>::iterator
btree<P>::insert_multi(const key_type &key, ValuePointer value)
{
    return internal_insert(find_insert_multi(key), *value);
}


template <typename P>
template <typename ... Ts>
typename btree<P>::iterator
btree<P>::emplace_multi(Ts&&... ts)
{
    mutable_value_type v(std::forward<Ts>(ts)...);
    iterator it = find_insert_multi(params_type::key(v));
    return internal_insert(it, std::move(v));
}


template <typename P>
typename btree<P>::iterator
btree<P>::find_insert_multi_hint(iterator position, const key_type& key)
{
    if (position == end() || !compare_keys(position.key(), key)) {
        iterator prev = position;
        if (position == begin() || !compare_keys(key, (--prev).key())) {
            // prev.key() <= key <= position.key()
            return position;
        }
    } else {
        iterator next = position;
        ++next;
        if (next == end() || !compare_keys(next.key(), key)) {
            // position.key() < key <= next.key()
            return next;
        }
    }
}


template <typename P>
typename btree<P>::iterator
btree<P>::insert_multi_hint(iterator position, const value_type& v)
{
    if (!empty()) {
        position = find_insert_multi_hint(position, params_type::key(v));
        return internal_insert(position, v);
    }
    return insert_multi(v);
}


template <typename P>
template <typename ... Ts>
typename btree<P>::iterator
btree<P>::emplace_multi_hint(iterator position, Ts&&... ts)
{
    if (!empty()) {
        mutable_value_type v(std::forward<Ts>(ts)...);
        position = find_insert_multi_hint(position, params_type::key(v));
        return internal_insert(position, std::move(v));
    }
    return insert_multi(std::forward<Ts>(ts)...);
}


template <typename P> template <typename InputIterator>
void btree<P>::insert_multi_range(InputIterator b, InputIterator e)
{
    for (; b != e; ++b) {
        insert_multi(end(), *b);
    }
}


template <typename P>
void btree<P>::assign(const self_type& x)
{
    clear();

    *mutable_key_comp() = x.key_comp();
    *mutable_internal_allocator() = x.internal_allocator();

    // Assignment can avoid key comparisons because we know the
    // order of the values is the same order we'll store them in.
    for (const_iterator iter = x.begin(); iter != x.end(); ++iter) {
        if (empty()) {
            insert_multi(*iter);
        } else {
            // If the btree is not empty, we can just insert the new
            // value at the end of the tree!
            internal_insert(end(), *iter);
        }
    }
}


template <typename P>
typename btree<P>::iterator btree<P>::erase(iterator iter)
{
    bool internal_delete = false;
    if (!iter.node->leaf()) {
        // Deletion of a value on an internal node. Swap the key with
        // the largest value of our left child. This is easy, we just
        // decrement iter.
        iterator tmp_iter(iter--);
        assert(iter.node->leaf());
        assert(!compare_keys(tmp_iter.key(), iter.key()));
        iter.node->value_swap(iter.position, tmp_iter.node, tmp_iter.position);
        internal_delete = true;
        --*mutable_size();
    } else if (!root()->leaf()) {
        --*mutable_size();
    }

    // Delete the key from the leaf.
    iter.node->remove_value(iter.position);

    // We want to return the next value after the one we just erased.
    // If we erased from an internal node (internal_delete == true),
    // then the next value is ++(++iter). If we erased from a leaf node
    // (internal_delete == false) then the next value is ++iter. Note
    // that ++iter may point to an internal node and the value in the
    // internal node may move to a leaf node (iter.node) when
    // rebalancing is performed at the leaf level.

    // Merge/rebalance as we walk back up the tree.
    iterator res(iter);
    for (;;) {
        if (iter.node == root()) {
            try_shrink();
            if (empty()) {
                return end();
            }
            break;
        }
        if (iter.node->count() >= min_node_values) {
            break;
        }
        bool merged = try_merge_or_rebalance(&iter);
        if (iter.node->leaf()) {
            res = iter;
        }
        if (!merged) {
            break;
        }
        iter.node = iter.node->parent();
    }

    // Adjust our return value. If we're pointing at the end of a node,
    // advance the iterator.
    if (res.position == res.node->count()) {
        res.position = res.node->count() - 1;
        ++res;
    }
    // If we erased from an internal node, advance the iterator.
    if (internal_delete) {
        ++res;
    }
    return res;
}


template <typename P>
int btree<P>::erase(iterator begin, iterator end)
{
    int count = distance(begin, end);
    for (int i = 0; i < count; i++) {
        begin = erase(begin);
    }
    return count;
}


template <typename P>
int btree<P>::erase_unique(const key_type& key)
{
    iterator iter = internal_find_unique(key, iterator(root(), 0));
    if (!iter.node) {
        // The key doesn't exist in the tree, return nothing done.
        return 0;
    }
    erase(iter);
    return 1;
}


template <typename P>
int btree<P>::erase_multi(const key_type& key)
{
    iterator begin = internal_lower_bound(key, iterator(root(), 0));
    if (!begin.node) {
        // The key doesn't exist in the tree, return nothing done.
        return 0;
    }
    // Delete all of the keys between begin and upper_bound(key).
    iterator end = internal_end(internal_upper_bound(key, iterator(root(), 0)));
    return erase(begin, end);
}


template <typename P>
void btree<P>::clear()
{
    if (root() != nullptr) {
        internal_clear(root());
    }
    *mutable_root() = nullptr;
}


template <typename P>
void btree<P>::swap(self_type &x)
{
    std::swap(static_cast<key_compare&>(*this), static_cast<key_compare&>(x));
    std::swap(root_, x.root_);
}


template <typename P>
void btree<P>::verify() const
{
    if (root() != nullptr) {
        assert(size() == internal_verify(root(), nullptr, nullptr));
        assert(leftmost() == (++const_iterator(root(), -1)).node);
        assert(rightmost() == (--const_iterator(root(), root()->count())).node);
        assert(leftmost()->leaf());
        assert(rightmost()->leaf());
    } else {
        assert(size() == 0);
        assert(leftmost() == nullptr);
        assert(rightmost() == nullptr);
    }
}


template <typename P>
void btree<P>::rebalance_or_split(iterator* iter)
{
    node_type *&node = iter->node;
    int &insert_position = iter->position;
    assert(node->count() == node->max_count());

    // First try to make room on the node by rebalancing.
    node_type* parent = node->parent();
    if (node != root()) {
        if (node->position() > 0) {
            // Try rebalancing with our left sibling.
            node_type *left = parent->child(node->position() - 1);
            if (left->count() < left->max_count()) {
                // We bias rebalancing based on the position being
                // inserted. If we're inserting at the end of the
                // right node then we bias rebalancing to fill up
                // the left node.
                int to_move = (left->max_count() - left->count()) / (1 + (insert_position < left->max_count()));
                to_move = std::max(1, to_move);

                if (((insert_position - to_move) >= 0) ||
                    ((left->count() + to_move) < left->max_count())) {
                    left->rebalance_right_to_left(node, to_move);

                    assert(node->max_count() - node->count() == to_move);
                    insert_position = insert_position - to_move;
                    if (insert_position < 0) {
                        insert_position = insert_position + left->count() + 1;
                        node = left;
                    }

                    assert(node->count() < node->max_count());
                    return;
                }
            }
        }

        if (node->position() < parent->count()) {
            // Try rebalancing with our right sibling.
            node_type *right = parent->child(node->position() + 1);
            if (right->count() < right->max_count()) {
                // We bias rebalancing based on the position being
                // inserted. If we're inserting at the beginning of
                // the left node then we bias rebalancing to fill up
                // the right node.
                int to_move = (right->max_count() - right->count()) / (1 + (insert_position > 0));
                to_move = std::max(1, to_move);

                if ((insert_position <= (node->count() - to_move)) ||
                    ((right->count() + to_move) < right->max_count())) {
                    node->rebalance_left_to_right(right, to_move);

                    if (insert_position > node->count()) {
                        insert_position = insert_position - node->count() - 1;
                        node = right;
                    }

                    assert(node->count() < node->max_count());
                    return;
                }
            }
        }

        // Rebalancing failed, make sure there is room on the parent
        // node for a new value.
        if (parent->count() == parent->max_count()) {
            iterator parent_iter(node->parent(), node->position());
            rebalance_or_split(&parent_iter);
        }
    } else {
        // Rebalancing not possible because this is the root node.
        if (root()->leaf()) {
            // The root node is currently a leaf node: create a new
            // root node and set the current root node as the child of
            // the new root.
            parent = new_internal_root_node();
            parent->set_child(0, root());
            *mutable_root() = parent;
            assert(*mutable_rightmost() == parent->child(0));
        } else {
            // The root node is an internal node. We do not want to
            // create a new root node because the root node is special
            // and holds the size of the tree and a pointer to the
            // rightmost node. So we create a new internal node and
            // move all of the items on the current root into the new
            // node.
            parent = new_internal_node(parent);
            parent->set_child(0, parent);
            parent->swap(root());
            node = parent;
        }
    }

    // Split the node.
    node_type* split_node;
    if (node->leaf()) {
        split_node = new_leaf_node(parent);
        node->split(split_node, insert_position);
        if (rightmost() == node) {
            *mutable_rightmost() = split_node;
        }
    } else {
        split_node = new_internal_node(parent);
        node->split(split_node, insert_position);
    }

    if (insert_position > node->count()) {
        insert_position = insert_position - node->count() - 1;
        node = split_node;
    }
}


template <typename P>
void btree<P>::merge_nodes(node_type* left, node_type* right)
{
    left->merge(right);
    if (right->leaf()) {
        if (rightmost() == right) {
            *mutable_rightmost() = left;
        }
        delete_leaf_node(right);
    } else {
        delete_internal_node(right);
    }
}


template <typename P>
bool btree<P>::try_merge_or_rebalance(iterator* iter)
{
    node_type* parent = iter->node->parent();
    if (iter->node->position() > 0) {
        // Try merging with our left sibling.
        node_type* left = parent->child(iter->node->position() - 1);
        if ((1 + left->count() + iter->node->count()) <= left->max_count()) {
            iter->position += 1 + left->count();
            merge_nodes(left, iter->node);
            iter->node = left;
            return true;
        }
    }
    if (iter->node->position() < parent->count()) {
        // Try merging with our right sibling.
        node_type* right = parent->child(iter->node->position() + 1);
        if ((1 + iter->node->count() + right->count()) <= right->max_count()) {
            merge_nodes(iter->node, right);
            return true;
        }
        // Try rebalancing with our right sibling. We don't perform
        // rebalancing if we deleted the first element from iter->node
        // and the node is not empty. This is a small optimization for
        // the common pattern of deleting from the front of the tree.
        if ((right->count() > min_node_values) &&
            ((iter->node->count() == 0) ||
            (iter->position > 0))) {
            int to_move = (right->count() - iter->node->count()) / 2;
            to_move = std::min(to_move, right->count() - 1);
            iter->node->rebalance_right_to_left(right, to_move);
            return false;
        }
    }
    if (iter->node->position() > 0) {
        // Try rebalancing with our left sibling. We don't perform
        // rebalancing if we deleted the last element from iter->node
        // and the node is not empty. This is a small optimization
        // for the common pattern of deleting from the back of the
        // tree.
        node_type* left = parent->child(iter->node->position() - 1);
        if ((left->count() > min_node_values) &&
            ((iter->node->count() == 0) ||
            (iter->position < iter->node->count()))) {
            int to_move = (left->count() - iter->node->count()) / 2;
            to_move = std::min(to_move, left->count() - 1);
            left->rebalance_left_to_right(iter->node, to_move);
            iter->position += to_move;
            return false;
        }
    }
    return false;
}


template <typename P>
void btree<P>::try_shrink()
{
    if (root()->count() > 0) {
        return;
    }
    // Deleted the last item on the root node, shrink the height of
    // the tree.
    if (root()->leaf()) {
        assert(size() == 0);
        delete_leaf_node(root());
        *mutable_root() = nullptr;
    } else {
        node_type* child = root()->child(0);
        if (child->leaf()) {
            // The child is a leaf node so simply make it the root node
            // in the tree.
            child->make_root();
            delete_internal_root_node();
            *mutable_root() = child;
        } else {
            // The child is an internal node. We want to keep the
            // existing root node so we move all of the values from
            // the child node into the existing (empty) root node.
            child->swap(root());
            delete_internal_node(child);
        }
    }
}


template <typename P> template <typename IterType>
inline IterType btree<P>::internal_last(IterType iter)
{
    while (iter.node && iter.position == iter.node->count()) {
        iter.position = iter.node->position();
        iter.node = iter.node->parent();
        if (iter.node->leaf()) {
            iter.node = nullptr;
        }
    }
    return iter;
}


template <typename P>
template <typename ... Ts>
inline typename btree<P>::iterator
btree<P>::internal_insert(iterator iter, Ts&&... ts)
{
    if (!iter.node->leaf()) {
        // We can't insert on an internal node. Instead, we'll insert
        // after the previous value which is guaranteed to be on a
        // leaf node.
        --iter;
        ++iter.position;
    }
    if (iter.node->count() == iter.node->max_count()) {
        // Make room in the leaf for the new item.
        if (iter.node->max_count() < node_values) {
            // Insertion into the root where the root is smaller that
            // the full node size. Simply grow the size of the root
            // node.
            assert(iter.node == root());
            iter.node = new_leaf_root_node(std::min<int>(node_values, 2 * iter.node->max_count()));
            iter.node->swap(root());
            delete_leaf_node(root());
            *mutable_root() = iter.node;
        } else {
            rebalance_or_split(&iter);
            ++*mutable_size();
        }
    } else if (!root()->leaf()) {
        ++*mutable_size();
    }
    iter.node->insert_value(iter.position, std::forward<Ts>(ts)...);
    return iter;
}


template <typename P> template <typename IterType>
inline pair<IterType, int> btree<P>::internal_locate(const key_type& key, IterType iter) const
{
    return internal_locate_type::dispatch(key, *this, iter);
}


template <typename P> template <typename IterType>
inline pair<IterType, int> btree<P>::internal_locate_plain_compare(const key_type& key, IterType iter) const
{
    for (;;) {
        iter.position = iter.node->lower_bound(key, key_comp());
        if (iter.node->leaf()) {
            break;
        }
        iter.node = iter.node->child(iter.position);
    }
    return std::make_pair(iter, 0);
}


template <typename P> template <typename IterType>
inline pair<IterType, int> btree<P>::internal_locate_compare_to(const key_type& key, IterType iter) const
{
    for (;;) {
        int res = iter.node->lower_bound(key, key_comp());
        iter.position = res & match_mask;
        if (res & exact_match) {
            return std::make_pair(iter, static_cast<int>(exact_match));
        }
        if (iter.node->leaf()) {
            break;
        }
        iter.node = iter.node->child(iter.position);
    }
    return std::make_pair(iter, -exact_match);
}


template <typename P> template <typename IterType>
IterType btree<P>::internal_lower_bound(const key_type& key, IterType iter) const
{
    if (iter.node) {
        for (;;) {
            iter.position = iter.node->lower_bound(key, key_comp()) & match_mask;
            if (iter.node->leaf()) {
                break;
            }
            iter.node = iter.node->child(iter.position);
        }
        iter = internal_last(iter);
    }
    return iter;
}


template <typename P> template <typename IterType>
IterType btree<P>::internal_upper_bound(const key_type& key, IterType iter) const
{
    if (iter.node) {
        for (;;) {
            iter.position = iter.node->upper_bound(key, key_comp());
            if (iter.node->leaf()) {
                break;
            }
            iter.node = iter.node->child(iter.position);
        }
        iter = internal_last(iter);
    }
    return iter;
}


template <typename P> template <typename IterType>
IterType btree<P>::internal_find_unique(const key_type& key, IterType iter) const
{
    if (iter.node) {
        pair<IterType, int> res = internal_locate(key, iter);
        if (res.second == exact_match) {
            return res.first;
        }
        if (!res.second) {
            iter = internal_last(res.first);
            if (iter.node && !compare_keys(key, iter.key())) {
                return iter;
            }
        }
    }
    return IterType(nullptr, 0);
}


template <typename P> template <typename IterType>
IterType btree<P>::internal_find_multi(const key_type& key, IterType iter) const
{
    if (iter.node) {
        iter = internal_lower_bound(key, iter);
        if (iter.node) {
            iter = internal_last(iter);
            if (iter.node && !compare_keys(key, iter.key())) {
                return iter;
            }
        }
    }
    return IterType(nullptr, 0);
}


template <typename P>
void btree<P>::internal_clear(node_type* node)
{
    if (!node->leaf()) {
        for (int i = 0; i <= node->count(); ++i) {
            internal_clear(node->child(i));
        }
        if (node == root()) {
            delete_internal_root_node();
        } else {
            delete_internal_node(node);
        }
    } else {
        delete_leaf_node(node);
    }
}


template <typename P>
void btree<P>::internal_dump(std::ostream &os, const node_type* node, int level) const
{
    for (int i = 0; i < node->count(); ++i) {
        if (!node->leaf()) {
            internal_dump(os, node->child(i), level + 1);
        }
        for (int j = 0; j < level; ++j) {
            os << "  ";
        }
        os << node->key(i) << " [" << level << "]\n";
    }
    if (!node->leaf()) {
        internal_dump(os, node->child(node->count()), level + 1);
    }
}


template <typename P>
int btree<P>::internal_verify(const node_type* node, const key_type* lo, const key_type* hi) const
{
    assert(node->count() > 0);
    assert(node->count() <= node->max_count());
    if (lo) {
        assert(!compare_keys(node->key(0), *lo));
    }
    if (hi) {
        assert(!compare_keys(*hi, node->key(node->count() - 1)));
    }
    for (int i = 1; i < node->count(); ++i) {
        assert(!compare_keys(node->key(i), node->key(i - 1)));
    }
    int count = node->count();
    if (!node->leaf()) {
        for (int i = 0; i <= node->count(); ++i) {
            assert(node->child(i) != nullptr);
            assert(node->child(i)->parent() == node);
            assert(node->child(i)->position() == i);
            count += internal_verify(
                node->child(i),
                (i == 0) ? lo : &node->key(i - 1),
                (i == node->count()) ? hi : &node->key(i)
            );
        }
    }
    return count;
}

// BTREE CONTAINER

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const btree_container<T>& b)
{
    b.dump(os);
    return os;
}

}   /* btree_detail */

PYCPP_END_NAMESPACE
