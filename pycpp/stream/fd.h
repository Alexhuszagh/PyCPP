//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief File descriptor stream definitions.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/filesystem/fd.h>
#include <iostream>

// OBJECTS
// -------


/**
 *  \brief Filtering streambuffer that wraps a file descriptor.
 */
class fd_streambuf: public std::streambuf
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
    fd_streambuf(std::ios_base::openmode, fd_t fd);
    virtual ~fd_streambuf();

    fd_streambuf(const fd_streambuf&) = delete;
    fd_streambuf& operator=(const fd_streambuf&) = delete;
    fd_streambuf(fd_streambuf&&);
    fd_streambuf& operator=(fd_streambuf&&);
    void close();
    void swap(fd_streambuf&);

    void set_fd(fd_t fd);

protected:
    // MEMBER FUNCTIONS
    // ----------------
    virtual int_type underflow();
    virtual int_type overflow(int_type = traits_type::eof());
    virtual int sync();
    virtual pos_type seekoff(off_type off, std::ios_base::seekdir way, std::ios_base::openmode openmode = std::ios_base::in | std::ios_base::out);
    virtual pos_type seekpos(pos_type pos, std::ios_base::openmode openmode = std::ios_base::in | std::ios_base::out);

private:
    void set_readp();
    void set_writep();

    friend class fd_stream;
    friend class fd_istream;
    friend class fd_ostream;

    std::ios_base::openmode mode;
    fd_t fd = INVALID_FD_VALUE;
    char_type* in_first = nullptr;
    char_type* in_last = nullptr;
    char_type* out_first = nullptr;
    char_type* out_last = nullptr;
};


/**
 *  \brief Stream wrapper using file descriptors.
 */
struct fd_stream: std::iostream
{
public:
    fd_stream();
    ~fd_stream();
    fd_stream(const fd_stream&) = delete;
    fd_stream & operator=(const fd_stream&) = delete;
    fd_stream(fd_stream &&other);
    fd_stream & operator=(fd_stream &&other);

    fd_stream(fd_t fd, bool close = false);
    void open(fd_t fd, bool close = false);
    void close();
    bool is_open() const;

    void swap(fd_stream &other);
    fd_streambuf* rdbuf() const;
    void rdbuf(fd_streambuf* buffer);

private:
    fd_streambuf buffer;
    bool close_ = false;
};


/**
 *  \brief Input stream wrapper using file descriptors.
 */
struct fd_istream: std::istream
{
    fd_istream();
    ~fd_istream();
    fd_istream(const fd_istream&) = delete;
    fd_istream & operator=(const fd_istream&) = delete;
    fd_istream(fd_istream &&other);
    fd_istream & operator=(fd_istream &&other);

    fd_istream(fd_t fd, bool close = false);
    void open(fd_t fd, bool close = false);
    void close();
    bool is_open() const;

    void swap(fd_istream &other);
    fd_streambuf* rdbuf() const;
    void rdbuf(fd_streambuf* buffer);

private:
    fd_streambuf buffer;
    bool close_ = false;
};


/**
 *  \brief Output stream wrapper using file descriptors.
 */
struct fd_ostream: std::ostream
{
public:
    fd_ostream();
    ~fd_ostream();
    fd_ostream(const fd_ostream&) = delete;
    fd_ostream & operator=(const fd_ostream&) = delete;
    fd_ostream(fd_ostream &&other);
    fd_ostream & operator=(fd_ostream &&other);

    fd_ostream(fd_t fd, bool close = false);
    void open(fd_t fd, bool close = false);
    void close();
    bool is_open() const;

    void swap(fd_ostream &other);
    fd_streambuf* rdbuf() const;
    void rdbuf(fd_streambuf* buffer);

private:
    fd_streambuf buffer;
    bool close_ = false;
};

PYCPP_END_NAMESPACE
