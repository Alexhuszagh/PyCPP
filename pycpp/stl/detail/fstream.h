//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Wide character path overloads for narrow fstreams.
 *
 *  Uses GNU extensions to provide wide-character overloads for narrow
 *  streams, so narrow streams (likely) encoding UTF-8 data may be
 *  opened using the Windows wide (Unicode) API.
 *
 *  \synopsis
 *      class fstream: public std::iostream
 *      {
 *      public:
 *          fstream();
 *          ~fstream();
 *          fstream(const fstream&) = delete;
 *          fstream& operator=(const fstream&) = delete;
 *          fstream(fstream&&);
 *          fstream& operator=(fstream&&);
 *
 *          fstream(const string_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *          void open(const string_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *
 *      #if defined(OS_WINDOWS)
 *          fstream(const wstring_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *          void open(const wstring_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *          fstream(const u16string_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *          void open(const u16string_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *      #endif
 *      };
 *
 *      class ifstream: public std::istream
 *      {
 *      public:
 *          ifstream();
 *          ~ifstream();
 *          ifstream(const ifstream&) = delete;
 *          ifstream& operator=(const ifstream&) = delete;
 *          ifstream(ifstream&&);
 *          ifstream& operator=(ifstream&&);
 *
 *          ifstream(const string_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *          void open(const string_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *
 *      #if defined(OS_WINDOWS)
 *          ifstream(const wstring_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *          void open(const wstring_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *          ifstream(const u16string_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *          void open(const u16string_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *      #endif
 *      };
 *
 *      class ofstream: public std::ostream
 *      {
 *      public:
 *          ofstream();
 *          ~ofstream();
 *          ofstream(const ofstream&) = delete;
 *          ofstream& operator=(const ofstream&) = delete;
 *          ofstream(ofstream&& rhs);
 *          ofstream& operator=(ofstream&& rhs);
 *
 *          ofstream(const string_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *          void open(const string_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *
 *      #if defined(OS_WINDOWS)
 *          ofstream(const wstring_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *          void open(const wstring_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *          ofstream(const u16string_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *          void open(const u16string_view& name,
 *              ios_base::openmode mode = implementation-defined);
 *      #endif
 *      };
 */

#pragma once

#include <pycpp/preprocessor/compiler.h>
#include <pycpp/stl/iostream.h>
#include <pycpp/stl/string_view.h>
#include <fstream>

#if defined(HAVE_GCC)
#   include <pycpp/preprocessor/os.h>
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
class fstream: public iostream
{
public:
    fstream();
    ~fstream();
    fstream(const fstream&) = delete;
    fstream& operator=(const fstream&) = delete;
    fstream(fstream&&);
    fstream& operator=(fstream&&);

    fstream(const string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary);
    void open(const string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    fstream(const wstring_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary);
    void open(const wstring_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary);
    fstream(const u16string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary);
    void open(const u16string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary);
#endif                                          // WINDOWS

    // DATA
    bool is_open() const;
    void close();
    void swap(fstream &other);
    std::filebuf* rdbuf() const;
    void rdbuf(std::filebuf *buffer);

private:
    FILE *file = nullptr;
    mutable std::filebuf buffer;
};


/**
 *  \brief Stream supporting both UTF-8 and UTF-16 paths on Windows.
 *
 *  ifstream based off a GNU stdio_filebuf. char strings assume
 *  UTF-8 encoding, while wchar_t/char16_t strings assume
 *  the native UTF-16 encoding.
 */
class ifstream: public istream
{
public:
    ifstream();
    ~ifstream();
    ifstream(const ifstream&) = delete;
    ifstream& operator=(const ifstream&) = delete;
    ifstream(ifstream&&);
    ifstream& operator=(ifstream&&);

    ifstream(const string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::binary);
    void open(const string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::binary);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    ifstream(const wstring_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::binary);
    void open(const wstring_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::binary);
    ifstream(const u16string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::binary);
    void open(const u16string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::binary);
#endif                                          // WINDOWS

    bool is_open() const;
    void close();
    void swap(ifstream &other);
    std::filebuf* rdbuf() const;
    void rdbuf(std::filebuf *buffer);

private:
    FILE *file = nullptr;
    mutable std::filebuf buffer;
};


/**
 *  \brief Stream supporting both UTF-8 and UTF-16 paths on Windows.
 *
 *  ofstream based off a GNU stdio_filebuf. char strings assume
 *  UTF-8 encoding, while wchar_t/char16_t strings assume
 *  the native UTF-16 encoding.
 */
class ofstream: public ostream
{
public:
    ofstream();
    ~ofstream();
    ofstream(const ofstream&) = delete;
    ofstream& operator=(const ofstream&) = delete;
    ofstream(ofstream&&);
    ofstream& operator=(ofstream&&);

    ofstream(const string_view& name,
        ios_base::openmode mode = ios_base::out | ios_base::binary);
    void open(const string_view& name,
        ios_base::openmode mode = ios_base::out | ios_base::binary);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    ofstream(const wstring_view& name,
        ios_base::openmode mode = ios_base::out | ios_base::binary);
    void open(const wstring_view& name,
        ios_base::openmode mode = ios_base::out | ios_base::binary);
    ofstream(const u16string_view& name,
        ios_base::openmode mode = ios_base::out | ios_base::binary);
    void open(const u16string_view& name,
        ios_base::openmode mode = ios_base::out | ios_base::binary);
#endif                                          // WINDOWS

    bool is_open() const;
    void close();
    void swap(ofstream &other);
    std::filebuf* rdbuf() const;
    void rdbuf(std::filebuf *buffer);

private:
    FILE *file = nullptr;
    mutable std::filebuf buffer;
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
    fstream& operator=(const fstream&) = delete;
    fstream(fstream&&);
    fstream& operator=(fstream&&);

    fstream(const string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary);
    void open(const string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    fstream(const wstring_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary);
    void open(const wstring_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary);
    fstream(const u16string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary);
    void open(const u16string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::binary);
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
    ifstream& operator=(const ifstream&) = delete;
    ifstream(ifstream&&);
    ifstream& operator=(ifstream&&);

    ifstream(const string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::binary);
    void open(const string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::binary);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    ifstream(const wstring_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::binary);
    void open(const wstring_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::binary);
    ifstream(const u16string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::binary);
    void open(const u16string_view& name,
        ios_base::openmode mode = ios_base::in | ios_base::binary);
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
    ofstream& operator=(const ofstream&) = delete;
    ofstream(ofstream&&);
    ofstream& operator=(ofstream&&);

    ofstream(const string_view& name,
        ios_base::openmode mode = ios_base::out | ios_base::binary);
    void open(const string_view& name,
        ios_base::openmode mode = ios_base::out | ios_base::binary);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    ofstream(const wstring_view& name,
        ios_base::openmode mode = ios_base::out | ios_base::binary);
    void open(const wstring_view& name,
        ios_base::openmode mode = ios_base::out | ios_base::binary);
    ofstream(const u16string_view& name,
        ios_base::openmode mode = ios_base::out | ios_base::binary);
    void open(const u16string_view& name,
        ios_base::openmode mode = ios_base::out | ios_base::binary);
#endif                                          // WINDOWS
};

#endif

PYCPP_END_NAMESPACE
