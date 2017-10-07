//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/stream/fd.h>
#if !defined(OS_WINDOWS)
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
    if (mode & std::ios_base::in) {
        in_first = new char_type[buffer_size];
    }
    if (mode & std::ios_base::out) {
        out_first = new char_type[buffer_size];
        out_last = out_first;
    }

    setg(0, 0, 0);
    setp(0, 0);
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

    delete[] in_first;
    delete[] out_first;
    in_first = nullptr;
    in_last = nullptr;
    out_first = nullptr;
    out_last = nullptr;
}


void fd_streambuf::swap(fd_streambuf& other)
{
    std::swap(fd, other.fd);
    std::swap(in_first, other.in_first);
    std::swap(in_last, other.in_last);
    std::swap(out_first, other.out_first);
    std::swap(out_last, other.out_last);

    // reset internal buffer pointers
    setg(0, 0, 0);
    setp(0, 0);
    other.setg(0, 0, 0);
    other.setp(0, 0);
}


auto fd_streambuf::underflow() -> int_type
{
    if (!(mode & std::ios_base::in)) {
        return traits_type::eof();
    }

    set_readp();

    std::streamsize read;
    if (fd != INVALID_FD_VALUE) {
        read = fd_read(fd, in_first, buffer_size);
        if (read == 0 || read == -1) {
            // 0 indicates EOF, -1 indicates error.
            return traits_type::eof();
        }
        in_last = in_first + read;
        setg(in_first, in_first, in_first + read);
        return traits_type::to_int_type(*gptr());
    }
    return traits_type::eof();
}


auto fd_streambuf::overflow(int_type c) -> int_type
{
    if (!(mode & std::ios_base::out)) {
        return traits_type::eof();
    }

    set_writep();

    std::streamsize distance, wrote;
    if (fd != INVALID_FD_VALUE) {
        distance = std::distance(out_first, out_last);
        if (distance == buffer_size) {
            wrote = fd_write(fd, out_first, distance);
            out_last = out_first;
        }

        if (!traits_type::eq_int_type(c, traits_type::eof())) {
            *out_last++ = traits_type::to_char_type(c);
        }
    }
    return traits_type::not_eof(c);
}


int fd_streambuf::sync()
{
    auto result = overflow(traits_type::eof());

    // flush buffer on output
    std::streamsize distance, wrote;
    if (fd != INVALID_FD_VALUE && mode & std::ios_base::out) {
        distance = std::distance(out_first, out_last);
        if (distance > 0) {
            wrote = fd_write(fd, out_first, distance);
            out_last = out_first;
        }
    }

    if (traits_type::eq_int_type(result, traits_type::eof())) {
        return -1;
    }
    return 0;
}


auto fd_streambuf::seekoff(off_type off, std::ios_base::seekdir way, std::ios_base::openmode) -> pos_type
{
    return fd_seek(fd, off, way);
}


auto fd_streambuf::seekpos(pos_type pos, std::ios_base::openmode) -> pos_type
{
    return fd_seek(fd, pos);
}


void fd_streambuf::set_fd(fd_t fd)
{
    close();
    this->fd = fd;
}


void fd_streambuf::set_readp()
{
    setp(in_first, in_first + buffer_size);
    setg(0, 0, 0);
}


void fd_streambuf::set_writep()
{
    setg(out_first, out_first, out_first + buffer_size);
    setp(0, 0);
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
    std::iostream(&buffer),
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
    std::istream(&buffer),
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
    std::ostream(&buffer),
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
