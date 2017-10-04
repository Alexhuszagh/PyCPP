//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/stream/fd.h>
#if defined(OS_WINDOWS)
#   include <windows.h>
#else
#   include <unistd.h>
#endif


PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

// STREAMBUF


fd_streambuf::fd_streambuf(std::ios_base::openmode mode, fd_t fd):
    mode(mode),
    fd(fd)
{
    in_buffer = new char_type[buffer_size];
    out_buffer = new char_type[buffer_size];
    if (mode & std::ios_base::in) {
        setg(0, 0, 0);
        setp(out_buffer, out_buffer + buffer_size);
    } else {
        setg(in_buffer, in_buffer, in_buffer+ buffer_size);
        setp(0, 0);
    }
}


fd_streambuf::~fd_streambuf()
{
    close();
}


fd_streambuf::fd_streambuf(fd_streambuf&& other)
{
    swap(other);
}


fd_streambuf& fd_streambuf::operator=(fd_streambuf&& other)
{
    swap(other);
    return *this;
}


void fd_streambuf::close()
{
    sync();

    delete[] in_buffer;
    delete[] out_buffer;
    in_first = nullptr;
    in_last = nullptr;
    in_buffer = nullptr;
    out_buffer = nullptr;
}


void fd_streambuf::swap(fd_streambuf& other)
{
    std::swap(fd, other.fd);
    std::swap(in_buffer, other.in_buffer);
    std::swap(out_buffer, other.out_buffer);
    std::swap(in_first, other.in_first);
    std::swap(in_last, other.in_last);
}


auto fd_streambuf::underflow() -> int_type
{
    if (!(mode & std::ios_base::in)) {
        return traits_type::eof();
    }

    std::streamsize read;

    if (fd != INVALID_FD_VALUE) {
        read = fd_read(fd, in_buffer, buffer_size);
        if (read == -1) {
            return traits_type::eof();
        }
        in_first = in_buffer;
        in_last = in_buffer + read;
        setg(out_buffer, out_buffer, out_buffer + read);
        return traits_type::to_int_type(*gptr());
    }
    return traits_type::eof();
}


auto fd_streambuf::overflow(int_type c) -> int_type
{
    if (!(mode & std::ios_base::out)) {
        return traits_type::eof();
    }

    std::streamsize distance, wrote;
    distance = std::distance(in_first, in_last);
    if (fd != INVALID_FD_VALUE) {
        // TODO: how do I know the out buffer size??
        // Is this right???
        wrote = fd_write(fd, in_first, distance);
        if (wrote == -1) {
            return traits_type::eof();
        }
        in_first += wrote;

        if (!traits_type::eq_int_type(c, traits_type::eof())) {
            *in_last++ = traits_type::to_char_type(c);
        }
    }
    return traits_type::not_eof(c);
}


int fd_streambuf::sync()
{
    auto result = overflow(traits_type::eof());

    // flush buffer on output
    if (fd != INVALID_FD_VALUE && mode & std::ios_base::out) {
        // TODO: is this right??
        std::streamsize distance, wrote;
        distance = std::distance(in_first, in_last);
        wrote = fd_write(fd, in_first, distance);
        in_first += wrote;
    }

    if (traits_type::eq_int_type(result, traits_type::eof())) {
        return -1;
    }
    return 0;
}


void fd_streambuf::set_fd(fd_t fd)
{
    close();
    this->fd = fd;
}


// ISTREAM


fd_stream::fd_stream():
    buffer(std::ios_base::in | std::ios_base::out, INVALID_FD_VALUE),
    std::iostream(&buffer),
    close_(false)
{}


fd_stream::~fd_stream()
{
    close();
}


fd_stream::fd_stream(fd_stream&& other):
    buffer(std::move(other.buffer)),
    close_(std::move(other.close_))
{
    std::ios::rdbuf(&buffer);
}


fd_stream & fd_stream::operator=(fd_stream&& other)
{
    swap(other);
    return *this;
}


fd_stream::fd_stream(fd_t fd, bool close):
    buffer(std::ios_base::in | std::ios_base::out, fd),
    std::iostream(&buffer),
    close_(close)
{}


