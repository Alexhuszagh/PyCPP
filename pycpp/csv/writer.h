//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief CSV base writer.
 */

#pragma once

#include <pycpp/csv/punct.h>
#include <pycpp/stream/fstream.h>

#include <memory>
#include <sstream>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Generic writer for CSV file.
 *
 *  This reader is mostly analogous to Python's `csv.writer` object, with
 *  one notable exception: it overloads `operator()()` to write a row,
 *  rather than `writerow()`.
 *
 *  The punctation can be altered similar to an STL locale, The
 *  CSV reader takes ownership of the punct object.
 *
 *  The quoting preferences may also similarly be changed, from quoting all fields
 *  (CSV_QUOTE_ALL), to only quoting fields as required (CSV_QUOTE_MINIMAL),
 *  to quoting all non-numeric fields
 */
struct csv_stream_writer
{
public:
    // MEMBER TYPES
    // ------------
    using value_type = csv_row;
    using reference = value_type&;

    // MEMBER FUNCTIONS
    // ----------------
    csv_stream_writer(csv_quoting quoting = CSV_QUOTE_MINIMAL);
    csv_stream_writer(std::ostream&, csv_quoting quoting = CSV_QUOTE_MINIMAL);
    void open(std::ostream&);
    void punctuation(csvpunct_impl*);
    const csvpunct_impl* punctuation() const;
    void quoting(csv_quoting);
    const csv_quoting quoting() const;

    // DATA
    void operator()(const value_type& row);

private:
    std::ostream* stream_ = nullptr;
    csv_quoting quoting_ = CSV_QUOTE_MINIMAL;
    std::shared_ptr<csvpunct_impl> punct_;
};

PYCPP_END_NAMESPACE
