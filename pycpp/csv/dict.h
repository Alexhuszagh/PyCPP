//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hashmap-based CSV readers and writers.
 */

#pragma once

#include <pycpp/collections/ordered_dict.h>
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
    csv_dict_stream_reader(csvpunct_impl* = nullptr);
    csv_dict_stream_reader(std::istream&, size_t skip = 0, csvpunct_impl* = nullptr);
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
    csv_stream_reader reader_;
    csv_indexes header_;
};


/**
 *  \brief Dict reader for file-based document.
 */
struct csv_dict_file_reader: csv_dict_stream_reader
{
public:
    csv_dict_file_reader(csvpunct_impl* = nullptr);
    csv_dict_file_reader(const std::string &name, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const std::string &name, size_t skip = 0, csvpunct_impl* = nullptr);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    csv_dict_file_reader(const std::wstring &name, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const std::wstring &name, size_t skip = 0, csvpunct_impl* = nullptr);
    csv_dict_file_reader(const std::u16string &name, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const std::u16string &name, size_t skip = 0, csvpunct_impl* = nullptr);
#endif                                          // WINDOWS

private:
    ifstream file_;
};


/**
 *  \brief Dict reader for string-based document.
 */
struct csv_dict_string_reader: csv_dict_stream_reader
{
public:
    csv_dict_string_reader(csvpunct_impl* = nullptr);
    csv_dict_string_reader(const std::string &str, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const std::string &str, size_t skip = 0, csvpunct_impl* = nullptr);

private:
    std::istringstream sstream_;
};

// WRITER


/**
 *  \brief Generic writer for CSV file.
 *
 *  This reader is mostly analogous to Python's `csv.DictWriter` object.
 */
struct csv_dict_stream_writer
{
public:
    // MEMBER TYPES
    // ------------
    using value_type = csv_map;
    using reference = value_type&;

    // MEMBER FUNCTIONS
    // ----------------
    csv_dict_stream_writer(csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    csv_dict_stream_writer(std::ostream&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    void open(std::ostream&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    void punctuation(csvpunct_impl*);
    const csvpunct_impl* punctuation() const;
    void quoting(csv_quoting);
    const csv_quoting quoting() const;

    // DATA
    void operator()(const value_type& row);

private:
    csv_stream_writer writer_;
    csv_indexes header_;
};


/**
 *  \brief Writer for file-based document.
 */
struct csv_dict_file_writer: csv_dict_stream_writer
{
public:
    csv_dict_file_writer(csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    csv_dict_file_writer(const std::string&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    void open(const std::string&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    csv_dict_file_writer(const std::wstring&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    void open(const std::wstring&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    csv_dict_file_writer(const std::u16string&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    void open(const std::u16string&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
#endif                                          // WINDOWS

private:
    ofstream file_;
};


/**
 *  \brief Writer for string-based document.
 */
struct csv_dict_string_writer: csv_dict_stream_writer
{
public:
    csv_dict_string_writer(csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    csv_dict_string_writer(const csv_row& header, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    void open(const csv_row& header, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    std::string str() const;

private:
    std::ostringstream sstream_;
};

PYCPP_END_NAMESPACE
