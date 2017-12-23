//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief File descriptor stream definitions.
 */

#pragma once

#include <pycpp/filesystem/fd.h>
#include <pycpp/stl/iostream.h>

PYCPP_BEGIN_NAMESPACE

// VARIABLES
// ---------

extern size_t DEFAULT_BUFFER_SIZE;

// OBJECTS
// -------

/**
 *  \brief Filtering streambuffer that wraps a file descriptor.
 */
class fd_streambuf: public streambuf
{
public:
    // MEMBER TYPES
    // ------------
    using typename streambuf::char_type;
    using typename streambuf::int_type;
    using typename streambuf::traits_type;
    using typename streambuf::off_type;
    using typename streambuf::pos_type;

    // MEMBER FUNCTIONS
    // ----------------
    fd_streambuf(ios_base::openmode, fd_t fd);
    fd_streambuf(ios_base::openmode, fd_t fd, size_t buffer_size);
    fd_streambuf(const fd_streambuf&) = delete;
    fd_streambuf& operator=(const fd_streambuf&) = delete;
    fd_streambuf(fd_streambuf&&);
    fd_streambuf& operator=(fd_streambuf&&);
    virtual ~fd_streambuf();

    // MODIFIERS/PROPERTIES
    void close();
    bool is_open() const;
    void swap(fd_streambuf&);
    fd_t fd() const;
    void fd(fd_t fd);

protected:
    // MEMBER FUNCTIONS
    // ----------------
    virtual int_type underflow();
    virtual int_type overflow(int_type = traits_type::eof());
    virtual int sync();
    virtual pos_type seekoff(off_type off, ios_base::seekdir way, ios_base::openmode openmode = ios_base::in | ios_base::out);
    virtual pos_type seekpos(pos_type pos, ios_base::openmode openmode = ios_base::in | ios_base::out);

private:
    void initialize_buffers();
    void set_readp();
    void set_writep();

    ios_base::openmode mode;
    size_t buffer_size;
    fd_t fd_ = INVALID_FD_VALUE;
    char_type* in_first = nullptr;
    char_type* in_last = nullptr;
    char_type* out_first = nullptr;
    char_type* out_last = nullptr;
};


/**
 *  \brief Stream wrapper using file descriptors.
 */
class fd_stream: public iostream
{
public:
    fd_stream();
    ~fd_stream();
    fd_stream(const fd_stream&) = delete;
    fd_stream& operator=(const fd_stream&) = delete;
    fd_stream(fd_stream&&);
    fd_stream& operator=(fd_stream&&);

    // STREAM
    fd_stream(fd_t fd, bool close = false);
    void open(fd_t fd, bool close = false);

    // MODIFIERS/PROPERTIES
    void close();
    bool is_open() const;
    void swap(fd_stream&);
    streambuf* rdbuf() const;
    void rdbuf(streambuf* buffer);

private:
    fd_streambuf buffer;
    bool close_ = false;
};


/**
 *  \brief Input stream wrapper using file descriptors.
 */
class fd_istream: public istream
{
public:
    fd_istream();
    ~fd_istream();
    fd_istream(const fd_istream&) = delete;
    fd_istream& operator=(const fd_istream&) = delete;
    fd_istream(fd_istream&&);
    fd_istream& operator=(fd_istream&&);

    // STREAM
    fd_istream(fd_t fd, bool close = false);
    void open(fd_t fd, bool close = false);

    // MODIFIERS/PROPERTIES
    void close();
    bool is_open() const;
    void swap(fd_istream&);
    streambuf* rdbuf() const;
    void rdbuf(streambuf* buffer);

private:
    fd_streambuf buffer;
    bool close_ = false;
};


/**
 *  \brief Output stream wrapper using file descriptors.
 */
class fd_ostream: public ostream
{
public:
    fd_ostream();
    ~fd_ostream();
    fd_ostream(const fd_ostream&) = delete;
    fd_ostream & operator=(const fd_ostream&) = delete;
    fd_ostream(fd_ostream&&);
    fd_ostream & operator=(fd_ostream&&);

    // STREAM
    fd_ostream(fd_t fd, bool close = false);
    void open(fd_t fd, bool close = false);

    // MODIFIERS/PROPERTIES
    void close();
    bool is_open() const;
    void swap(fd_ostream&);
    streambuf* rdbuf() const;
    void rdbuf(streambuf* buffer);

private:
    fd_streambuf buffer;
    bool close_ = false;
};

PYCPP_END_NAMESPACE
