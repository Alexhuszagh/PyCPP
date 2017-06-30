//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Transformable stream overloads.
 *
 *  Provides I/O transformation in a stream-like wrapper using a callback,
 *  `transform_callback`, which transforms either input or output data
 *  from an underlying file buffer.
 */

#pragma once

#include "fstream.h"
#include <functional>
#include <iostream>
#include <sstream>
#include <utility>

// OBJECTS
// -------

/**
 *  \brief
 */
typedef std::pair<std::streamsize, std::streamsize> streamsize_pair;

/**
 *  \brief Generic callback to convert bytes from input to output buffers.
 *
 *  Converts up to `srclen` chars from `src` to up to `dstlen` chars
 *  in `dst`, returning a pair containing the number of bytes read
 *  (`first`) and the number of bytes written (`second`).
 */
typedef std::function<streamsize_pair(
    const void* src, size_t srclen,
    void* dst, size_t dstlen,
    size_t char_size)>
transform_callback;


/**
 *  \brief String buffer that transforms input to output data.
 */
class transform_streambuf: public std::streambuf
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
    transform_streambuf(std::streambuf* = nullptr, transform_callback = nullptr);
    ~transform_streambuf();

    transform_streambuf(const transform_streambuf&) = delete;
    transform_streambuf& operator=(const transform_streambuf&) = delete;
    transform_streambuf(transform_streambuf&&);
    transform_streambuf& operator=(transform_streambuf&&);
    void swap(transform_streambuf&);

    void set_filebuf(std::streambuf*);
    void set_callback(transform_callback);

protected:
    // MEMBER FUNCTIONS
    // ----------------
    virtual int_type underflow();
    virtual int_type overflow(int_type = traits_type::eof());
    virtual int sync();
    // TODO: Might need to implement seekoff, seekpos, etc.

private:
    friend class transform_istream;
    friend class transform_ostream;

    std::streambuf *filebuf = nullptr;
    transform_callback callback = nullptr;
    char* in_buffer = nullptr;
    char* in_first = nullptr;
    char* in_last = nullptr;
    char* out_buffer = nullptr;
};


/**
 *  \brief Transform streaming input data via callback.
 */
class transform_istream: public std::istream
{
public:
    transform_istream(transform_callback = nullptr);
    ~transform_istream();
    transform_istream(const transform_istream&) = delete;
    transform_istream & operator=(const transform_istream&) = delete;

    transform_istream(std::istream& stream, transform_callback = nullptr);
    void open(std::istream& stream, transform_callback = nullptr);

protected:
    transform_istream(transform_istream&&);
    transform_istream & operator=(transform_istream&&);
    transform_streambuf* rdbuf() const;
    void rdbuf(transform_streambuf *buffer);
    void swap(transform_istream &other);

private:
    transform_streambuf buffer;
    std::istream *stream = nullptr;
};


/**
 *  \brief Transform streaming output data via callback.
 */
class transform_ostream: public std::ostream
{
public:
    transform_ostream(transform_callback = nullptr);
    ~transform_ostream();
    transform_ostream(const transform_ostream&) = delete;
    transform_ostream & operator=(const transform_ostream&) = delete;

    transform_ostream(std::ostream& stream, transform_callback = nullptr);
    void open(std::ostream& stream, transform_callback = nullptr);

protected:
    transform_ostream(transform_ostream&&);
    transform_ostream & operator=(transform_ostream&&);
    transform_streambuf* rdbuf() const;
    void rdbuf(transform_streambuf *buffer);
    void swap(transform_ostream &other);

private:
    transform_streambuf buffer;
    std::ostream *stream = nullptr;
};


/**
 *  \brief File like overload for istream.
 */
class transform_ifstream: public transform_istream
{
public:
    transform_ifstream(transform_callback = nullptr);
    ~transform_ifstream();

    transform_ifstream(const transform_ifstream&) = delete;
    transform_ifstream & operator=(const transform_ifstream&) = delete;
    transform_ifstream(transform_ifstream&&);
    transform_ifstream & operator=(transform_ifstream&&);

    transform_ifstream(const std::string &name, std::ios_base::openmode = std::ios_base::in, transform_callback = nullptr);
    void open(const std::string &name, std::ios_base::openmode = std::ios_base::in, transform_callback = nullptr);

#if defined(HAVE_WFOPEN)
    transform_ifstream(const std::wstring &name, std::ios_base::openmode = std::ios_base::in, transform_callback = nullptr);
    void open(const std::wstring &name, std::ios_base::openmode = std::ios_base::in, transform_callback = nullptr);
#endif

    bool is_open() const;
    void close();
    void swap(transform_ifstream&);

private:
    ifstream file;
};


/**
 *  \brief File like overload for ostream.
 */
class transform_ofstream: public transform_ostream
{
public:
    transform_ofstream(transform_callback = nullptr);
    ~transform_ofstream();

    transform_ofstream(const transform_ofstream&) = delete;
    transform_ofstream & operator=(const transform_ofstream&) = delete;
    transform_ofstream(transform_ofstream&&);
    transform_ofstream & operator=(transform_ofstream&&);

    transform_ofstream(const std::string &name, std::ios_base::openmode = std::ios_base::out, transform_callback = nullptr);
    void open(const std::string &name, std::ios_base::openmode = std::ios_base::out, transform_callback = nullptr);

#if defined(HAVE_WFOPEN)
    transform_ofstream(const std::wstring &name, std::ios_base::openmode = std::ios_base::out, transform_callback = nullptr);
    void open(const std::wstring &name, std::ios_base::openmode = std::ios_base::out, transform_callback = nullptr);
#endif

    bool is_open() const;
    void close();
    void swap(transform_ofstream&);

private:
    ofstream file;
};
