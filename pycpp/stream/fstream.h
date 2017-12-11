//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wide character path overloads for narrow fstreams.
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/fstream.h>
#include <pycpp/stl/string_view.h>

#if defined(HAVE_GCC)
#   include <pycpp/preprocessor/os.h>
#   include <pycpp/stl/iostream.h>
#   include <ext/stdio_filebuf.h>
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

    fstream(const string_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const string_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    fstream(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    fstream(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
#endif                                          // WINDOWS

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

    ifstream(const string_view& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const string_view& name, std::ios_base::openmode mode = std::ios_base::in);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    ifstream(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::in);
    ifstream(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::in);
#endif                                          // WINDOWS

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

    ofstream(const string_view& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const string_view& name, std::ios_base::openmode mode = std::ios_base::out);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    ofstream(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::out);
    ofstream(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::out);
#endif                                          // WINDOWS

    bool is_open() const;
    void close();
    void swap(ofstream &other);
    std::basic_filebuf<char>* rdbuf() const;
    void rdbuf(std::basic_filebuf<char> *buffer);

private:
    FILE *file = nullptr;
    std::basic_filebuf<char> buffer;
};

#else                           // NON-GCC/MSVC COMPILER

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

    fstream(const string_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const string_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    fstream(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    fstream(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
#endif                                          // WINDOWS
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

    ifstream(const string_view& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const string_view& name, std::ios_base::openmode mode = std::ios_base::in);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    ifstream(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::in);
    ifstream(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::in);
#endif                                          // WINDOWS
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

    ofstream(const string_view& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const string_view& name, std::ios_base::openmode mode = std::ios_base::out);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    ofstream(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const wstring_view& name, std::ios_base::openmode mode = std::ios_base::out);
    ofstream(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const u16string_view& name, std::ios_base::openmode mode = std::ios_base::out);
#endif                                          // WINDOWS
};

#endif

PYCPP_END_NAMESPACE
