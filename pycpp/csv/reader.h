//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief CSV base reader.
 */

#pragma once

#include <pycpp/csv/punct.h>
#include <pycpp/iterator/input_iterator_facade.h>
#include <pycpp/stream/fstream.h>

#include <memory>
#include <sstream>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Generic reader for CSV file.
 *
 *  This reader is mostly analogous to Python's `csv.reader` object.
 *
 *  The punctation can be altered similar to an STL locale using `punctuation()`,
 *  the CSV reader takes ownership of the punct object.
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
    csv_stream_reader(csvpunct_impl* = nullptr);
    csv_stream_reader(std::istream&, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(std::istream&, size_t skip = 0, csvpunct_impl* = nullptr);
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
    std::istream* stream_ = nullptr;
    size_t row_length_ = 0;
    std::shared_ptr<csvpunct_impl> punct_;
};


/**
 *  \brief Reader for file-based document.
 */
struct csv_file_reader: csv_stream_reader
{
public:
    csv_file_reader(csvpunct_impl* = nullptr);
    csv_file_reader(const std::string &name, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const std::string &name, size_t skip = 0, csvpunct_impl* = nullptr);

#if defined(PYCPP_HAVE_WFOPEN)
    csv_file_reader(const std::wstring &name, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const std::wstring &name, size_t skip = 0, csvpunct_impl* = nullptr);
#endif

private:
    ifstream file_;
};


/**
 *  \brief Reader for string-based document.
 */
struct csv_string_reader: csv_stream_reader
{
public:
    csv_string_reader(csvpunct_impl* = nullptr);
    csv_string_reader(const std::string &str, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const std::string &str, size_t skip = 0, csvpunct_impl* = nullptr);

private:
    std::istringstream sstream_;
};

PYCPP_END_NAMESPACE
