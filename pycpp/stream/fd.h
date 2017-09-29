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

//    // MEMBER FUNCTIONS
//    // ----------------
//    fd_streambuf(std::ios_base::openmode, std::streambuf* = nullptr, filter_callback = nullptr);
//    virtual ~fd_streambuf();
//
//    fd_streambuf(const fd_streambuf&) = delete;
//    fd_streambuf& operator=(const fd_streambuf&) = delete;
//    fd_streambuf(fd_streambuf&&);
//    fd_streambuf& operator=(fd_streambuf&&);
//    void close();
//    void swap(fd_streambuf&);
//
//    void set_filebuf(std::streambuf*);
//    void set_callback(filter_callback);

protected:
    // MEMBER FUNCTIONS
    // ----------------
    virtual int_type underflow();
    virtual int_type overflow(int_type = traits_type::eof());
    virtual int sync();

private:
//    friend class fd_istream;
//    friend class fd_ostream;

    std::ios_base::openmode mode;
    fd_t fd_;
    char_type* in_buffer = nullptr;
    char_type* out_buffer = nullptr;
    char_type* in_first = nullptr;
    char_type* in_last = nullptr;
};


/**
 *  \brief Stream wrapper using file descriptors.
 */
struct fd_fstream: std::iostream
{
public:
//    fd_fstream();
//    ~fd_fstream();
    fd_fstream(const fd_fstream&) = delete;
    fd_fstream & operator=(const fd_fstream&) = delete;
//    fd_fstream(fd_fstream &&other);
//    fd_fstream & operator=(fd_fstream &&other);
    // TODO: more constructors...

    // TODO: here...
    // bool is_open() const;
    // void close();
    // void swap(fd_fstream &other);
    // std::basic_filebuf<char>* rdbuf() const;
    // void rdbuf(std::basic_filebuf<char> *buffer);

private:
    // TODO: doesn't this need two buffers??
//    fd_streambuf buffer;
    bool close;
};


/**
 *  \brief Input stream wrapper using file descriptors.
 */
struct fd_ifstream: std::iostream
{
public:
//    fd_ifstream();
//    ~fd_ifstream();
    fd_ifstream(const fd_ifstream&) = delete;
    fd_ifstream & operator=(const fd_ifstream&) = delete;
//    fd_ifstream(fd_ifstream &&other);
//    fd_ifstream & operator=(fd_ifstream &&other);
    // TODO: more constructors...

    // TODO: here...
    // bool is_open() const;
    // void close();
    // void swap(fd_ifstream &other);
    // std::basic_filebuf<char>* rdbuf() const;
    // void rdbuf(std::basic_filebuf<char> *buffer);

private:
    fd_streambuf buffer;
    bool close;
};


/**
 *  \brief Output stream wrapper using file descriptors.
 */
struct fd_ofstream: std::iostream
{
public:
//    fd_ofstream();
//    ~fd_ofstream();
    fd_ofstream(const fd_ofstream&) = delete;
    fd_ofstream & operator=(const fd_ofstream&) = delete;
//    fd_ofstream(fd_ofstream &&other);
//    fd_ofstream & operator=(fd_ofstream &&other);
    // TODO: more constructors...

    // TODO: here...
    // bool is_open() const;
    // void close();
    // void swap(fd_ofstream &other);
    // std::basic_filebuf<char>* rdbuf() const;
    // void rdbuf(std::basic_filebuf<char> *buffer);

private:
    fd_streambuf buffer;
    bool close;
};

PYCPP_END_NAMESPACE
