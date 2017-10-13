//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wide character path overloads for narrow fstreams.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>
#include <fstream>

#if defined(HAVE_GCC)
#   include <pycpp/preprocessor/os.h>
#   include <ext/stdio_filebuf.h>
#   include <stdio.h>
#   include <iostream>
#   if defined(_wfopen)
#       define PYCPP_HAVE_WFOPEN 1
#   endif
#elif defined(HAVE_MSVC)
#   define PYCPP_HAVE_WFOPEN 1
#endif

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


#if defined(HAVE_GCC)           // GCC

/**
 *  \brief Stream supporting both UTF-8 and UTF-16 paths on Windows.
 *
 *  fstream based off a GNU stdio_filebuf. char strings assume
 *  UTF-8 encoding, while wchar_t/char16_t strings assume
 *  the native UTF-16 encoding.
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

    fstream(const std::string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const std::string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

#if defined(PYCPP_HAVE_WFOPEN)
    fstream(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    fstream(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
#endif

    // DATA
    bool is_open() const;
    void close();
    void swap(fstream &other);
    std::basic_filebuf<char>* rdbuf() const;
    void rdbuf(std::basic_filebuf<char> *buffer);

private:
    FILE *file = nullptr;
    std::basic_filebuf<char> buffer;
};


/**
 *  \brief Stream supporting both UTF-8 and UTF-16 paths on Windows.
 *
 *  ifstream based off a GNU stdio_filebuf. char strings assume
 *  UTF-8 encoding, while wchar_t/char16_t strings assume
 *  the native UTF-16 encoding.
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

    ifstream(const std::string& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const std::string& name, std::ios_base::openmode mode = std::ios_base::in);

#if defined(PYCPP_HAVE_WFOPEN)
    ifstream(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in);
    ifstream(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in);
#endif

    bool is_open() const;
    void close();
    void swap(ifstream &other);
    std::basic_filebuf<char>* rdbuf() const;
    void rdbuf(std::basic_filebuf<char> *buffer);

private:
    FILE *file = nullptr;
    std::basic_filebuf<char> buffer;
};


/**
 *  \brief Stream supporting both UTF-8 and UTF-16 paths on Windows.
 *
 *  ofstream based off a GNU stdio_filebuf. char strings assume
 *  UTF-8 encoding, while wchar_t/char16_t strings assume
 *  the native UTF-16 encoding.
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

    ofstream(const std::string& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const std::string& name, std::ios_base::openmode mode = std::ios_base::out);

#if defined(PYCPP_HAVE_WFOPEN)
    ofstream(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::out);
    ofstream(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::out);
#endif

    bool is_open() const;
    void close();
    void swap(ofstream &other);
    std::basic_filebuf<char>* rdbuf() const;
    void rdbuf(std::basic_filebuf<char> *buffer);

private:
    FILE *file = nullptr;
    std::basic_filebuf<char> buffer;
};

#elif defined(HAVE_MSVC)        // MSVC

/**
 *  \brief Stream supporting both UTF-8 and UTF-16 paths on Windows.
 *
 *  char strings assume UTF-8 encoding, while wchar_t/char16_t strings
 *  assume the native UTF-16 encoding.
 */
class fstream: public std::fstream
{
public:
    fstream();
    ~fstream();
    fstream(const fstream&) = delete;
    fstream & operator=(const fstream&) = delete;
    fstream(fstream &&other);
    fstream & operator=(fstream &&other);

    fstream(const std::string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const std::string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

#if defined(PYCPP_HAVE_WFOPEN)
    fstream(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    fstream(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
#endif
};


/**
 *  \brief Stream supporting both UTF-8 and UTF-16 paths on Windows.
 *
 *  char strings assume UTF-8 encoding, while wchar_t/char16_t strings
 *  assume the native UTF-16 encoding.
 */
class ifstream: public std::ifstream
{
public:
    ifstream();
    ~ifstream();
    ifstream(const ifstream&) = delete;
    ifstream & operator=(const ifstream&) = delete;
    ifstream(ifstream &&other);
    ifstream & operator=(ifstream &&other);

    ifstream(const std::string& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const std::string& name, std::ios_base::openmode mode = std::ios_base::in);

#if defined(PYCPP_HAVE_WFOPEN)
    ifstream(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in);
    ifstream(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in);
#endif
};


/**
 *  \brief Stream supporting both UTF-8 and UTF-16 paths on Windows.
 *
 *  char strings assume UTF-8 encoding, while wchar_t/char16_t strings
 *  assume the native UTF-16 encoding.
 */
class ofstream: public std::ofstream
{
public:
    ofstream();
    ~ofstream();
    ofstream(const ofstream&) = delete;
    ofstream & operator=(const ofstream&) = delete;
    ofstream(ofstream &&other);
    ofstream & operator=(ofstream &&other);

    ofstream(const std::string& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const std::string& name, std::ios_base::openmode mode = std::ios_base::out);

#if defined(PYCPP_HAVE_WFOPEN)
    ofstream(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::out);
    ofstream(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::out);
#endif
};

#else                           // NON-GCC/MSVC COMPILER

typedef std::fstream fstream;
typedef std::ifstream ifstream;
typedef std::ofstream ofstream;

#endif

PYCPP_END_NAMESPACE
