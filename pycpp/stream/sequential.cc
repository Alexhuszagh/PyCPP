//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/stream/sequential.h>

PYCPP_BEGIN_NAMESPACE

// VARIABLES
// ---------

size_t SEQUENTIAL_BUFFER_SIZE = 8192;

// OBJECTS
// -------

// SEQUENTIAL FSTREAM

sequential_fstream::sequential_fstream():
    buffer(ios_base::in | ios_base::out, INVALID_FD_VALUE, SEQUENTIAL_BUFFER_SIZE),
    iostream(&buffer)
{}


sequential_fstream::~sequential_fstream()
{
    close();
}


sequential_fstream::sequential_fstream(sequential_fstream &&other):
    buffer(PYCPP_NAMESPACE::move(other.buffer)),
    iostream(&buffer)
{
    ios::rdbuf(&buffer);
}


sequential_fstream & sequential_fstream::operator=(sequential_fstream &&other)
{
    swap(other);
    return *this;
}


// TODO: noppppppppeeeeeee
sequential_fstream::sequential_fstream(const string_view& name, ios_base::openmode mode):
    buffer(ios_base::in | ios_base::out, INVALID_FD_VALUE, SEQUENTIAL_BUFFER_SIZE),
    iostream(&buffer)
{
    open(name, mode);
}


void sequential_fstream::open(const string_view& name, ios_base::openmode mode)
{
    close();
    mode |= ios_base::in | ios_base::out;
    buffer.fd(fd_open(name, mode, S_IWR_USR_GRP, access_sequential));
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

sequential_fstream::sequential_fstream(const wstring_view& name, ios_base::openmode mode):
    buffer(ios_base::in | ios_base::out, INVALID_FD_VALUE, SEQUENTIAL_BUFFER_SIZE),
    iostream(&buffer)
{
    open(name, mode);
}


void sequential_fstream::open(const wstring_view& name, ios_base::openmode mode)
{
    open(reinterpret_cast<const char16_t*>(name.data()), mode);
}


sequential_fstream::sequential_fstream(const u16string_view& name, ios_base::openmode mode):
    buffer(ios_base::in | ios_base::out, INVALID_FD_VALUE, SEQUENTIAL_BUFFER_SIZE),
    iostream(&buffer)
{
    open(name, mode);
}


void sequential_fstream::open(const u16string_view& name, ios_base::openmode mode)
{
    close();
    mode |= ios_base::in | ios_base::out;
    buffer.fd(fd_open(name, mode, S_IWR_USR_GRP, access_sequential));
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
    iostream::swap(other);
    PYCPP_NAMESPACE::swap(buffer, other.buffer);

    // set filebuffers
    ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

// SEQUENTIAL IFSTREAM


sequential_ifstream::sequential_ifstream():
    buffer(ios_base::in, INVALID_FD_VALUE, SEQUENTIAL_BUFFER_SIZE),
    istream(&buffer)
{}


sequential_ifstream::~sequential_ifstream()
{
    close();
}


sequential_ifstream::sequential_ifstream(sequential_ifstream &&other):
    buffer(PYCPP_NAMESPACE::move(other.buffer)),
    istream(&buffer)
{
    ios::rdbuf(&buffer);
}


sequential_ifstream & sequential_ifstream::operator=(sequential_ifstream &&other)
{
    swap(other);
    return *this;
}


sequential_ifstream::sequential_ifstream(const string_view& name, ios_base::openmode mode):
    buffer(ios_base::in, INVALID_FD_VALUE, SEQUENTIAL_BUFFER_SIZE),
    istream(&buffer)
{
    open(name, mode);
}


void sequential_ifstream::open(const string_view& name, ios_base::openmode mode)
{
    close();
    mode |= ios_base::in;
    buffer.fd(fd_open(name, mode, S_IWR_USR_GRP, access_sequential));
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

sequential_ifstream::sequential_ifstream(const wstring_view& name, ios_base::openmode mode):
    buffer(ios_base::in, INVALID_FD_VALUE, SEQUENTIAL_BUFFER_SIZE),
    istream(&buffer)
{
    open(name, mode);
}


void sequential_ifstream::open(const wstring_view& name, ios_base::openmode mode)
{
    open(reinterpret_cast<const char16_t*>(name.data()), mode);
}


sequential_ifstream::sequential_ifstream(const u16string_view& name, ios_base::openmode mode):
    buffer(ios_base::in, INVALID_FD_VALUE, SEQUENTIAL_BUFFER_SIZE),
    istream(&buffer)
{
    open(name, mode);
}


void sequential_ifstream::open(const u16string_view& name, ios_base::openmode mode)
{
    close();
    mode |= ios_base::in;
    buffer.fd(fd_open(name, mode, S_IWR_USR_GRP, access_sequential));
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
    istream::swap(other);
    PYCPP_NAMESPACE::swap(buffer, other.buffer);

    // set filebuffers
    ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

// SEQUENTIAL OFSTREAM

sequential_ofstream::sequential_ofstream():
    buffer(ios_base::out, INVALID_FD_VALUE, SEQUENTIAL_BUFFER_SIZE),
    ostream(&buffer)
{}


sequential_ofstream::~sequential_ofstream()
{
    close();
}


sequential_ofstream::sequential_ofstream(sequential_ofstream &&other):
    buffer(PYCPP_NAMESPACE::move(other.buffer)),
    ostream(&buffer)
{
    ios::rdbuf(&buffer);
}


sequential_ofstream & sequential_ofstream::operator=(sequential_ofstream &&other)
{
    swap(other);
    return *this;
}


sequential_ofstream::sequential_ofstream(const string_view& name, ios_base::openmode mode):
    buffer(ios_base::out, INVALID_FD_VALUE, SEQUENTIAL_BUFFER_SIZE),
    ostream(&buffer)
{
    open(name, mode);
}


void sequential_ofstream::open(const string_view& name, ios_base::openmode mode)
{
    close();
    mode |= ios_base::out;
    buffer.fd(fd_open(name, mode, S_IWR_USR_GRP, access_sequential));
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

sequential_ofstream::sequential_ofstream(const wstring_view& name, ios_base::openmode mode):
    buffer(ios_base::out, INVALID_FD_VALUE, SEQUENTIAL_BUFFER_SIZE),
    ostream(&buffer)
{
    open(name, mode);
}


void sequential_ofstream::open(const wstring_view& name, ios_base::openmode mode)
{
    open(reinterpret_cast<const char16_t*>(name.data()), mode);
}


sequential_ofstream::sequential_ofstream(const u16string_view& name, ios_base::openmode mode):
    buffer(ios_base::out, INVALID_FD_VALUE, SEQUENTIAL_BUFFER_SIZE),
    ostream(&buffer)
{
    open(name, mode);
}


void sequential_ofstream::open(const u16string_view& name, ios_base::openmode mode)
{
    close();
    mode |= ios_base::out;
    buffer.fd(fd_open(name, mode, S_IWR_USR_GRP, access_sequential));
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
    ostream::swap(other);
    PYCPP_NAMESPACE::swap(buffer, other.buffer);

    // set filebuffers
    ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

PYCPP_END_NAMESPACE
