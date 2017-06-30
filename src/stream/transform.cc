//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  These streambufs are implemented as part of the excellent
 *  reference from Angelika Langer's excellent series.
 */

#include "transform.h"
#include <algorithm>
#include <cstring>

// FUNCTIONS
// ---------


/**
 *  \brief Empty callback, copy src to dst.
 */
static void null_callback(const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    size_t char_size)
{
    size_t bytes = std::min(srclen, dstlen) * char_size;
    const char* src_ = reinterpret_cast<const char*>(src);
    char* dst_ = reinterpret_cast<char*>(dst);

    // copy bytes
    while (bytes--) {
        *dst_++ = *src_++;
    }

    // reassign to buffer
    src = (const void*) src_;
    dst = (void*) dst_;
}

// OBJECTS
// -------

// STREAMBUF

transform_streambuf::transform_streambuf(std::streambuf* f, transform_callback c):
    filebuf(f),
    callback(null_callback)
{
    in_buffer = new char_type[buffer_size];
    out_buffer = new char_type[buffer_size];
    setg(0, 0, 0);
    setp(out_buffer, out_buffer + buffer_size);
    set_callback(c);
}


transform_streambuf::~transform_streambuf()
{
    sync();
    delete[] in_buffer;
    delete[] out_buffer;
}


transform_streambuf::transform_streambuf(transform_streambuf&& other)
{
    swap(other);
}


transform_streambuf& transform_streambuf::operator=(transform_streambuf&& other)
{
    swap(other);
    return *this;
}


void transform_streambuf::swap(transform_streambuf& other)
{
    std::streambuf::swap(other);
    std::swap(filebuf, other.filebuf);
    std::swap(in_buffer, other.in_buffer);
    std::swap(in_first, other.in_first);
    std::swap(in_last, other.in_last);
    std::swap(out_buffer, other.out_buffer);
}


auto transform_streambuf::underflow() -> int_type
{
    size_t distance;
    std::streamsize read, converted;

    if (filebuf) {
        if (in_first == 0) {
            read = filebuf->sgetn(in_buffer, buffer_size);
            in_first = in_buffer;
            in_last = in_buffer + read;
        }

        // convert bytes
        distance = std::distance(in_first, in_last);
        const void* src = (const void*) in_first;
        void* dst = (void*) out_buffer;
        callback(src, distance, dst, buffer_size, sizeof(char_type));
        read = std::distance(in_first, (char*)src);
        converted = std::distance(out_buffer, (char*)dst);

        // store state
        if (read < distance) {
            // overflow in bytes written to dst, store state
            in_first += read;
        } else {
            // fully converted
            in_first = nullptr;
            in_last = nullptr;
        }

        // set get input pointer
        if (!converted) {
            return traits_type::eof();
        }
        setg(out_buffer, out_buffer, out_buffer + converted);

        return traits_type::to_int_type(*gptr());
    }
    return traits_type::eof();
}


auto transform_streambuf::overflow(int_type c) -> int_type
{
    if (filebuf) {
        int write = pptr() - pbase();
        if (write) {
            if (filebuf->sputn(out_buffer, write) != write)  {
                return traits_type::eof();
            }
        }

        // write to output buffer
        setp(out_buffer, out_buffer + buffer_size);
        if (!traits_type::eq_int_type(c, traits_type::eof())) {
            sputc(c);
        }
        return traits_type::not_eof(c);
    }

    return traits_type::eof();
}


int transform_streambuf::sync()
{
    auto result = overflow(traits_type::eof());
    if (filebuf) {
        filebuf->pubsync();
    }

    if (traits_type::eq_int_type(result, traits_type::eof())) {
        return -1;
    }
    return 0;
}


auto transform_streambuf::seekoff(off_type off, std::ios_base::seekdir way, std::ios_base::openmode mode) -> pos_type
{
    pos_type out;
    if (filebuf) {
        out = filebuf->pubseekoff(off, way, mode);
        in_first = in_last = nullptr;
    } else {
        out = pos_type(off_type(-1));
    }

    return out;
}


auto transform_streambuf::seekpos(pos_type pos, std::ios_base::openmode mode) -> pos_type
{
    pos_type out;
    if (filebuf) {
        out = filebuf->pubseekpos(pos, mode);
        in_first = in_last = nullptr;
    } else {
        out = pos_type(off_type(-1));
    }

    return out;
}


void transform_streambuf::set_filebuf(std::streambuf* f)
{
    filebuf = f;
}


void transform_streambuf::set_callback(transform_callback c)
{
    if (c) {
        callback = c;
    } else {
        callback = null_callback;
    }
}


// ISTREAM


transform_istream::transform_istream(transform_callback c):
    buffer(nullptr, c),
    std::istream(&buffer)
{}


transform_istream::~transform_istream()
{}


transform_istream::transform_istream(std::istream& stream, transform_callback c):
    buffer(nullptr, c),
    std::istream(&buffer)
{
    open(stream, c);
}


void transform_istream::open(std::istream& stream, transform_callback c)
{
    this->stream = &stream;
    buffer.set_filebuf(stream.rdbuf());
    buffer.set_callback(c);
}


