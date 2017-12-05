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

#include <pycpp/config.h>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <new>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>
#include <sys/types.h>

PYCPP_BEGIN_NAMESPACE

namespace btree_detail
{
// DETAIL
// ------

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

//TODO: missing
// TODO: if_ is just std::conditional


// A helper type used to indicate that a key-compare-to functor has been
// provided. A user can specify a key-compare-to functor by doing:
//
//  struct MyStringComparer
//      : public util::btree::btree_key_compare_to_tag {
//    int operator()(const string &a, const string &b) const {
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
struct btree_is_key_compare_to: public std::is_convertible<Compare, btree_key_compare_to_tag>
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

    btree_key_compare_to_adapter(const Compare &c):
        Compare(c)
    {}

    btree_key_compare_to_adapter(const btree_key_compare_to_adapter<Compare> &c):
        Compare(c)
    {}
};


template <>
struct btree_key_compare_to_adapter<std::less<std::string>>: public btree_key_compare_to_tag
{
    btree_key_compare_to_adapter()
    {}

    btree_key_compare_to_adapter(const std::less<std::string>&)
    {}

    btree_key_compare_to_adapter(const btree_key_compare_to_adapter<std::less<std::string>>&)
    {}

    int operator()(const std::string& a, const std::string& b) const
    {
        return a.compare(b);
    }
};


template <>
struct btree_key_compare_to_adapter<std::greater<std::string>>: public btree_key_compare_to_tag
{
    btree_key_compare_to_adapter()
    {}

    btree_key_compare_to_adapter(const std::greater<std::string>&)
    {}

    btree_key_compare_to_adapter(const btree_key_compare_to_adapter<std::greater<std::string>>&)
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

    static bool bool_compare(const Compare &comp, const Key &x, const Key &y)
    {
        return comp(x, y);
    }

    bool operator()(const Key &x, const Key &y) const
    {
        return bool_compare(comp, x, y);
    }

    Compare comp;
};

// A specialization of btree_key_comparer when a compare-to functor is
// present. We need a plain (boolean) comparison in some parts of the btree
// code, such as insert-with-hint.
template <typename Key, typename Compare>
struct btree_key_comparer<Key, Compare, true>
{
    btree_key_comparer()
    {}

    btree_key_comparer(Compare c):
        comp(c)
    {}

    static bool bool_compare(const Compare &comp, const Key &x, const Key &y)
    {
        return comp(x, y) < 0;
    }

    bool operator()(const Key &x, const Key &y) const
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
static bool btree_compare_keys(const Compare &comp, const Key &x, const Key &y)
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
    // If Compare is derived from btree_key_compare_to_tag then use it as the
    // key_compare type. Otherwise, use btree_key_compare_to_adapter<> which will
    // fall-back to Compare if we don't have an appropriate specialization.
    using key_compare = typename std::conditional<
        btree_is_key_compare_to<Compare>::value,
        Compare,
        btree_key_compare_to_adapter<Compare>
    >::type;
    // A type which indicates if we have a key-compare-to functor or a plain old
    // key-compare functor.
    using is_key_compare_to = btree_is_key_compare_to<key_compare>;
    using allocator_type = Alloc;
    using key_type = Key;
    using size_type = intptr_t;
    using difference_type = ptrdiff_t;

    enum {
        target_node_size = TargetNodeSize,

        // Available space for values.  This is largest for leaf nodes,
        // which has overhead no fewer than two pointers.
        node_value_soace = TargetNodeSize - 2 * sizeof(void*),
    };

    // This is an integral type large enough to hold as many
    // ValueSize-values as will fit a node of TargetNodeSize bytes.
    using node_count_type = typename std::conditional<
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
    using value_type = std::pair<const Key, data_type>;
    using mutable_value_type = std::pair<Key, data_type>;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;

    enum {
        value_size = sizeof(Key) + sizeof(data_type),
    };

    static const Key& key(const value_type &x)
    {
        return x.first;
    }

    static const Key& key(const mutable_value_type &x)
    {
        return x.first;
    }


    static void swap(mutable_value_type *a, mutable_value_type *b)
    {
        btree_swap_helper(a->first, b->first);
        btree_swap_helper(a->second, b->second);
    }
};


// A parameters structure for holding the type parameters for a btree_set.
template <typename Key, typename Compare, typename Alloc, int TargetNodeSize>
struct btree_set_params: public btree_common_params<Key, Compare, Alloc, TargetNodeSize,sizeof(Key)>
{
    using data_type = std::false_type;
    using mapped_type = std::false_type;
    using value_type = Key;
    using mutable_value_type = value_type;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;

    enum {
        value_size = sizeof(Key),
    };

    static const Key& key(const value_type &x)
    {
        return x;
    }

    static void swap(mutable_value_type *a, mutable_value_type *b)
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

    bool operator()(const Key &a, const Key &b) const
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

    int operator()(const Key &a, const Key &b) const
    {
        return static_cast<const CompareTo&>(*this)(b, a);
    }
};

}   /* btree_detail */

PYCPP_END_NAMESPACE
