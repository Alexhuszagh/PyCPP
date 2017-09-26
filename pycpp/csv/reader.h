//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief CSV base reader.
 */

#pragma once

#include <pycpp/collections/ordereddict.h>
#include <pycpp/csv/punct.h>
#include <pycpp/iterator/input_iterator_facade.h>
#include <string>
#include <vector>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using csv_row = std::vector<std::string>;
using csv_indexes = ordered_map<std::string, size_t>;

// OBJECTS
// -------

/**
 *  \brief Generic reader for CSV file.
 *
 *  This reader is mostly analogous to Python's `csv.reader` object, with
 *  one notable exception: it stores the CSV headers rather than treating
 *  them as a new row.
 */
struct csv_stream_reader
{
public:
    // MEMBER TYPES
    // ------------
    using value_type = csv_row;
    using reference = value_type&;
    using iterator = input_iterator_facade<csv_stream_reader>;

    // MEMBER FUNCTIONS
    // ----------------
    csv_stream_reader(const csvpunct&);
    csv_stream_reader(csvpunct&&);
    csv_stream_reader(const csvpunct&, std::istream&, size_t skip = 0);
    csv_stream_reader(csvpunct&&, std::istream&, size_t skip = 0);
    void parse(std::istream&, size_t skip = 0);

    // DATA
    value_type operator()();
    bool eof() const;
    explicit operator bool() const;

    // ITERATORS
    iterator begin();
    iterator end();

private:
    std::istream* stream_ = nullptr;
    csvpunct punct_;
    csv_indexes header_;
};

// TODO: csv_file_reader
// TODO: csv_string_reader

PYCPP_END_NAMESPACE
