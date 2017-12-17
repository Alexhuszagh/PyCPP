//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  Although the C standard makes no guarantees a C FILE descriptor will
 *  be used internally, both GCC and Clang do so, with GCC providing
 *  an extension to create a basic_filebuf from an existing C FILE or
 *  POSIX file descriptor[1]. As basic_fstream is merely a wrapper around
 *  basic_iostream with a basic_filebuf, this provides a full API
 *  that should have native performance.
 *
 *  By using a GNU extension to solve a GNU-specific problem, or std::fstream
 *  elsewhere, this code should work with MSVC and MinGW on Windows, and
 *  with any compiler elsewhere.
 *
 *  1. http://cs.brown.edu/~jwicks/libstdc++/html_user/stdio__filebuf_8h-source.html
 *  2. https://github.com/llvm-mirror/libcxx/blob/master/include/fstream#L132
 */

#include <pycpp/stl/detail/fstream.h>
#include <pycpp/string/codec.h>
#include <pycpp/string/unicode.h>
#include <assert.h>

PYCPP_BEGIN_NAMESPACE

#if defined(HAVE_GCC)                            // GCC

// MACROS
// ------

#define BASIC_FILEBUF(t) static_cast<std::filebuf>(t)

// FUNCTIONS
// ---------

/**
 *  \brief Convert C++ mode to C mode.
 */
std::string c_ios_mode(ios_base::openmode mode)
{
    using sios = ios_base;

    // Since "r+" and "r+b" do not create the file if it does not
    // exist,  we need to lazily check if the file exists only if
    // the mode corresponds to either mode. If the file does not exist,
    // use "w+" and "w+b", respectively.

    // static cast for clang warning, -WSwitch
    switch (static_cast<int>(mode & ~sios::ate)) {
        case sios::out:
        case sios::out | sios::trunc:
            return "w";
        case sios::out | sios::app:
        case sios::app:
            return "a";
        case sios::in:
            return "r";
        case sios::in | sios::out:
            return "r+";
        case sios::in | sios::out | sios::trunc:
            return "w+";
        case sios::in | sios::out | sios::app:
        case sios::in | sios::app:
            return "a+";
        case sios::out | sios::binary:
        case sios::out | sios::trunc | sios::binary:
            return "wb";
        case sios::out | sios::app | sios::binary:
        case sios::app | sios::binary:
            return "ab";
        case sios::in | sios::binary:
            return "rb";
        case sios::in | sios::out | sios::binary:
            return "r+b";
        case sios::in | sios::out | sios::trunc | sios::binary:
            return "w+b";
        case sios::in | sios::out | sios::app | sios::binary:
        case sios::in | sios::app | sios::binary:
            return "a+b";
        default:
            return "";
    }
}


/**
 *  \brief Get C FILE pointer from narrow filename.
 */
FILE * get_c_file(const string_view& narrow, ios_base::openmode mode)
{
    assert(is_null_terminated(narrow));

    auto str = c_ios_mode(mode);
    if (str.size()) {
        return fopen(narrow.data(), str.data());
    }
    return nullptr;
}


#if defined(HAVE_WFOPEN)                        // WINDOWS


/**
 *  \brief Get C FILE pointer from wide filename.
 */
FILE * get_c_file(const u16string_view& wide, const ios_base::openmode mode)
{
    assert(is_null_terminated(wide));

    auto str = c_ios_mode(mode);
    if (str.size()) {
        auto data = reinterpret_cast<const wchar_t*>(codec_utf8_utf16(str).data());
        return _wfopen(reinterpret_cast<const wchar_t*>(wide.data()), data);
    }
    return nullptr;
}

/**
 *  \brief Get C FILE pointer from wide filename.
 */
FILE * get_c_file(const wstring_view& wide, const ios_base::openmode mode)
{
    return get_c_file(reinterpret_cast<const char16_t*>(wide.data()), mode);
}

#endif                                          // HAVE_WFOPEN

// OBJECTS
// -------


// FSTREAM

fstream::fstream():
    iostream(&buffer)
{}


fstream::~fstream()
{
    close();
}


fstream::fstream(fstream &&other)
{
    swap(other);
}


fstream & fstream::operator=(fstream &&other)
{
    swap(other);
    return *this;
}


