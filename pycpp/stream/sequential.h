//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Sequential file-backed stream definitions.
 *
 *  Sequential hints for stream behavior significantly improve
 *  performance for sequentially-read files by increasing
 *  read-ahead (doubling it on POSIX systems).
 */

#pragma once

#include <pycpp/stl/string_view.h>
#include <pycpp/stream/fd.h>

PYCPP_BEGIN_NAMESPACE

// VARIABLES
// ---------

extern size_t SEQUENTIAL_BUFFER_SIZE;

// OBJECTS
// -------


/**
 *  \brief Stream wrapping a sequential I/O file.
 */
class sequential_fstream: public iostream
{
public:
    sequential_fstream();
    ~sequential_fstream();
    sequential_fstream(const sequential_fstream&) = delete;
    sequential_fstream & operator=(const sequential_fstream&) = delete;
    sequential_fstream(sequential_fstream &&other);
    sequential_fstream & operator=(sequential_fstream &&other);

    sequential_fstream(const string_view& name, ios_base::openmode mode = ios_base::in | ios_base::out);
    void open(const string_view& name, ios_base::openmode mode = ios_base::in | ios_base::out);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    sequential_fstream(const wstring_view& name, ios_base::openmode mode = ios_base::in | ios_base::out);
    void open(const wstring_view& name, ios_base::openmode mode = ios_base::in | ios_base::out);
    sequential_fstream(const u16string_view& name, ios_base::openmode mode = ios_base::in | ios_base::out);
    void open(const u16string_view& name, ios_base::openmode mode = ios_base::in | ios_base::out);
#endif                                          // WINDOWS

    // DATA
    bool is_open() const;
    void close();
    void swap(sequential_fstream &other);

private:
    fd_streambuf buffer;
};


/**
 *  \brief Stream wrapping a sequential input file.
 */
class sequential_ifstream: public istream
{
public:
    sequential_ifstream();
    ~sequential_ifstream();
    sequential_ifstream(const sequential_ifstream&) = delete;
    sequential_ifstream & operator=(const sequential_ifstream&) = delete;
    sequential_ifstream(sequential_ifstream &&other);
    sequential_ifstream & operator=(sequential_ifstream &&other);

    sequential_ifstream(const string_view& name, ios_base::openmode mode = ios_base::in);
    void open(const string_view& name, ios_base::openmode mode = ios_base::in);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    sequential_ifstream(const wstring_view& name, ios_base::openmode mode = ios_base::in);
    void open(const wstring_view& name, ios_base::openmode mode = ios_base::in);
    sequential_ifstream(const u16string_view& name, ios_base::openmode mode = ios_base::in);
    void open(const u16string_view& name, ios_base::openmode mode = ios_base::in);
#endif                                          // WINDOWS

    // DATA
    bool is_open() const;
    void close();
    void swap(sequential_ifstream &other);

private:
    fd_streambuf buffer;
};


/**
 *  \brief Stream wrapping a sequential ouput file.
 */
class sequential_ofstream: public ostream
{
public:
    sequential_ofstream();
    ~sequential_ofstream();
    sequential_ofstream(const sequential_ofstream&) = delete;
    sequential_ofstream & operator=(const sequential_ofstream&) = delete;
    sequential_ofstream(sequential_ofstream &&other);
    sequential_ofstream & operator=(sequential_ofstream &&other);

    sequential_ofstream(const string_view& name, ios_base::openmode mode = ios_base::out);
    void open(const string_view& name, ios_base::openmode mode = ios_base::out);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    sequential_ofstream(const wstring_view& name, ios_base::openmode mode = ios_base::out);
    void open(const wstring_view& name, ios_base::openmode mode = ios_base::out);
    sequential_ofstream(const u16string_view& name, ios_base::openmode mode = ios_base::out);
    void open(const u16string_view& name, ios_base::openmode mode = ios_base::out);
#endif                                          // WINDOWS

    // DATA
    bool is_open() const;
    void close();
    void swap(sequential_ofstream &other);

private:
    fd_streambuf buffer;
};

PYCPP_END_NAMESPACE
