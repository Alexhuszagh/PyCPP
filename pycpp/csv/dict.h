//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hashmap-based CSV readers and writers.
 */

#pragma once

#include <pycpp/collections/ordered_map.h>
#include <pycpp/csv/reader.h>
#include <pycpp/csv/writer.h>
#include <pycpp/stl/unordered_map.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using csv_indexes = ordered_map<string, size_t>;
using csv_map = unordered_map<string, string>;

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
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = input_iterator_facade<csv_dict_stream_reader>;

    // MEMBER FUNCTIONS
    // ----------------
    csv_dict_stream_reader(csvpunct_impl* = nullptr);
    csv_dict_stream_reader(const csv_dict_stream_reader&) = delete;
    csv_dict_stream_reader& operator=(const csv_dict_stream_reader&) = delete;
    csv_dict_stream_reader(csv_dict_stream_reader&&);
    csv_dict_stream_reader& operator=(csv_dict_stream_reader&&);

    // STREAM
    csv_dict_stream_reader(istream&, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(istream&, size_t skip = 0, csvpunct_impl* = nullptr);

    // PROPERTIES/MODIFIERS
    void punctuation(csvpunct_impl*);
    const csvpunct_impl* punctuation() const;
    void swap(csv_dict_stream_reader&);

    // DATA
    value_type operator()();
    bool eof() const;
    explicit operator bool() const;

    // ITERATORS
    iterator begin();
    iterator end();

protected:
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
    csv_dict_file_reader(const csv_dict_file_reader&) = delete;
    csv_dict_file_reader& operator=(const csv_dict_file_reader&) = delete;
    csv_dict_file_reader(csv_dict_file_reader&&);
    csv_dict_file_reader& operator=(csv_dict_file_reader&&);

    // STREAM
    csv_dict_file_reader(const string_view& name, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const string_view& name, size_t skip = 0, csvpunct_impl* = nullptr);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    csv_dict_file_reader(const wstring_view& name, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const wstring_view& name, size_t skip = 0, csvpunct_impl* = nullptr);
    csv_dict_file_reader(const u16string_view& name, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const u16string_view& name, size_t skip = 0, csvpunct_impl* = nullptr);
#endif                                          // WINDOWS

    // MODIFIERS
    void swap(csv_dict_file_reader&);

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
    csv_dict_string_reader(const csv_dict_string_reader&) = delete;
    csv_dict_string_reader& operator=(const csv_dict_string_reader&) = delete;
    csv_dict_string_reader(csv_dict_string_reader&&);
    csv_dict_string_reader& operator=(csv_dict_string_reader&&);

    // STREAM
    csv_dict_string_reader(const string_wrapper& str, size_t skip = 0, csvpunct_impl* = nullptr);
    void open(const string_wrapper& str, size_t skip = 0, csvpunct_impl* = nullptr);

    // MODIFIERS
    void swap(csv_dict_string_reader&);

private:
    istringstream sstream_;
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
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;

    // MEMBER FUNCTIONS
    // ----------------
    csv_dict_stream_writer(csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    csv_dict_stream_writer(const csv_dict_stream_writer&) = delete;
    csv_dict_stream_writer& operator=(const csv_dict_stream_writer&) = delete;
    csv_dict_stream_writer(csv_dict_stream_writer&&);
    csv_dict_stream_writer& operator=(csv_dict_stream_writer&&);

    // STREAM
    csv_dict_stream_writer(ostream&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    void open(ostream&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);

    // PROPERTIES/MODIFIERS
    void punctuation(csvpunct_impl*);
    const csvpunct_impl* punctuation() const;
    void quoting(csv_quoting);
    const csv_quoting quoting() const;
    void swap(csv_dict_stream_writer&);

    // DATA
    void operator()(const value_type& row);

protected:
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
    csv_dict_file_writer(const csv_dict_file_writer&) = delete;
    csv_dict_file_writer& operator=(const csv_dict_file_writer&) = delete;
    csv_dict_file_writer(csv_dict_file_writer&&);
    csv_dict_file_writer& operator=(csv_dict_file_writer&&);

    // STREAM
    csv_dict_file_writer(const string_view&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    void open(const string_view&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    csv_dict_file_writer(const wstring_view&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    void open(const wstring_view&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    csv_dict_file_writer(const u16string_view&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    void open(const u16string_view&, const csv_row&, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
#endif                                          // WINDOWS

    // MODIFIERS
    void swap(csv_dict_file_writer&);

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
    csv_dict_string_writer(const csv_dict_string_writer&) = delete;
    csv_dict_string_writer& operator=(const csv_dict_string_writer&) = delete;
    csv_dict_string_writer(csv_dict_string_writer&&);
    csv_dict_string_writer& operator=(csv_dict_string_writer&&);

    // STREAM
    csv_dict_string_writer(const csv_row& header, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    void open(const csv_row& header, csv_quoting = CSV_QUOTE_MINIMAL, csvpunct_impl* = nullptr);
    string str() const;

    // MODIFIERS
    void swap(csv_dict_string_writer&);

private:
    ostringstream sstream_;
};

PYCPP_END_NAMESPACE
