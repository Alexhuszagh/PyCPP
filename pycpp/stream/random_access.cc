//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/stream/random_access.h>

PYCPP_BEGIN_NAMESPACE

// VARIABLES
// ---------

size_t RANDOM_ACCESS_BUFFER_SIZE = 512;

// OBJECTS
// -------

// RANDOM ACCESS FSTREAM

random_access_fstream::random_access_fstream():
    buffer(ios_base::in | ios_base::out, INVALID_FD_VALUE, RANDOM_ACCESS_BUFFER_SIZE),
    iostream(&buffer)
{}


random_access_fstream::~random_access_fstream()
{
    close();
}


random_access_fstream::random_access_fstream(random_access_fstream &&other):
    buffer(PYCPP_NAMESPACE::move(other.buffer)),
    iostream(&buffer)
{
    ios::rdbuf(&buffer);
}


random_access_fstream & random_access_fstream::operator=(random_access_fstream &&other)
{
    swap(other);
    return *this;
}


random_access_fstream::random_access_fstream(const string_view& name, ios_base::openmode mode):
    buffer(ios_base::in | ios_base::out, INVALID_FD_VALUE, RANDOM_ACCESS_BUFFER_SIZE),
    iostream(&buffer)
{
    open(name, mode);
}


void random_access_fstream::open(const string_view& name, ios_base::openmode mode)
{
    close();
    mode |= ios_base::in | ios_base::out;
    buffer.fd(fd_open(name, mode, S_IWR_USR_GRP, access_random));
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

random_access_fstream::random_access_fstream(const wstring_view& name, ios_base::openmode mode):
    buffer(ios_base::in | ios_base::out, INVALID_FD_VALUE, RANDOM_ACCESS_BUFFER_SIZE),
    iostream(&buffer)
{
    open(name, mode);
}


void random_access_fstream::open(const wstring_view& name, ios_base::openmode mode)
{
    open(reinterpret_cast<const char16_t*>(name.data()), mode);
}


random_access_fstream::random_access_fstream(const u16string_view& name, ios_base::openmode mode):
    buffer(ios_base::in | ios_base::out, INVALID_FD_VALUE, RANDOM_ACCESS_BUFFER_SIZE),
    iostream(&buffer)
{
    open(name, mode);
}


void random_access_fstream::open(const u16string_view& name, ios_base::openmode mode)
{
    close();
    mode |= ios_base::in | ios_base::out;
    buffer.fd(fd_open(name, mode, S_IWR_USR_GRP, access_random));
}

#endif                                          // WINDOWS

bool random_access_fstream::is_open() const
{
    return buffer.is_open();
}

void random_access_fstream::close()
{
    if (buffer.fd() != INVALID_FD_VALUE) {
        buffer.close();
        fd_close(buffer.fd());
        buffer.fd(INVALID_FD_VALUE);
    }
}


void random_access_fstream::swap(random_access_fstream &other)
{
    // swap
    iostream::swap(other);
    PYCPP_NAMESPACE::swap(buffer, other.buffer);

    // set filebuffers
    ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

// RANDOM ACCESS IFSTREAM


random_access_ifstream::random_access_ifstream():
    buffer(ios_base::in, INVALID_FD_VALUE, RANDOM_ACCESS_BUFFER_SIZE),
    istream(&buffer)
{}


random_access_ifstream::~random_access_ifstream()
{
    close();
}


random_access_ifstream::random_access_ifstream(random_access_ifstream &&other):
    buffer(PYCPP_NAMESPACE::move(other.buffer)),
    istream(&buffer)
{
    ios::rdbuf(&buffer);
}


random_access_ifstream & random_access_ifstream::operator=(random_access_ifstream &&other)
{
    swap(other);
    return *this;
}


random_access_ifstream::random_access_ifstream(const string_view& name, ios_base::openmode mode):
    buffer(ios_base::in, INVALID_FD_VALUE, RANDOM_ACCESS_BUFFER_SIZE),
    istream(&buffer)
{
    open(name, mode);
}


void random_access_ifstream::open(const string_view& name, ios_base::openmode mode)
{
    close();
    mode |= ios_base::in;
    buffer.fd(fd_open(name, mode, S_IWR_USR_GRP, access_random));
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

random_access_ifstream::random_access_ifstream(const wstring_view& name, ios_base::openmode mode):
    buffer(ios_base::in, INVALID_FD_VALUE, RANDOM_ACCESS_BUFFER_SIZE),
    istream(&buffer)
{
    open(name, mode);
}


void random_access_ifstream::open(const wstring_view& name, ios_base::openmode mode)
{
    open(reinterpret_cast<const char16_t*>(name.data()), mode);
}


random_access_ifstream::random_access_ifstream(const u16string_view& name, ios_base::openmode mode):
    buffer(ios_base::in, INVALID_FD_VALUE, RANDOM_ACCESS_BUFFER_SIZE),
    istream(&buffer)
{
    open(name, mode);
}


void random_access_ifstream::open(const u16string_view& name, ios_base::openmode mode)
{
    close();
    mode |= ios_base::in;
    buffer.fd(fd_open(name, mode, S_IWR_USR_GRP, access_random));
}

#endif                                          // WINDOWS

bool random_access_ifstream::is_open() const
{
    return buffer.is_open();
}

void random_access_ifstream::close()
{
    if (buffer.fd() != INVALID_FD_VALUE) {
        buffer.close();
        fd_close(buffer.fd());
        buffer.fd(INVALID_FD_VALUE);
    }
}


void random_access_ifstream::swap(random_access_ifstream &other)
{
    // swap
    istream::swap(other);
    PYCPP_NAMESPACE::swap(buffer, other.buffer);

    // set filebuffers
    ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

// RANDOM ACCESS OFSTREAM

random_access_ofstream::random_access_ofstream():
    buffer(ios_base::out, INVALID_FD_VALUE, RANDOM_ACCESS_BUFFER_SIZE),
    ostream(&buffer)
{}


random_access_ofstream::~random_access_ofstream()
{
    close();
}


random_access_ofstream::random_access_ofstream(random_access_ofstream &&other):
    buffer(PYCPP_NAMESPACE::move(other.buffer)),
    ostream(&buffer)
{
    ios::rdbuf(&buffer);
}


random_access_ofstream & random_access_ofstream::operator=(random_access_ofstream &&other)
{
    swap(other);
    return *this;
}


random_access_ofstream::random_access_ofstream(const string_view& name, ios_base::openmode mode):
    buffer(ios_base::out, INVALID_FD_VALUE, RANDOM_ACCESS_BUFFER_SIZE),
    ostream(&buffer)
{
    open(name, mode);
}


void random_access_ofstream::open(const string_view& name, ios_base::openmode mode)
{
    close();
    mode |= ios_base::out;
    buffer.fd(fd_open(name, mode, S_IWR_USR_GRP, access_random));
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

random_access_ofstream::random_access_ofstream(const wstring_view& name, ios_base::openmode mode):
    buffer(ios_base::out, INVALID_FD_VALUE, RANDOM_ACCESS_BUFFER_SIZE),
    ostream(&buffer)
{
    open(name, mode);
}


void random_access_ofstream::open(const wstring_view& name, ios_base::openmode mode)
{
    open(reinterpret_cast<const char16_t*>(name.data()), mode);
}


random_access_ofstream::random_access_ofstream(const u16string_view& name, ios_base::openmode mode):
    buffer(ios_base::out, INVALID_FD_VALUE, RANDOM_ACCESS_BUFFER_SIZE),
    ostream(&buffer)
{
    open(name, mode);
}


void random_access_ofstream::open(const u16string_view& name, ios_base::openmode mode)
{
    close();
    mode |= ios_base::out;
    buffer.fd(fd_open(name, mode, S_IWR_USR_GRP, access_random));
}

#endif                                          // WINDOWS

bool random_access_ofstream::is_open() const
{
    return buffer.is_open();
}

void random_access_ofstream::close()
{
    if (buffer.fd() != INVALID_FD_VALUE) {
        buffer.close();
        fd_close(buffer.fd());
        buffer.fd(INVALID_FD_VALUE);
    }
}


void random_access_ofstream::swap(random_access_ofstream &other)
{
    // swap
    ostream::swap(other);
    PYCPP_NAMESPACE::swap(buffer, other.buffer);

    // set filebuffers
    ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

PYCPP_END_NAMESPACE
