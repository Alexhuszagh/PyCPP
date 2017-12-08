//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Random-access file-backed stream definitions.
 *
 *  Random-access hints for stream behavior significantly
 *  improve performance for files using frequent seeks,
 *  by minimizing file read-ahead. On HDDs, however,
 *  seeks are prohibitively expensive so these hints
 *  will only provide limited benefit.
 */

#pragma once

#include <pycpp/stream/fd.h>
#include <pycpp/view/string.h>

PYCPP_BEGIN_NAMESPACE

// VARIABLES
// ---------

extern size_t RANDOM_ACCESS_BUFFER_SIZE;

// OBJECTS
// -------


/**
 *  \brief Stream wrapping a random-access I/O file.
 */
class random_access_fstream: public std::iostream
{
public:
    random_access_fstream();
    ~random_access_fstream();
    random_access_fstream(const random_access_fstream&) = delete;
    random_access_fstream & operator=(const random_access_fstream&) = delete;
    random_access_fstream(random_access_fstream &&other);
    random_access_fstream & operator=(random_access_fstream &&other);

    random_access_fstream(const string_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const string_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    random_access_fstream(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    random_access_fstream(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
#endif                                          // WINDOWS

    // DATA
    bool is_open() const;
    void close();
    void swap(random_access_fstream &other);

private:
    fd_streambuf buffer;
};


/**
 *  \brief Stream wrapping a random-access input file.
 */
class random_access_ifstream: public std::istream
{
public:
    random_access_ifstream();
    ~random_access_ifstream();
    random_access_ifstream(const random_access_ifstream&) = delete;
    random_access_ifstream & operator=(const random_access_ifstream&) = delete;
    random_access_ifstream(random_access_ifstream &&other);
    random_access_ifstream & operator=(random_access_ifstream &&other);

    random_access_ifstream(const string_view& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const string_view& name, std::ios_base::openmode mode = std::ios_base::in);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    random_access_ifstream(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::in);
    random_access_ifstream(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::in);
#endif                                          // WINDOWS

    // DATA
    bool is_open() const;
    void close();
    void swap(random_access_ifstream &other);

private:
    fd_streambuf buffer;
};


/**
 *  \brief Stream wrapping a random-access ouput file.
 */
class random_access_ofstream: public std::ostream
{
public:
    random_access_ofstream();
    ~random_access_ofstream();
    random_access_ofstream(const random_access_ofstream&) = delete;
    random_access_ofstream & operator=(const random_access_ofstream&) = delete;
    random_access_ofstream(random_access_ofstream &&other);
    random_access_ofstream & operator=(random_access_ofstream &&other);

    random_access_ofstream(const string_view& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const string_view& name, std::ios_base::openmode mode = std::ios_base::out);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    random_access_ofstream(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::out);
    random_access_ofstream(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::out);
#endif                                          // WINDOWS

    // DATA
    bool is_open() const;
    void close();
    void swap(random_access_ofstream &other);

private:
    fd_streambuf buffer;
};

PYCPP_END_NAMESPACE
