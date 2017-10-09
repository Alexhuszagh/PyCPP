//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hashmap-based CSV readers and writers.
 */

#pragma once

#include <pycpp/collections/ordereddict.h>
#include <pycpp/csv/reader.h>
#include <pycpp/csv/writer.h>

#include <unordered_map>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using csv_indexes = ordered_map<std::string, size_t>;
using csv_map = std::unordered_map<std::string, std::string>;

// OBJECTS
// -------

// READER

/**
 *  \brief Hashmap-based reader for CSF file.
 *
 *  This reader is mostly analogous to Python's `csv.DictReader` object.
 */
struct csv_dict_stream_reader
{
public:
    // MEMBER TYPES
    // ------------
    using value_type = csv_map;
    using reference = value_type&;
    using iterator = input_iterator_facade<csv_dict_stream_reader>;

    // MEMBER FUNCTIONS
    // ----------------
    csv_dict_stream_reader();
    csv_dict_stream_reader(std::istream&, size_t skip = 0);
    void parse(std::istream&, size_t skip = 0);
    void punctuation(csvpunct_impl*);
    const csvpunct_impl* punctuation() const;

    // DATA
    value_type operator()();
    bool eof() const;
    explicit operator bool() const;

    // ITERATORS
    iterator begin();
    iterator end();

private:
    csv_stream_reader reader_;
    csv_indexes header_;
};

// WRITER

// TODO: need the stream writer

PYCPP_END_NAMESPACE
