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

// OBJECTS
// -------

typedef std::function<void(char* src, size_t srclen, char* dst, size_t dstlen)> transform_callback;


/**
 *  \brief String buffer that transforms input to output data.
 */
class transform_streambuf: public std::streambuf
{
public:
    // TODO: implement??

private:
    transform_callback input_callback = nullptr;
    transform_callback output_callback = nullptr;
};


/**
 *  \brief Transform streaming I/O data via callback.
 */
class transform_iostream: public std::iostream
{
public:
    transform_iostream();
    ~transform_iostream();
    transform_iostream(const transform_iostream&) = delete;
    transform_iostream & operator=(const transform_iostream&) = delete;
//    transform_iostream(transform_iostream &&other);
//    transform_iostream & operator=(transform_iostream &&other);

    // TODO: need I/O callbacks...
    transform_iostream(std::iostream& stream);
    void open(std::iostream& stream);

    // DATA
    transform_streambuf* rdbuf() const;
    void rdbuf(transform_streambuf *buffer);
//    void swap(transform_iostream &other);

private:
    std::iostream *stream = nullptr;
    transform_streambuf buffer;
};


/**
 *  \brief Transform streaming input data via callback.
 */
class transform_istream: public std::istream
{
public:
//    istream();
//    ~istream();
//    istream(const istream&) = delete;
//    istream & operator=(const istream&) = delete;
//    istream(istream &&other);
//    istream & operator=(istream &&other);
//
//    istream(std::istream& stream);
//    void open(std::istream& stream);
//
//    // DATA
//    transform_streambuf* rdbuf() const;
//    void rdbuf(transform_streambuf *buffer);
//    bool is_open() const;
//    void close();
//    void swap(istream &other);

private:
    std::istream *stream = nullptr;
    transform_streambuf buffer;
};


/**
 *  \brief Transform streaming output data via callback.
 */
class transform_ostream: public std::ostream
{
public:
//    ostream();
//    ~ostream();
//    ostream(const ostream&) = delete;
//    ostream & operator=(const ostream&) = delete;
//    ostream(ostream &&other);
//    ostream & operator=(ostream &&other);
//
//    ostream(std::ostream& stream);
//    void open(std::ostream& stream);
//
//    // DATA
//    transform_streambuf* rdbuf() const;
//    void rdbuf(transform_streambuf *buffer);
//    bool is_open() const;
//    void close();
//    void swap(ostream &other);

private:
    std::ostream *stream = nullptr;
    transform_streambuf buffer;
};


/**
 *  \brief File like overload for iostream.
 */
class transform_fstream: public transform_iostream
{
public:
    // TODO: need constructors
    // TODO: need open

    bool is_open() const;
    void close();
//    void swap(transform_fstream &other);

private:
    fstream file;
};


/**
 *  \brief File like overload for istream.
 */
class transform_ifstream: public transform_istream
{
public:
    // TODO: need constructors
    // TODO: need open

    bool is_open() const;
    void close();
//    void swap(transform_ifstream &other);

private:
    ifstream file;
};


/**
 *  \brief File like overload for ostream.
 */
class transform_ofstream: public transform_ostream
{
public:
    // TODO: need constructors
    // TODO: need open

    bool is_open() const;
    void close();
//    void swap(transform_ofstream &other);

private:
    ofstream file;
};
