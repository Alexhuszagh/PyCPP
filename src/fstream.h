//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Wide character path overloads for narrow fstreams.
 */

#pragma once

#include "compiler.h"
#include <fstream>
#if defined(HAVE_GCC)
#   include "os.h"
#   include <ext/stdio_filebuf.h>
#   include <iostream>
#endif


// OBJECTS
// -------


#if defined(HAVE_GCC)           // GCC

/** \brief Stream supporting narrow and wide APIs on Windows.
 *
 *  fstream based off a GNU stdio_filebuf.
 */
class fstream: public std::iostream
{
public:
    fstream();
    ~fstream();
    fstream(const fstream&) = delete;
    fstream & operator=(const fstream&) = delete;
    fstream(fstream &&other);
    fstream & operator=(fstream &&other);

    fstream(const std::string &name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const std::string &name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

#if defined(OS_WINDOWS)
    fstream(const std::wstring &name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const std::wstring &name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
#endif

    // DATA
    std::basic_filebuf<char>* rdbuf() const;
    void rdbuf(std::basic_filebuf<char> *buffer);
    bool is_open() const;
    void close();
    void swap(fstream &other);

private:
    FILE *file = nullptr;
    std::basic_filebuf<char> buffer;

    typedef char                          char_type;
    typedef std::char_traits<char>        traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;
};


/** \brief Stream supporting narrow and wide APIs on Windows.
 *
 *  ifstream based off a GNU stdio_filebuf.
 */
class ifstream: public std::istream
{
public:
    ifstream();
    ~ifstream();
    ifstream(const ifstream&) = delete;
    ifstream & operator=(const ifstream&) = delete;
    ifstream(ifstream &&other);
    ifstream & operator=(ifstream &&other);

    ifstream(const std::string &name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const std::string &name, std::ios_base::openmode mode = std::ios_base::in);

#if defined(OS_WINDOWS)
    ifstream(const std::wstring &name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const std::wstring &name, std::ios_base::openmode mode = std::ios_base::in);
#endif

    std::basic_filebuf<char>* rdbuf() const;
    void rdbuf(std::basic_filebuf<char> *buffer);
    bool is_open() const;
    void close();
    void swap(ifstream &other);

private:
    FILE *file = nullptr;
    std::basic_filebuf<char> buffer;

    typedef char                          char_type;
    typedef std::char_traits<char>        traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;
};


/** \brief Stream supporting narrow and wide APIs on Windows.
 *
 *  ofstream based off a GNU stdio_filebuf.
 */
class ofstream: public std::ostream
{
public:
    ofstream();
    ~ofstream();
    ofstream(const ofstream&) = delete;
    ofstream & operator=(const ofstream&) = delete;
    ofstream(ofstream &&other);
    ofstream & operator=(ofstream &&other);

    ofstream(const std::string &name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const std::string &name, std::ios_base::openmode mode = std::ios_base::out);

#if defined(OS_WINDOWS)
    ofstream(const std::wstring &name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const std::wstring &name, std::ios_base::openmode mode = std::ios_base::out);
#endif

    std::basic_filebuf<char>* rdbuf() const;
    void rdbuf(std::basic_filebuf<char> *buffer);
    bool is_open() const;
    void close();
    void swap(ofstream &other);

private:
    FILE *file = nullptr;
    std::basic_filebuf<char> buffer;

    typedef char                          char_type;
    typedef std::char_traits<char>        traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;
};

#else                   // NON-GCC COMPILER

typedef std::fstream fstream;
typedef std::ifstream ifstream;
typedef std::ofstream ofstream;

#endif
