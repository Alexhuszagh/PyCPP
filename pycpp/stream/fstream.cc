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

#include <pycpp/stream/fstream.h>
#include <pycpp/string/codec.h>

PYCPP_BEGIN_NAMESPACE

#if defined(HAVE_GCC)                   // GCC

// MACROS
// ------

#define BASIC_FILEBUF(t) static_cast<std::basic_filebuf<char>>(t)

// FUNCTIONS
// ---------

/**
 *  \brief Convert C++ mode to C mode.
 */
std::string c_ios_mode(std::ios_base::openmode mode)
{
    using sios = std::ios_base;

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
FILE * get_c_file(const char* narrow, std::ios_base::openmode mode)
{
    auto str = c_ios_mode(mode);
    if (str.size()) {
        return fopen(narrow, str.data());
    }
    return nullptr;
}


#ifdef PYCPP_HAVE_WFOPEN

/**
 *  \brief Get C FILE pointer from wide filename.
 */
FILE * get_c_file(const wchar_t* wide, const std::ios_base::openmode mode)
{
    auto str = c_ios_mode(mode);
    if (str.size()) {
        auto data = reinterpret_cast<const wchar_t*>(codec_utf8_utf16(str).data());
        return _wfopen(wide, data);
    }
    return nullptr;
}


/**
 *  \brief Get C FILE pointer from wide filename.
 */
FILE * get_c_file(const char16_t* wide, const std::ios_base::openmode mode)
{
    auto str = c_ios_mode(mode);
    if (str.size()) {
        auto data = reinterpret_cast<const wchar_t*>(codec_utf8_utf16(str).data());
        return _wfopen(reinterpret_cast<const wchar_t*>(wide), data);
    }
    return nullptr;
}

#endif                              // PYCPP_HAVE_WFOPEN

// OBJECTS
// -------


// FSTREAM

fstream::fstream():
    std::iostream(&buffer)
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


fstream::fstream(const std::string& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void fstream::open(const std::string& name, std::ios_base::openmode mode)
{
#ifdef PYCPP_HAVE_WFOPEN
    open(codec_utf8_utf16(name), mode);
#else
    file = get_c_file(name.data(), mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    std::ios::rdbuf(&buffer);
#endif
}

#ifdef PYCPP_HAVE_WFOPEN

fstream::fstream(const wchar_t* name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void fstream::open(const std::wstring& name, std::ios_base::openmode mode)
{
    file = get_c_file(name.data(), mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    std::ios::rdbuf(&buffer);
}


fstream::fstream(const std::u16string& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void fstream::open(const std::u16string& name, std::ios_base::openmode mode)
{
    file = get_c_file(name.data(), mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    std::ios::rdbuf(&buffer);
}

#endif                              // PYCPP_HAVE_WFOPEN


std::basic_filebuf<char>* fstream::rdbuf() const
{
    return const_cast<std::basic_filebuf<char>*>(&buffer);
}


void fstream::rdbuf(std::basic_filebuf<char> *buffer)
{
    std::ios::rdbuf(buffer);
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
    std::iostream::swap(other);
    std::swap(file, other.file);
    std::swap(buffer, other.buffer);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}


// IFSTREAM

ifstream::ifstream():
    std::istream(&buffer)
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


ifstream::ifstream(const std::string& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void ifstream::open(const std::string& name, std::ios_base::openmode mode)
{
#ifdef PYCPP_HAVE_WFOPEN
    open(codec_utf8_utf16(name), mode);
#else
    file = get_c_file(name.data(), mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    std::ios::rdbuf(&buffer);
#endif
}

#ifdef PYCPP_HAVE_WFOPEN


ifstream::ifstream(const std::wstring& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void ifstream::open(const std::wstring& name, std::ios_base::openmode mode)
{
    file = get_c_file(name.data()), mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    std::ios::rdbuf(&buffer);
}


ifstream::ifstream(const std::u16string& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void ifstream::open(const std::u16string& name, std::ios_base::openmode mode)
{
    file = get_c_file(name.data()), mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    std::ios::rdbuf(&buffer);
}

#endif                              // PYCPP_HAVE_WFOPEN


std::basic_filebuf<char>* ifstream::rdbuf() const
{
    return const_cast<std::basic_filebuf<char>*>(&buffer);
}


void ifstream::rdbuf(std::basic_filebuf<char> *buffer)
{
    std::ios::rdbuf(buffer);
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
    std::istream::swap(other);
    std::swap(file, other.file);
    std::swap(buffer, other.buffer);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}


ofstream::ofstream():
    std::ostream(&buffer)
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


ofstream::ofstream(const std::string& name, std::ios_base::openmode mode)
{
    open(name, mode);
}

void ofstream::open(const std::string& name, std::ios_base::openmode mode)
{
#ifdef PYCPP_HAVE_WFOPEN
    open(codec_utf8_utf16(name), mode);
#else
    file = get_c_file(name.data(), mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    std::ios::rdbuf(&buffer);
#endif
}


#ifdef PYCPP_HAVE_WFOPEN

ofstream::ofstream(const wchar_t* name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void ofstream::open(const std::wstring& name, std::ios_base::openmode mode)
{
    file = get_c_file(name.data(), mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    std::ios::rdbuf(&buffer);
}


ofstream::ofstream(const std::u16string& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void ofstream::open(const std::u16string& name, std::ios_base::openmode mode)
{
    file = get_c_file(name.data(), mode);
    buffer = BASIC_FILEBUF(__gnu_cxx::stdio_filebuf<char>(file, mode));
    std::ios::rdbuf(&buffer);
}

#endif                              // PYCPP_HAVE_WFOPEN


std::basic_filebuf<char>* ofstream::rdbuf() const
{
    return const_cast<std::basic_filebuf<char>*>(&buffer);
}


void ofstream::rdbuf(std::basic_filebuf<char> *buffer)
{
    std::ios::rdbuf(buffer);
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
    std::ostream::swap(other);
    std::swap(file, other.file);
    std::swap(buffer, other.buffer);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

#elif defined(HAVE_MSVC)                // MSVC

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


fstream::fstream(const std::string& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void fstream::open(const std::string& name, std::ios_base::openmode mode)
{
#ifdef PYCPP_HAVE_WFOPEN
    open(codec_utf8_utf16(name), mode);
#else
    std::fstream::open(name, mode);
#endif
}

#ifdef PYCPP_HAVE_WFOPEN


fstream::fstream(const std::wstring& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void fstream::open(const std::wstring& name, std::ios_base::openmode mode)
{
    std::fstream::open(name, mode);
}


fstream::fstream(const std::u16string& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void fstream::open(const std::u16string& name, std::ios_base::openmode mode)
{
    std::fstream::open(reinterpret_cast<const wchar_t*>(name.data()), mode);
}

#endif                              // PYCPP_HAVE_WFOPEN

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


ifstream::ifstream(const std::string& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void ifstream::open(const std::string& name, std::ios_base::openmode mode)
{
#ifdef PYCPP_HAVE_WFOPEN
    open(codec_utf8_utf16(name), mode);
#else
    std::ifstream::open(name, mode);
#endif
}

#ifdef PYCPP_HAVE_WFOPEN        // PYCPP_HAVE_WFOPEN


ifstream::ifstream(const std::wstring& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void ifstream::open(const std::wstring& name, std::ios_base::openmode mode)
{
    std::ifstream::open(name, mode);
}


ifstream::ifstream(const std::u16string& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void ifstream::open(const std::u16string& name, std::ios_base::openmode mode)
{
    std::ifstream::open(reinterpret_cast<const wchar_t*>(name.data()), mode);
}

#endif                          // PYCPP_HAVE_WFOPEN

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


ofstream::ofstream(const std::string& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void ofstream::open(const std::string& name, std::ios_base::openmode mode)
{
#ifdef PYCPP_HAVE_WFOPEN
    open(codec_utf8_utf16(name), mode);
#else
    std::ofstream::open(name, mode);
#endif
}

#ifdef PYCPP_HAVE_WFOPEN


ofstream::ofstream(const std::wstring& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void ofstream::open(const std::wstring& name, std::ios_base::openmode mode)
{
    std::ofstream::open(name, mode);
}


ofstream::ofstream(const std::u16string& name, std::ios_base::openmode mode)
{
    open(name, mode);
}


void ofstream::open(const std::u16string& name, std::ios_base::openmode mode)
{
    std::ofstream::open(reinterpret_cast<const wchar_t*>(name.data()), mode);
}

#endif                              // PYCPP_HAVE_WFOPEN
#endif                              // GCC

PYCPP_END_NAMESPACE
