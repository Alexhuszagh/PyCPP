//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/stream/sequential.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

// SEQUENTIAL FSTREAM

sequential_fstream::sequential_fstream():
    buffer(std::ios_base::in | std::ios_base::out, INVALID_FD_VALUE),
    std::iostream(&buffer)
{}


sequential_fstream::~sequential_fstream()
{
    close();
}


sequential_fstream::sequential_fstream(sequential_fstream &&other):
    buffer(std::move(other.buffer)),
    std::iostream(&buffer)
{
    std::ios::rdbuf(&buffer);
}


sequential_fstream & sequential_fstream::operator=(sequential_fstream &&other)
{
    swap(other);
    return *this;
}


sequential_fstream::sequential_fstream(const std::string& name, std::ios_base::openmode mode):
    buffer(std::ios_base::in | std::ios_base::out, INVALID_FD_VALUE),
    std::iostream(&buffer)
{
    open(name, mode);
}


void sequential_fstream::open(const std::string& name, std::ios_base::openmode mode)
{
    close();
// TODO: restore....
    //    buffer.fd(fd_open(name, mode, access_sequential));
    buffer.fd(fd_open(name, mode));
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

sequential_fstream::sequential_fstream(const std::wstring& name, std::ios_base::openmode mode):
    buffer(std::ios_base::in | std::ios_base::out, INVALID_FD_VALUE),
    std::iostream(&buffer)
{
    open(name, mode);
}


void sequential_fstream::open(const std::wstring& name, std::ios_base::openmode mode)
{
    open(reinterpret_cast<const char16_t*>(name.data()), mode);
}


sequential_fstream::sequential_fstream(const std::u16string& name, std::ios_base::openmode mode):
    buffer(std::ios_base::in | std::ios_base::out, INVALID_FD_VALUE),
    std::iostream(&buffer)
{
    open(name, mode);
}


void sequential_fstream::open(const std::u16string& name, std::ios_base::openmode mode)
{
    close();
// TODO: restore....
    //    buffer.fd(fd_open(name, mode, access_sequential));
    buffer.fd(fd_open(name, mode));
}

#endif                                          // WINDOWS

bool sequential_fstream::is_open() const
{
    return buffer.is_open();
}

void sequential_fstream::close()
{
    if (buffer.fd() != INVALID_FD_VALUE) {
        buffer.close();
        fd_close(buffer.fd());
        buffer.fd(INVALID_FD_VALUE);
    }
}


void sequential_fstream::swap(sequential_fstream &other)
{
    // swap
    std::iostream::swap(other);
    std::swap(buffer, other.buffer);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

// SEQUENTIAL IFSTREAM


sequential_ifstream::sequential_ifstream():
    buffer(std::ios_base::in, INVALID_FD_VALUE),
    std::istream(&buffer)
{}


sequential_ifstream::~sequential_ifstream()
{
    close();
}


sequential_ifstream::sequential_ifstream(sequential_ifstream &&other):
    buffer(std::move(other.buffer)),
    std::istream(&buffer)
{
    std::ios::rdbuf(&buffer);
}


sequential_ifstream & sequential_ifstream::operator=(sequential_ifstream &&other)
{
    swap(other);
    return *this;
}


sequential_ifstream::sequential_ifstream(const std::string& name, std::ios_base::openmode mode):
    buffer(std::ios_base::in, INVALID_FD_VALUE),
    std::istream(&buffer)
{
    open(name, mode);
}


void sequential_ifstream::open(const std::string& name, std::ios_base::openmode mode)
{
    close();
    buffer.fd(fd_open(name, mode, access_sequential));
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

sequential_ifstream::sequential_ifstream(const std::wstring& name, std::ios_base::openmode mode):
    buffer(std::ios_base::in, INVALID_FD_VALUE),
    std::istream(&buffer)
{
    open(name, mode);
}


void sequential_ifstream::open(const std::wstring& name, std::ios_base::openmode mode)
{
    open(reinterpret_cast<const char16_t*>(name.data()), mode);
}


sequential_ifstream::sequential_ifstream(const std::u16string& name, std::ios_base::openmode mode):
    buffer(std::ios_base::in, INVALID_FD_VALUE),
    std::istream(&buffer)
{
    open(name, mode);
}


void sequential_ifstream::open(const std::u16string& name, std::ios_base::openmode mode)
{
    close();
// TODO: restore....
    //    buffer.fd(fd_open(name, mode, access_sequential));
    buffer.fd(fd_open(name, mode));
}

#endif                                          // WINDOWS

bool sequential_ifstream::is_open() const
{
    return buffer.is_open();
}

void sequential_ifstream::close()
{
    if (buffer.fd() != INVALID_FD_VALUE) {
        buffer.close();
        fd_close(buffer.fd());
        buffer.fd(INVALID_FD_VALUE);
    }
}


void sequential_ifstream::swap(sequential_ifstream &other)
{
    // swap
    std::istream::swap(other);
    std::swap(buffer, other.buffer);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

// SEQUENTIAL OFSTREAM

sequential_ofstream::sequential_ofstream():
    buffer(std::ios_base::out, INVALID_FD_VALUE),
    std::ostream(&buffer)
{}


sequential_ofstream::~sequential_ofstream()
{
    close();
}


sequential_ofstream::sequential_ofstream(sequential_ofstream &&other):
    buffer(std::move(other.buffer)),
    std::ostream(&buffer)
{
    std::ios::rdbuf(&buffer);
}


sequential_ofstream & sequential_ofstream::operator=(sequential_ofstream &&other)
{
    swap(other);
    return *this;
}


sequential_ofstream::sequential_ofstream(const std::string& name, std::ios_base::openmode mode):
    buffer(std::ios_base::out, INVALID_FD_VALUE),
    std::ostream(&buffer)
{
    open(name, mode);
}


void sequential_ofstream::open(const std::string& name, std::ios_base::openmode mode)
{
    close();
// TODO: restore....
    //    buffer.fd(fd_open(name, mode, access_sequential));
    buffer.fd(fd_open(name, mode));
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

sequential_ofstream::sequential_ofstream(const std::wstring& name, std::ios_base::openmode mode):
    buffer(std::ios_base::out, INVALID_FD_VALUE),
    std::ostream(&buffer)
{
    open(name, mode);
}


void sequential_ofstream::open(const std::wstring& name, std::ios_base::openmode mode)
{
    open(reinterpret_cast<const char16_t*>(name.data()), mode);
}


sequential_ofstream::sequential_ofstream(const std::u16string& name, std::ios_base::openmode mode):
    buffer(std::ios_base::out, INVALID_FD_VALUE),
    std::ostream(&buffer)
{
    open(name, mode);
}


void sequential_ofstream::open(const std::u16string& name, std::ios_base::openmode mode)
{
    close();
// TODO: restore....
    //    buffer.fd(fd_open(name, mode, access_sequential));
    buffer.fd(fd_open(name, mode));
}

#endif                                          // WINDOWS

bool sequential_ofstream::is_open() const
{
    return buffer.is_open();
}

void sequential_ofstream::close()
{
    if (buffer.fd() != INVALID_FD_VALUE) {
        buffer.close();
        fd_close(buffer.fd());
        buffer.fd(INVALID_FD_VALUE);
    }
}


void sequential_ofstream::swap(sequential_ofstream &other)
{
    // swap
    std::ostream::swap(other);
    std::swap(buffer, other.buffer);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

PYCPP_END_NAMESPACE
