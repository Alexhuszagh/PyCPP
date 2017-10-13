//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Sequential file-backed stream definitions.
 */

#pragma once

#include <pycpp/stream/fd.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Stream wrapping a sequential I/O file.
 */
class sequential_fstream: public std::iostream
{
public:
    sequential_fstream();
    ~sequential_fstream();
    sequential_fstream(const sequential_fstream&) = delete;
    sequential_fstream & operator=(const sequential_fstream&) = delete;
    sequential_fstream(sequential_fstream &&other);
    sequential_fstream & operator=(sequential_fstream &&other);

//    sequential_fstream(const char* name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//    void open(const char* name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//    sequential_fstream(const std::string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//    void open(const std::string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//
//#if defined(PYCPP_HAVE_WFOPEN)
//    sequential_fstream(const wchar_t* name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//    void open(const wchar_t* name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//    sequential_fstream(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//    void open(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//    sequential_fstream(const char16_t* name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//    void open(const char16_t* name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//    sequential_fstream(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//    void open(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
//#endif

    // DATA
    bool is_open() const;
    void close();
    void swap(sequential_fstream &other);

private:
    fd_streambuf buffer;
};

PYCPP_END_NAMESPACE
