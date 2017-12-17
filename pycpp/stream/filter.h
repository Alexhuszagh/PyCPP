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

#include <pycpp/stl/fstream.h>
#include <pycpp/stl/functional.h>
#include <pycpp/stl/iostream.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/stl/utility.h>
#include <pycpp/stl/string_view.h>

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
using filter_callback = function<void(
    const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    size_t char_size)
>;


/**
 *  \brief String buffer that transforms input to output data.
 */
class filter_streambuf: public streambuf
{
public:
    // MEMBER TYPES
    // ------------
    using typename streambuf::char_type;
    using typename streambuf::int_type;
    using typename streambuf::traits_type;
    using typename streambuf::off_type;
    using typename streambuf::pos_type;

    // MEMBER VARIABLES
    // ----------------
    static constexpr size_t buffer_size = 4096;

    // MEMBER FUNCTIONS
    // ----------------
    filter_streambuf(ios_base::openmode, streambuf* = nullptr, filter_callback = nullptr);
    virtual ~filter_streambuf();

    filter_streambuf(const filter_streambuf&) = delete;
    filter_streambuf& operator=(const filter_streambuf&) = delete;
    filter_streambuf(filter_streambuf&&);
    filter_streambuf& operator=(filter_streambuf&&);
    void close();
    void swap(filter_streambuf&);

    void set_filebuf(streambuf*);
    void set_callback(filter_callback);

protected:
    // MEMBER FUNCTIONS
    // ----------------
    virtual int_type underflow();
    virtual int_type overflow(int_type = traits_type::eof());
    virtual int sync();

private:
    void set_pointers();
    streamsize do_callback();

    friend class filter_istream;
    friend class filter_ostream;

    ios_base::openmode mode;
    streambuf *filebuf = nullptr;
    filter_callback callback = nullptr;
    char_type* in_buffer = nullptr;
    char_type* out_buffer = nullptr;
    char_type* first = nullptr;
    char_type* last = nullptr;
};


/**
 *  \brief Transform streaming input data via callback.
 */
class filter_istream: public istream
{
public:
    filter_istream(filter_callback = nullptr);
    filter_istream(const filter_istream&) = delete;
    filter_istream & operator=(const filter_istream&) = delete;
    ~filter_istream();

    // STREAM
    filter_istream(istream& stream, filter_callback = nullptr);
    void open(istream& stream, filter_callback = nullptr);
    void close();
    filter_streambuf* rdbuf() const;
    void rdbuf(filter_streambuf *buffer);

protected:
    // PROPERTIES/MODIFIERS
    filter_istream(filter_istream&&);
    filter_istream & operator=(filter_istream&&);
    void swap(filter_istream &other);
    using istream::tellg;
    using istream::seekg;

private:
    filter_streambuf buffer;
    istream *stream = nullptr;
};


/**
 *  \brief Transform streaming output data via callback.
 */
class filter_ostream: public ostream
{
public:
    filter_ostream(filter_callback = nullptr);
    filter_ostream(const filter_ostream&) = delete;
    filter_ostream & operator=(const filter_ostream&) = delete;
    ~filter_ostream();

    // STREAM
    filter_ostream(ostream& stream, filter_callback = nullptr);
    void open(ostream& stream, filter_callback = nullptr);
    void close();
    filter_streambuf* rdbuf() const;
    void rdbuf(filter_streambuf *buffer);

protected:
    // PROPERTIES/MODIFIERS
    filter_ostream(filter_ostream&&);
    filter_ostream & operator=(filter_ostream&&);
    void swap(filter_ostream &other);
    using ostream::tellp;
    using ostream::seekp;

private:
    filter_streambuf buffer;
    ostream *stream = nullptr;
};


/**
 *  \brief File like overload for istream.
 */
class filter_ifstream: public filter_istream
{
public:
    filter_ifstream(filter_callback = nullptr);
    filter_ifstream(const filter_ifstream&) = delete;
    filter_ifstream & operator=(const filter_ifstream&) = delete;
    filter_ifstream(filter_ifstream&&);
    filter_ifstream & operator=(filter_ifstream&&);
    ~filter_ifstream();

    // STREAM
    filter_ifstream(const string_view&, ios_base::openmode = ios_base::in, filter_callback = nullptr);
    void open(const string_view&, ios_base::openmode = ios_base::in, filter_callback = nullptr);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    filter_ifstream(const wstring_view&, ios_base::openmode = ios_base::in, filter_callback = nullptr);
    void open(const wstring_view&, ios_base::openmode = ios_base::in, filter_callback = nullptr);
    filter_ifstream(const u16string_view&, ios_base::openmode = ios_base::in, filter_callback = nullptr);
    void open(const u16string_view&, ios_base::openmode = ios_base::in, filter_callback = nullptr);
#endif                                          // WINDOWS

    // PROPERTIES/MODIFIERS
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

    // STREAM
    filter_ofstream(const string_view&, ios_base::openmode = ios_base::out, filter_callback = nullptr);
    void open(const string_view&, ios_base::openmode = ios_base::out, filter_callback = nullptr);
#if defined(HAVE_WFOPEN)                        // WINDOWS
    filter_ofstream(const wstring_view&, ios_base::openmode = ios_base::out, filter_callback = nullptr);
    void open(const wstring_view&, ios_base::openmode = ios_base::out, filter_callback = nullptr);
    filter_ofstream(const u16string_view&, ios_base::openmode = ios_base::out, filter_callback = nullptr);
    void open(const u16string_view&, ios_base::openmode = ios_base::out, filter_callback = nullptr);
#endif                                          // WINDOWS

    // PROPERTIES/MODIFIERS
    bool is_open() const;
    void close();
    void swap(filter_ofstream&);
    using filter_ostream::rdbuf;

private:
    ofstream file;
};

PYCPP_END_NAMESPACE