transform_istream::transform_istream(transform_istream&& other):
    std::istream(std::move(other)),
    stream(std::move(other.stream)),
    buffer(std::move(other.buffer))
{
    std::ios::rdbuf(&buffer);
}


transform_istream & transform_istream::operator=(transform_istream&& other)
{
    swap(other);
    return *this;
}


transform_streambuf* transform_istream::rdbuf() const
{
    return const_cast<transform_streambuf*>(&buffer);
}


void transform_istream::rdbuf(transform_streambuf *buffer)
{
    std::ios::rdbuf(buffer);
}


void transform_istream::swap(transform_istream &other)
{
    // swap
    std::istream::swap(other);
    std::swap(buffer, other.buffer);
    std::swap(stream, other.stream);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

// OSTREAM

transform_ostream::transform_ostream(transform_callback c):
    buffer(nullptr, c),
    std::ostream(&buffer)
{}


transform_ostream::~transform_ostream()
{}


transform_ostream::transform_ostream(std::ostream& stream, transform_callback c):
    buffer(nullptr, c),
    std::ostream(&buffer)
{
    open(stream, c);
}


void transform_ostream::open(std::ostream& stream, transform_callback c)
{
    this->stream = &stream;
    buffer.set_filebuf(stream.rdbuf());
    buffer.set_callback(c);
}


transform_ostream::transform_ostream(transform_ostream&& other):
    std::ostream(std::move(other)),
    stream(std::move(other.stream)),
    buffer(std::move(other.buffer))
{
    std::ios::rdbuf(&buffer);
}


transform_ostream & transform_ostream::operator=(transform_ostream&& other)
{
    swap(other);
    return *this;
}


transform_streambuf* transform_ostream::rdbuf() const
{
    return const_cast<transform_streambuf*>(&buffer);
}


void transform_ostream::rdbuf(transform_streambuf *buffer)
{
    std::ios::rdbuf(buffer);
}


void transform_ostream::swap(transform_ostream &other)
{
    // swap
    std::ostream::swap(other);
    std::swap(buffer, other.buffer);
    std::swap(stream, other.stream);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}

// IFSTREAM

transform_ifstream::transform_ifstream(transform_callback c):
    transform_istream(file, c)
{}


transform_ifstream::~transform_ifstream()
{
    close();
}


transform_ifstream::transform_ifstream(transform_ifstream &&other):
    transform_istream(file)
{
    swap(other);
}


transform_ifstream & transform_ifstream::operator=(transform_ifstream &&other)
{
    swap(other);
    return *this;
}


transform_ifstream::transform_ifstream(const std::string &name, std::ios_base::openmode mode, transform_callback c):
    transform_istream(file, c)
{
    open(name, mode, c);
}


void transform_ifstream::open(const std::string &name, std::ios_base::openmode mode, transform_callback c)
{
    file.open(name, mode);
    rdbuf()->set_callback(c);
}

#ifdef HAVE_WFOPEN

transform_ifstream::transform_ifstream(const std::wstring &name, std::ios_base::openmode mode, transform_callback c):
    transform_istream(file, c)
{
    open(name, mode, c);
}

void transform_ifstream::open(const std::wstring &name, std::ios_base::openmode mode, transform_callback c)
{
    file.open(name, mode);
    rdbuf()->set_callback(c);
}

#endif


bool transform_ifstream::is_open() const
{
    return file.is_open();
}


void transform_ifstream::close()
{
    file.close();
}


void transform_ifstream::swap(transform_ifstream &other)
{
    transform_istream::swap(other);
    std::swap(file, other.file);
}

// OFSTREAM

transform_ofstream::transform_ofstream(transform_callback c):
    transform_ostream(file, c)
{}


transform_ofstream::~transform_ofstream()
{
    close();
}


transform_ofstream::transform_ofstream(transform_ofstream &&other):
    transform_ostream(file)
{
    swap(other);
}


transform_ofstream & transform_ofstream::operator=(transform_ofstream &&other)
{
    swap(other);
    return *this;
}


transform_ofstream::transform_ofstream(const std::string &name, std::ios_base::openmode mode, transform_callback c):
    transform_ostream(file, c)
{
    open(name, mode, c);
}

void transform_ofstream::open(const std::string &name, std::ios_base::openmode mode, transform_callback c)
{
    file.open(name, mode);
    rdbuf()->set_callback(c);
}

#ifdef HAVE_WFOPEN

transform_ofstream::transform_ofstream(const std::wstring &name, std::ios_base::openmode mode, transform_callback c):
    transform_ostream(file, c)
{
    open(name, mode, c);
}

void transform_ofstream::open(const std::wstring &name, std::ios_base::openmode mode, transform_callback c)
{
    file.open(name, mode);
    rdbuf()->set_callback(c);
}

#endif


bool transform_ofstream::is_open() const
{
    return file.is_open();
}


void transform_ofstream::close()
{
    file.close();
}


void transform_ofstream::swap(transform_ofstream &other)
{
    transform_ostream::swap(other);
    std::swap(file, other.file);
}
