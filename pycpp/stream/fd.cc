//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/stream/fd.h>
#if !defined(OS_WINDOWS)
#   include <unistd.h>
#endif


PYCPP_BEGIN_NAMESPACE

// VARIABLES
// ---------

size_t DEFAULT_BUFFER_SIZE = 4096;

// OBJECTS
// -------

// STREAMBUF


fd_streambuf::fd_streambuf(ios_base::openmode mode, fd_t fd):
    mode(mode),
    fd_(fd),
    buffer_size(DEFAULT_BUFFER_SIZE)
{
    initialize_buffers();
}


fd_streambuf::fd_streambuf(ios_base::openmode mode, fd_t fd, size_t buffer_size):
    mode(mode),
    fd_(fd),
    buffer_size(buffer_size)
{
    initialize_buffers();
}


fd_streambuf::~fd_streambuf()
{
    delete[] in_first;
    delete[] out_first;
}


fd_streambuf::fd_streambuf(fd_streambuf&& rhs)
{
    swap(rhs);
}


fd_streambuf& fd_streambuf::operator=(fd_streambuf&& rhs)
{
    swap(rhs);
    return *this;
}


void fd_streambuf::close()
{
    sync();
}


bool fd_streambuf::is_open() const
{
    return fd_ != INVALID_FD_VALUE;
}


void fd_streambuf::swap(fd_streambuf& rhs)
{
    using PYCPP_NAMESPACE::swap;

    swap(mode, rhs.mode);
    swap(buffer_size, rhs.buffer_size);
    swap(fd_, rhs.fd_);
    swap(in_first, rhs.in_first);
    swap(in_last, rhs.in_last);
    swap(out_first, rhs.out_first);
    swap(out_last, rhs.out_last);
    streambuf::swap(rhs);
}


