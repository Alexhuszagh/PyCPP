//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Filtering stream overloads.
 *
 *  Provides I/O transformation in a stream-like wrapper using a callback,
 *  `filter_callback`, which transforms either input or output data
 *  from an underlying file buffer.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/stream/fstream.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <utility>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Generic callback to convert bytes from input to output buffers.
 *
 *  Converts up to `srclen` chars from `src` to up to `dstlen` chars
 *  in `dst`, returning a pair containing the number of bytes read
 *  (`first`) and the number of bytes written (`second`).
 */
typedef std::function<void(
    const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    size_t char_size)>
filter_callback;


/**
 *  \brief String buffer that transforms input to output data.
 */
class filter_streambuf: public std::streambuf
{
public:
    // MEMBER TYPES
    // ------------
    using typename std::streambuf::char_type;
    using typename std::streambuf::int_type;
    using typename std::streambuf::traits_type;
    using typename std::streambuf::off_type;
    using typename std::streambuf::pos_type;

    // MEMBER VARIABLES
    // ----------------
    static constexpr size_t buffer_size = 4096;

    // MEMBER FUNCTIONS
    // ----------------
    filter_streambuf(std::streambuf* = nullptr, filter_callback = nullptr);
    ~filter_streambuf();

    filter_streambuf(const filter_streambuf&) = delete;
    filter_streambuf& operator=(const filter_streambuf&) = delete;
    filter_streambuf(filter_streambuf&&);
    filter_streambuf& operator=(filter_streambuf&&);
    void swap(filter_streambuf&);

    void set_filebuf(std::streambuf*);
    void set_callback(filter_callback);

protected:
    // MEMBER FUNCTIONS
    // ----------------
    virtual int_type underflow();
    virtual int_type overflow(int_type = traits_type::eof());
    virtual int sync();
    pos_type seekoff(off_type, std::ios_base::seekdir, std::ios_base::openmode = std::ios_base::in | std::ios_base::out);
    pos_type seekpos(pos_type, std::ios_base::openmode = std::ios_base::in | std::ios_base::out);

private:
    friend class filter_istream;
    friend class filter_ostream;

    std::streambuf *filebuf = nullptr;
    filter_callback callback = nullptr;
    char_type* in_buffer = nullptr;
    char_type* out_buffer = nullptr;
    char_type* in_first = nullptr;
    char_type* in_last = nullptr;
};


/**
 *  \brief Transform streaming input data via callback.
 */
class filter_istream: public std::istream
{
public:
    filter_istream(filter_callback = nullptr);
    ~filter_istream();
    filter_istream(const filter_istream&) = delete;
    filter_istream & operator=(const filter_istream&) = delete;

    filter_istream(std::istream& stream, filter_callback = nullptr);
    void open(std::istream& stream, filter_callback = nullptr);
    filter_streambuf* rdbuf() const;
    void rdbuf(filter_streambuf *buffer);

protected:
    filter_istream(filter_istream&&);
    filter_istream & operator=(filter_istream&&);
    void swap(filter_istream &other);

private:
    filter_streambuf buffer;
    std::istream *stream = nullptr;
};


/**
 *  \brief Transform streaming output data via callback.
 */
class filter_ostream: public std::ostream
{
public:
    filter_ostream(filter_callback = nullptr);
    ~filter_ostream();
    filter_ostream(const filter_ostream&) = delete;
    filter_ostream & operator=(const filter_ostream&) = delete;

    filter_ostream(std::ostream& stream, filter_callback = nullptr);
    void open(std::ostream& stream, filter_callback = nullptr);
    filter_streambuf* rdbuf() const;
    void rdbuf(filter_streambuf *buffer);

protected:
    filter_ostream(filter_ostream&&);
    filter_ostream & operator=(filter_ostream&&);
    void swap(filter_ostream &other);

private:
    filter_streambuf buffer;
    std::ostream *stream = nullptr;
};


/**
 *  \brief File like overload for istream.
 */
class filter_ifstream: public filter_istream
{
public:
    filter_ifstream(filter_callback = nullptr);
    ~filter_ifstream();

    filter_ifstream(const filter_ifstream&) = delete;
    filter_ifstream & operator=(const filter_ifstream&) = delete;
    filter_ifstream(filter_ifstream&&);
    filter_ifstream & operator=(filter_ifstream&&);

    filter_ifstream(const std::string &name, std::ios_base::openmode = std::ios_base::in, filter_callback = nullptr);
    void open(const std::string &name, std::ios_base::openmode = std::ios_base::in, filter_callback = nullptr);

#if defined(PYCPP_HAVE_WFOPEN)
    filter_ifstream(const std::wstring &name, std::ios_base::openmode = std::ios_base::in, filter_callback = nullptr);
    void open(const std::wstring &name, std::ios_base::openmode = std::ios_base::in, filter_callback = nullptr);
#endif

    bool is_open() const;
    void close();
    void swap(filter_ifstream&);
    using filter_istream::rdbuf;

private:
    ifstream file;
};


/**
 *  \brief File like overload for ostream.
 */
class filter_ofstream: public filter_ostream
{
public:
    filter_ofstream(filter_callback = nullptr);
    ~filter_ofstream();

    filter_ofstream(const filter_ofstream&) = delete;
    filter_ofstream & operator=(const filter_ofstream&) = delete;
    filter_ofstream(filter_ofstream&&);
    filter_ofstream & operator=(filter_ofstream&&);

    filter_ofstream(const std::string &name, std::ios_base::openmode = std::ios_base::out, filter_callback = nullptr);
    void open(const std::string &name, std::ios_base::openmode = std::ios_base::out, filter_callback = nullptr);

#if defined(PYCPP_HAVE_WFOPEN)
    filter_ofstream(const std::wstring &name, std::ios_base::openmode = std::ios_base::out, filter_callback = nullptr);
    void open(const std::wstring &name, std::ios_base::openmode = std::ios_base::out, filter_callback = nullptr);
#endif

    bool is_open() const;
    void close();
    void swap(filter_ofstream&);
    using filter_ostream::rdbuf;

private:
    ofstream file;
};

PYCPP_END_NAMESPACE
