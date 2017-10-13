//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Memory-mapped file-backed stream definitions.
 *
 *  All the stream offsets must be non-negative, and
 *  therefore `size_t` is used rather than `streamoff`
 *  or `streampos`, which may represent negative values.
 */

#pragma once

#include <pycpp/stream/fd.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


/**
 *  \brief Stream wrapping a memory-mapped I/O file.
 */
class mmap_fstream: public std::iostream
{
public:
    mmap_fstream();
    ~mmap_fstream();
    mmap_fstream(const mmap_fstream&) = delete;
    mmap_fstream & operator=(const mmap_fstream&) = delete;
    mmap_fstream(mmap_fstream &&other);
    mmap_fstream & operator=(mmap_fstream &&other);

    mmap_fstream(const std::string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const std::string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    mmap_fstream(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    mmap_fstream(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
    void open(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
#endif                                          // WINDOWS

    // MAPPING
    void map(size_t offset = 0);
    void map(size_t offset, size_t length);
    void unmap();
    void flush(bool async = true);

    // PROPERTIES
    bool is_open() const;
    bool has_mapping() const;

    // MODIFIERS
    void close();
    void swap(mmap_fstream &other);

    // DATA
    char& operator[](size_t index);
    const char& operator[](size_t index) const;
    char* data() const;
    size_t length() const;

private:
    fd_streambuf buffer;
#if defined(OS_WINDOWS)
    fd_t map_fd = INVALID_FD_VALUE;
#endif
    char* data_ = nullptr;
    size_t length_ = 0;
};


/**
 *  \brief Stream wrapping a memory-mapped input file.
 */
class mmap_ifstream: public std::istream
{
public:
    mmap_ifstream();
    ~mmap_ifstream();
    mmap_ifstream(const mmap_ifstream&) = delete;
    mmap_ifstream & operator=(const mmap_ifstream&) = delete;
    mmap_ifstream(mmap_ifstream &&other);
    mmap_ifstream & operator=(mmap_ifstream &&other);

    mmap_ifstream(const std::string& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const std::string& name, std::ios_base::openmode mode = std::ios_base::in);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    mmap_ifstream(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::in);
    mmap_ifstream(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in);
    void open(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::in);
#endif                                          // WINDOWS

    // MAPPING
    void map(size_t offset = 0);
    void map(size_t offset, size_t length);
    void unmap();
    void flush(bool async = true);

    // PROPERTIES
    bool is_open() const;
    bool has_mapping() const;

    // MODIFIERS
    void close();
    void swap(mmap_ifstream &other);

    // DATA
    const char& operator[](size_t index) const;
    const char* data() const;
    size_t length() const;

private:
    fd_streambuf buffer;
#if defined(OS_WINDOWS)
    fd_t map_fd = INVALID_FD_VALUE;
#endif
    char* data_ = nullptr;
    size_t length_ = 0;
};


/**
 *  \brief Stream wrapping a memory-mapped ouput file.
 */
class mmap_ofstream: public std::ostream
{
public:
    mmap_ofstream();
    ~mmap_ofstream();
    mmap_ofstream(const mmap_ofstream&) = delete;
    mmap_ofstream & operator=(const mmap_ofstream&) = delete;
    mmap_ofstream(mmap_ofstream &&other);
    mmap_ofstream & operator=(mmap_ofstream &&other);

    mmap_ofstream(const std::string& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const std::string& name, std::ios_base::openmode mode = std::ios_base::out);

#if defined(HAVE_WFOPEN)                        // WINDOWS
    mmap_ofstream(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const std::wstring& name, std::ios_base::openmode mode = std::ios_base::out);
    mmap_ofstream(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::out);
    void open(const std::u16string& name, std::ios_base::openmode mode = std::ios_base::out);
#endif                                          // WINDOWS

    // MAPPING
    void map(size_t offset = 0);
    void map(size_t offset, size_t length);
    void unmap();
    void flush(bool async = true);

    // PROPERTIES
    bool is_open() const;
    bool has_mapping() const;

    // MODIFIERS
    void close();
    void swap(mmap_ofstream &other);

    // DATA
    char& operator[](size_t index);
    char* data() const;
    size_t length() const;

private:
    fd_streambuf buffer;
#if defined(OS_WINDOWS)
    fd_t map_fd = INVALID_FD_VALUE;
#endif
    char* data_ = nullptr;
    size_t length_ = 0;
};

PYCPP_END_NAMESPACE