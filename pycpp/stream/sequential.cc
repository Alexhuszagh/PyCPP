//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/stream/sequential.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


sequential_fstream::sequential_fstream():
    buffer(std::ios_base::in | std::ios_base::out, INVALID_FD_VALUE),
    std::iostream(&buffer)
{}


sequential_fstream::~sequential_fstream()
{
    close();
}


sequential_fstream::sequential_fstream(sequential_fstream &&other):
    buffer(std::ios_base::in | std::ios_base::out, INVALID_FD_VALUE),
    std::iostream(&buffer)
{
    swap(other);
}


sequential_fstream & sequential_fstream::operator=(sequential_fstream &&other)
{
    swap(other);
    return *this;
}


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

// POSIX
// int posix_fadvise(int fd, off_t offset, off_t len, int advice);

// WINDOWS
//FILE_FLAG_RANDOM_ACCESS

PYCPP_END_NAMESPACE