void fd_stream::open(fd_t fd, bool close)
{
    buffer.set_fd(fd);
    close_ = close;
}


fd_streambuf* fd_stream::rdbuf() const
{
    return const_cast<fd_streambuf*>(&buffer);
}


void fd_stream::rdbuf(fd_streambuf* buffer)
{
    std::ios::rdbuf(buffer);
}



bool fd_stream::is_open() const
{
    return buffer.fd != INVALID_FD_VALUE;
}


void fd_stream::close()
{
    if (close_) {
        buffer.close();
        fd_close(buffer.fd);
        buffer.fd = INVALID_FD_VALUE;
        close_ = false;
    }
}


void fd_stream::swap(fd_stream &other)
{
    // swap
    std::iostream::swap(other);
    std::swap(buffer, other.buffer);
    std::swap(close_, other.close_);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}


fd_istream::fd_istream():
    buffer(std::ios_base::in, INVALID_FD_VALUE),
    std::istream(&buffer),
    close_(false)
{}


fd_istream::~fd_istream()
{
    close();
}


fd_istream::fd_istream(fd_istream&& other):
    buffer(std::move(other.buffer)),
    close_(std::move(other.close_))
{
    std::ios::rdbuf(&buffer);
}


fd_istream & fd_istream::operator=(fd_istream&& other)
{
    swap(other);
    return *this;
}


fd_istream::fd_istream(fd_t fd, bool close):
    buffer(std::ios_base::in, fd),
    std::istream(&buffer),
    close_(close)
{}


void fd_istream::open(fd_t fd, bool close)
{
    buffer.set_fd(fd);
    close_ = close;
}


fd_streambuf* fd_istream::rdbuf() const
{
    return const_cast<fd_streambuf*>(&buffer);
}


void fd_istream::rdbuf(fd_streambuf* buffer)
{
    std::ios::rdbuf(buffer);
}



bool fd_istream::is_open() const
{
    return buffer.fd != INVALID_FD_VALUE;
}


void fd_istream::close()
{
    if (close_) {
        buffer.close();
        fd_close(buffer.fd);
        buffer.fd = INVALID_FD_VALUE;
        close_ = false;
    }
}


void fd_istream::swap(fd_istream &other)
{
    // swap
    std::istream::swap(other);
    std::swap(buffer, other.buffer);
    std::swap(close_, other.close_);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}


// OSTREAM


fd_ostream::fd_ostream():
    buffer(std::ios_base::out, INVALID_FD_VALUE),
    std::ostream(&buffer),
    close_(false)
{}


fd_ostream::~fd_ostream()
{
    close();
}


fd_ostream::fd_ostream(fd_ostream&& other):
    buffer(std::move(other.buffer)),
    close_(std::move(other.close_))
{
    std::ios::rdbuf(&buffer);
}


fd_ostream & fd_ostream::operator=(fd_ostream&& other)
{
    swap(other);
    return *this;
}


fd_ostream::fd_ostream(fd_t fd, bool close):
    buffer(std::ios_base::out, fd),
    std::ostream(&buffer),
    close_(close)
{}


void fd_ostream::open(fd_t fd, bool close)
{
    buffer.set_fd(fd);
    close_ = close;
}


fd_streambuf* fd_ostream::rdbuf() const
{
    return const_cast<fd_streambuf*>(&buffer);
}


void fd_ostream::rdbuf(fd_streambuf* buffer)
{
    std::ios::rdbuf(buffer);
}



bool fd_ostream::is_open() const
{
    return buffer.fd != INVALID_FD_VALUE;
}


void fd_ostream::close()
{
    if (close_) {
        buffer.close();
        fd_close(buffer.fd);
        buffer.fd = INVALID_FD_VALUE;
        close_ = false;
    }
}


void fd_ostream::swap(fd_ostream &other)
{
    // swap
    std::ostream::swap(other);
    std::swap(buffer, other.buffer);
    std::swap(close_, other.close_);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}


PYCPP_END_NAMESPACE
