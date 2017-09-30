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

//// HELPERS
//// -------
//
//static void handle_seek_error(int code)
//{
//    switch (code) {
//        case 0:
//            return;
//        case EBADF:
//            throw filesystem_error(filesystem_file_descriptor_not_open);
//        case EINVAL:
//            throw filesystem_error(filesystem_seek_offset_not_valid);
//        case ENXIO:
//            throw filesystem_error(filesystem_seek_offset_beyond_file);
//        case EOVERFLOW:
//            throw filesystem_error(filesystem_seek_offset_overflow);
//        case ESPIPE:
//            throw filesystem_error(filesystem_invalid_seek);
//        default:
//            throw filesystem_error(filesystem_unexpected_error);
//    }
//}
//
//
//#if defined(OS_WINDOWS)                     // WINDOWS
//
///**
// *  \brief Map GetLastError to errno.
// */
//static int get_error_code()
//{
//    DWORD error = GetLastError();
//    switch (error) {
////        case ERROR_FILE_NOT_FOUND:
////        case ERROR_PATH_NOT_FOUND:
////            return ENOENT;
////        case ERROR_BAD_ARGUMENTS:
////            return EINVAL;
////        case ERROR_TOO_MANY_OPEN_FILES:
////            return EMFILE;
////        case ERROR_ACCESS_DENIED:
////            if (PYCPP_SKIP_PERMISSIONS_ERROR) {
////                return 0;
////            }
////            return EACCES;
////        case ERROR_NOT_ENOUGH_MEMORY:
////        case ERROR_OUTOFMEMORY:
////            return ENOMEM;
//        default:
//            return -1;
//    }
//}
//
//#else                                       // POSIX
//
//static int get_error_code()
//{
//    return errno;
//}
//
//#endif                                      // WINDOWS


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
        // TODO: is this right??
        read = fd_read(fd, in_buffer, buffer_size);
        if (read == -1) {
            // TODO: set badbit.
            // TODO: return EOF
            // TODO: check clang's implementation
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
            // TODO: set badbit.
            // TODO: return EOF
            // TODO: check clang's implementation
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
    this->fd = fd;
}

PYCPP_END_NAMESPACE