auto fd_streambuf::underflow() -> int_type
{
    if (!(mode & ios_base::in)) {
        return traits_type::eof();
    }

    streamsize read;
    if (fd_ != INVALID_FD_VALUE) {
        set_readp();
        do {
            read = fd_read(fd_, in_first, buffer_size);
        } while (read == -1 && errno == EINTR);
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
    if (!(mode & ios_base::out)) {
        return traits_type::eof();
    }

    streamsize dist, wrote;
    if (fd_ != INVALID_FD_VALUE) {
        set_writep();
        dist = distance(out_first, out_last);
        if (dist == buffer_size) {
            do {
                wrote = fd_write(fd_, out_first, dist);
            } while (wrote == -1 && errno == EINTR);
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
    streamsize dist, wrote;
    if (fd_ != INVALID_FD_VALUE && mode & ios_base::out) {
        dist = distance(out_first, out_last);
        if (dist > 0) {
            do {
                wrote = fd_write(fd_, out_first, dist);
            } while (wrote == -1 && errno == EINTR);
            out_last = out_first;
        }
    }

    if (traits_type::eq_int_type(result, traits_type::eof())) {
        return -1;
    }
    return 0;
}


auto fd_streambuf::seekoff(off_type off, ios_base::seekdir way, ios_base::openmode) -> pos_type
{
    return fd_seek(fd_, off, way);
}


auto fd_streambuf::seekpos(pos_type pos, ios_base::openmode) -> pos_type
{
    return fd_seek(fd_, pos);
}


fd_t fd_streambuf::fd() const
{
    return fd_;
}


void fd_streambuf::fd(fd_t fd)
{
    close();
    fd_ = fd;
}


void fd_streambuf::initialize_buffers()
{
    if (mode & ios_base::in) {
        in_first = new char_type[buffer_size];
    }
    if (mode & ios_base::out) {
        out_first = new char_type[buffer_size];
        out_last = out_first;
    }

    setg(0, 0, 0);
    setp(0, 0);
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
    buffer(ios_base::in | ios_base::out, INVALID_FD_VALUE),
    iostream(&buffer),
    close_(false)
{}


fd_stream::~fd_stream()
{
    close();
}


fd_stream::fd_stream(fd_stream&& rhs):
    buffer(PYCPP_NAMESPACE::move(rhs.buffer)),
    iostream(&buffer),
    close_(PYCPP_NAMESPACE::move(rhs.close_))
{
    ios::rdbuf(&buffer);
}


fd_stream & fd_stream::operator=(fd_stream&& rhs)
{
    swap(rhs);
    return *this;
}


fd_stream::fd_stream(fd_t fd, bool close):
    buffer(ios_base::in | ios_base::out, fd),
    iostream(&buffer),
    close_(close)
{}


void fd_stream::open(fd_t fd, bool c)
{
    close();
    buffer.fd(fd);
    close_ = c;
}


streambuf* fd_stream::rdbuf() const
{
    return ios::rdbuf();
}


void fd_stream::rdbuf(streambuf* buffer)
{
    ios::rdbuf(buffer);
}


bool fd_stream::is_open() const
{
    return buffer.is_open();
}


void fd_stream::close()
{
    if (close_) {
        buffer.close();
        fd_close(buffer.fd());
        buffer.fd(INVALID_FD_VALUE);
        close_ = false;
    }
}


void fd_stream::swap(fd_stream& rhs)
{
    using PYCPP_NAMESPACE::swap;

    // swap
    iostream::swap(rhs);
    swap(buffer, rhs.buffer);
    swap(close_, rhs.close_);
}


fd_istream::fd_istream():
    buffer(ios_base::in, INVALID_FD_VALUE),
    istream(&buffer),
    close_(false)
{}


fd_istream::~fd_istream()
{
    close();
}


fd_istream::fd_istream(fd_istream&& rhs):
    buffer(PYCPP_NAMESPACE::move(rhs.buffer)),
    istream(&buffer),
    close_(PYCPP_NAMESPACE::move(rhs.close_))
{
    ios::rdbuf(&buffer);
}


fd_istream & fd_istream::operator=(fd_istream&& rhs)
{
    swap(rhs);
    return *this;
}


fd_istream::fd_istream(fd_t fd, bool close):
    buffer(ios_base::in, fd),
    istream(&buffer),
    close_(close)
{}


void fd_istream::open(fd_t fd, bool c)
{
    close();
    buffer.fd(fd);
    close_ = c;
}


streambuf* fd_istream::rdbuf() const
{
    return ios::rdbuf();
}


void fd_istream::rdbuf(streambuf* buffer)
{
    ios::rdbuf(buffer);
}



bool fd_istream::is_open() const
{
    return buffer.is_open();
}


void fd_istream::close()
{
    if (close_) {
        buffer.close();
        fd_close(buffer.fd());
        buffer.fd(INVALID_FD_VALUE);
        close_ = false;
    }
}


void fd_istream::swap(fd_istream& rhs)
{
    using PYCPP_NAMESPACE::swap;

    // swap
    istream::swap(rhs);
    swap(buffer, rhs.buffer);
    swap(close_, rhs.close_);
}


// OSTREAM


fd_ostream::fd_ostream():
    buffer(ios_base::out, INVALID_FD_VALUE),
    ostream(&buffer),
    close_(false)
{}


fd_ostream::~fd_ostream()
{
    close();
}


fd_ostream::fd_ostream(fd_ostream&& rhs):
    buffer(PYCPP_NAMESPACE::move(rhs.buffer)),
    ostream(&buffer),
    close_(PYCPP_NAMESPACE::move(rhs.close_))
{
    ios::rdbuf(&buffer);
}


fd_ostream & fd_ostream::operator=(fd_ostream&& rhs)
{
    swap(rhs);
    return *this;
}


fd_ostream::fd_ostream(fd_t fd, bool close):
    buffer(ios_base::out, fd),
    ostream(&buffer),
    close_(close)
{}


void fd_ostream::open(fd_t fd, bool c)
{
    close();
    buffer.fd(fd);
    close_ = c;
}


streambuf* fd_ostream::rdbuf() const
{
    return ios::rdbuf();
}


void fd_ostream::rdbuf(streambuf* buffer)
{
    ios::rdbuf(buffer);
}



bool fd_ostream::is_open() const
{
    return buffer.is_open();
}


void fd_ostream::close()
{
    if (close_) {
        buffer.close();
        fd_close(buffer.fd());
        buffer.fd(INVALID_FD_VALUE);
        close_ = false;
    }
}


void fd_ostream::swap(fd_ostream& rhs)
{
    using PYCPP_NAMESPACE::swap;

    // swap
    ostream::swap(rhs);
    swap(buffer, rhs.buffer);
    swap(close_, rhs.close_);
}


PYCPP_END_NAMESPACE