fstream::fstream(const string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void fstream::open(const string_view& name, ios_base::openmode mode)
{
    // Windows, Unicode API
#if defined(HAVE_WFOPEN)
    if (is_unicode(name)) {
        open(codec_utf8_utf16(name), mode);
        return;
    }
#endif      // HAVE_WFOPEN

    file = get_c_file(name, mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    ios::rdbuf(&buffer);
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

fstream::fstream(const wstring_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void fstream::open(const wstring_view& name, ios_base::openmode mode)
{
    file = get_c_file(name, mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    ios::rdbuf(&buffer);
}


fstream::fstream(const u16string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void fstream::open(const u16string_view& name, ios_base::openmode mode)
{
    file = get_c_file(name, mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    ios::rdbuf(&buffer);
}

#endif                                          // HAVE_WFOPEN


std::filebuf* fstream::rdbuf() const
{
    return const_cast<std::filebuf*>(&buffer);
}


void fstream::rdbuf(std::filebuf *buffer)
{
    ios::rdbuf(buffer);
}


bool fstream::is_open() const
{
    return buffer.is_open();
}


void fstream::close()
{
    if (file) {
        buffer.close();
        fclose(file);
        file = nullptr;
    }
}


void fstream::swap(fstream &other)
{
    // swap
    iostream::swap(other);
    PYCPP_NAMESPACE::swap(file, other.file);
    PYCPP_NAMESPACE::swap(buffer, other.buffer);

    // set filebuffers
    ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}


// IFSTREAM

ifstream::ifstream():
    istream(&buffer)
{}


ifstream::~ifstream()
{
    close();
}


ifstream::ifstream(ifstream &&other)
{
    swap(other);
}


ifstream & ifstream::operator=(ifstream &&other)
{
    swap(other);
    return *this;
}


ifstream::ifstream(const string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void ifstream::open(const string_view& name, ios_base::openmode mode)
{
    // Windows, Unicode API
#if defined(HAVE_WFOPEN)
    if (is_unicode(name)) {
        open(codec_utf8_utf16(name), mode);
        return;
    }
#endif      // HAVE_WFOPEN

    file = get_c_file(name, mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    ios::rdbuf(&buffer);
}

#if defined(HAVE_WFOPEN)                        // WINDOWS


ifstream::ifstream(const wstring_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void ifstream::open(const wstring_view& name, ios_base::openmode mode)
{
    file = get_c_file(name, mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    ios::rdbuf(&buffer);
}


ifstream::ifstream(const u16string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void ifstream::open(const u16string_view& name, ios_base::openmode mode)
{
    file = get_c_file(name, mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    ios::rdbuf(&buffer);
}

#endif                                          // HAVE_WFOPEN


std::filebuf* ifstream::rdbuf() const
{
    return const_cast<std::filebuf*>(&buffer);
}


void ifstream::rdbuf(std::filebuf *buffer)
{
    ios::rdbuf(buffer);
}


bool ifstream::is_open() const
{
    return buffer.is_open();
}


void ifstream::close()
{
    if (file) {
        buffer.close();
        fclose(file);
        file = nullptr;
    }
}


void ifstream::swap(ifstream &other)
{
    // swap
    istream::swap(other);
    PYCPP_NAMESPACE::swap(file, other.file);
    PYCPP_NAMESPACE::swap(buffer, other.buffer);

    // set filebuffers
    ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}


ofstream::ofstream():
    ostream(&buffer)
{}


ofstream::~ofstream()
{
    close();
}


ofstream::ofstream(ofstream &&other)
{
    swap(other);
}


ofstream & ofstream::operator=(ofstream &&other)
{
    swap(other);
    return *this;
}


ofstream::ofstream(const string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}

void ofstream::open(const string_view& name, ios_base::openmode mode)
{
    // Windows, Unicode API
#if defined(HAVE_WFOPEN)
    if (is_unicode(name)) {
        open(codec_utf8_utf16(name), mode);
        return;
    }
#endif      // HAVE_WFOPEN

    file = get_c_file(name, mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    ios::rdbuf(&buffer);
}


#if defined(HAVE_WFOPEN)                        // WINDOWS

ofstream::ofstream(const wstring_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void ofstream::open(const wstring_view& name, ios_base::openmode mode)
{
    file = get_c_file(name, mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    ios::rdbuf(&buffer);
}


ofstream::ofstream(const u16string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void ofstream::open(const u16string_view& name, ios_base::openmode mode)
{
    file = get_c_file(name, mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    ios::rdbuf(&buffer);
}

#endif                                          // HAVE_WFOPEN


std::filebuf* ofstream::rdbuf() const
{
    return const_cast<std::filebuf*>(&buffer);
}


void ofstream::rdbuf(std::filebuf *buffer)
{
    ios::rdbuf(buffer);
}


bool ofstream::is_open() const
{
    return buffer.is_open();
}


void ofstream::close()
{
    if (file) {
        buffer.close();
        fclose(file);
        file = nullptr;
    }
}


void ofstream::swap(ofstream &other)
{
    // swap
    ostream::swap(other);
    PYCPP_NAMESPACE::swap(file, other.file);
    PYCPP_NAMESPACE::swap(buffer, other.buffer);

    // set filebuffers
    ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

#else                                   // NON-GCC/MSVC COMPILER

// FSTREAM

fstream::fstream()
{}


fstream::~fstream()
{}


fstream::fstream(fstream &&other)
{
    std::fstream::swap(other);
}


fstream & fstream::operator=(fstream &&other)
{
    std::fstream::swap(other);
    return *this;
}


fstream::fstream(const string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void fstream::open(const string_view& name, ios_base::openmode mode)
{
    // Windows, Unicode API
#if defined(HAVE_WFOPEN)
    if (is_unicode(name)) {
        open(codec_utf8_utf16(name), mode);
        return;
    }
#endif      // HAVE_WFOPEN

    assert(is_null_terminated(name));
    std::fstream::open(name.data(), mode);
}

#if defined(HAVE_WFOPEN)                        // WINDOWS


fstream::fstream(const wstring_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void fstream::open(const wstring_view& name, ios_base::openmode mode)
{
    assert(is_null_terminated(name));
    std::fstream::open(name.data(), mode);
}


fstream::fstream(const u16string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void fstream::open(const u16string_view& name, ios_base::openmode mode)
{
    assert(is_null_terminated(name));
    std::fstream::open(reinterpret_cast<const wchar_t*>(name.data()), mode);
}

#endif                                          // HAVE_WFOPEN

// IFSTREAM

ifstream::ifstream()
{}


ifstream::~ifstream()
{}


ifstream::ifstream(ifstream &&other)
{
    std::ifstream::swap(other);
}


ifstream & ifstream::operator=(ifstream &&other)
{
    std::ifstream::swap(other);
    return *this;
}


ifstream::ifstream(const string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void ifstream::open(const string_view& name, ios_base::openmode mode)
{
    // Windows, Unicode API
#if defined(HAVE_WFOPEN)
    if (is_unicode(name)) {
        open(codec_utf8_utf16(name), mode);
        return;
    }
#endif      // HAVE_WFOPEN

    assert(is_null_terminated(name));
    std::ifstream::open(name.data(), mode);
}

#if defined(HAVE_WFOPEN)                        // WINDOWS


ifstream::ifstream(const wstring_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void ifstream::open(const wstring_view& name, ios_base::openmode mode)
{
    assert(is_null_terminated(name));
    std::ifstream::open(name.data(), mode);
}


ifstream::ifstream(const u16string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void ifstream::open(const u16string_view& name, ios_base::openmode mode)
{
    assert(is_null_terminated(name));
    std::ifstream::open(reinterpret_cast<const wchar_t*>(name.data()), mode);
}

#endif                                          // HAVE_WFOPEN

// OFSTREAM


ofstream::ofstream()
{}


ofstream::~ofstream()
{}


ofstream::ofstream(ofstream &&other)
{
    std::ofstream::swap(other);
}


ofstream & ofstream::operator=(ofstream &&other)
{
    std::ofstream::swap(other);
    return *this;
}


ofstream::ofstream(const string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void ofstream::open(const string_view& name, ios_base::openmode mode)
{
    // Windows, Unicode API
#if defined(HAVE_WFOPEN)
    if (is_unicode(name)) {
        open(codec_utf8_utf16(name), mode);
        return;
    }
#endif      // HAVE_WFOPEN

    assert(is_null_terminated(name));
    std::ofstream::open(name.data(), mode);
}

#if defined(HAVE_WFOPEN)                        // WINDOWS


ofstream::ofstream(const wstring_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void ofstream::open(const wstring_view& name, ios_base::openmode mode)
{
    assert(is_null_terminated(name));
    std::ofstream::open(name.data(), mode);
}


ofstream::ofstream(const u16string_view& name, ios_base::openmode mode)
{
    open(name, mode);
}


void ofstream::open(const u16string_view& name, ios_base::openmode mode)
{
    assert(is_null_terminated(name));
    std::ofstream::open(reinterpret_cast<const wchar_t*>(name.data()), mode);
}

#endif                                          // HAVE_WFOPEN
#endif                                          // GCC

PYCPP_END_NAMESPACE
