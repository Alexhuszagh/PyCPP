//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/stl/algorithm.h>
#include <pycpp/stream/filter.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


/**
 *  \brief Empty callback, copy src to dst.
 */
static void null_callback(const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    size_t char_size)
{
    size_t bytes = min(srclen, dstlen) * char_size;
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

filter_streambuf::filter_streambuf(ios_base::openmode m, streambuf* f, filter_callback c):
    mode(m),
    filebuf(f),
    callback(null_callback)
{
    in_buffer = new char_type[buffer_size];
    out_buffer = new char_type[buffer_size];
    set_pointers();
    set_callback(c);
}


filter_streambuf::~filter_streambuf()
{
    close();
}


filter_streambuf::filter_streambuf(filter_streambuf&& rhs):
    filter_streambuf(rhs.mode)
{
    swap(rhs);
}


filter_streambuf& filter_streambuf::operator=(filter_streambuf&& rhs)
{
    swap(rhs);
    return *this;
}


void filter_streambuf::close()
{
    sync();
    streamsize converted = do_callback();
    if (converted && filebuf && mode & ios_base::out) {
        filebuf->sputn(out_buffer, converted);
    }

    delete[] in_buffer;
    delete[] out_buffer;
    filebuf = nullptr;
    first = nullptr;
    last = nullptr;
    in_buffer = nullptr;
    out_buffer = nullptr;
}


void filter_streambuf::swap(filter_streambuf& rhs)
{
    using PYCPP_NAMESPACE::swap;

    swap(filebuf, rhs.filebuf);
    swap(callback, rhs.callback);
    swap(in_buffer, rhs.in_buffer);
    swap(out_buffer, rhs.out_buffer);
    swap(first, rhs.first);
    swap(last, rhs.last);
    streambuf::swap(static_cast<streambuf&>(rhs));
}


auto filter_streambuf::underflow() -> int_type
{
    if (!(mode & ios_base::in)) {
        return traits_type::eof();
    }

    streamsize read, converted;

    if (filebuf) {
        if (first == nullptr) {
            read = filebuf->sgetn(in_buffer, buffer_size);
            first = in_buffer;
            last = in_buffer + read;
        }

        // perform the callback
        converted = do_callback();
        if (!converted) {
            return traits_type::eof();
        }
        setg(out_buffer, out_buffer, out_buffer + converted);

        return traits_type::to_int_type(*gptr());
    }
    return traits_type::eof();
}


void filter_streambuf::set_pointers()
{
    if (mode & ios_base::in) {
        setg(0, 0, 0);
        setp(out_buffer, out_buffer + buffer_size);
    } else {
        setg(in_buffer, in_buffer, in_buffer + buffer_size);
        setp(0, 0);
    }
}


streamsize filter_streambuf::do_callback()
{
    size_t dist;
    streamsize processed, converted;

    // prep arguments
    dist = distance(first, last);
    const void* src = (const void*) first;
    void* dst = (void*) out_buffer;

    // do callback
    callback(src, dist, dst, buffer_size, sizeof(char_type));

    // get callback data
    processed = distance(first, (char*)src);
    converted = distance(out_buffer, (char*)dst);

    // store state
    if (processed < dist) {
        // overflow in bytes written to dst, store state
        first += processed;
    } else {
        // fully converted
        first = nullptr;
        last = nullptr;
    }

    return converted;
}


auto filter_streambuf::overflow(int_type c) -> int_type
{
    if (!(mode & ios_base::out)) {
        return traits_type::eof();
    }

    if (filebuf) {
        if (first == nullptr) {
            first = in_buffer;
            last = in_buffer;
        } else if (last == first + buffer_size) {
            streamsize converted = do_callback();
            filebuf->sputn(out_buffer, converted);
        }

        if (!traits_type::eq_int_type(c, traits_type::eof())) {
            *last++ = traits_type::to_char_type(c);
        }
    }
    return traits_type::not_eof(c);
}


int filter_streambuf::sync()
{
    auto result = overflow(traits_type::eof());

    // flush buffer on output
    if (filebuf && mode & ios_base::out) {
        streamsize converted = do_callback();
        filebuf->sputn(out_buffer, converted);
        filebuf->pubsync();
    }

    if (traits_type::eq_int_type(result, traits_type::eof())) {
        return -1;
    }
    return 0;
}


void filter_streambuf::set_filebuf(streambuf* f)
{
    filebuf = f;
}


void filter_streambuf::set_callback(filter_callback c)
{
    callback = c ? c : null_callback;
}


// ISTREAM


filter_istream::filter_istream(filter_callback c):
    buffer(ios_base::in, nullptr, c),
    istream(&buffer)
{}


filter_istream::~filter_istream()
{}


filter_istream::filter_istream(istream& stream, filter_callback c):
    buffer(ios_base::in, nullptr, c),
    istream(&buffer)
{
    open(stream, c);
}


void filter_istream::open(istream& stream, filter_callback c)
{
    this->stream = &stream;
    buffer.set_filebuf(stream.rdbuf());
    buffer.set_callback(c);
}


filter_istream::filter_istream(filter_istream&& rhs):
    filter_istream()
{
    swap(rhs);
}


filter_istream & filter_istream::operator=(filter_istream&& rhs)
{
    swap(rhs);
    return *this;
}


filter_streambuf* filter_istream::rdbuf() const
{
    return const_cast<filter_streambuf*>(&buffer);
}


void filter_istream::rdbuf(filter_streambuf *buffer)
{
    ios::rdbuf(buffer);
}


void filter_istream::close()
{
    buffer.close();
}


void filter_istream::swap(filter_istream &rhs)
{
    using PYCPP_NAMESPACE::swap;

    // swap
    istream::swap(rhs);
    swap(buffer, rhs.buffer);
    swap(stream, rhs.stream);
}

// OSTREAM

filter_ostream::filter_ostream(filter_callback c):
    buffer(ios_base::out, nullptr, c),
    ostream(&buffer)
{}


filter_ostream::~filter_ostream()
{}


filter_ostream::filter_ostream(ostream& stream, filter_callback c):
    buffer(ios_base::out, nullptr, c),
    ostream(&buffer)
{
    open(stream, c);
}


void filter_ostream::open(ostream& stream, filter_callback c)
{
    this->stream = &stream;
    buffer.set_filebuf(stream.rdbuf());
    buffer.set_callback(c);
}


filter_ostream::filter_ostream(filter_ostream&& rhs):
    filter_ostream()
{
    swap(rhs);
}


filter_ostream & filter_ostream::operator=(filter_ostream&& rhs)
{
    swap(rhs);
    return *this;
}


filter_streambuf* filter_ostream::rdbuf() const
{
    return const_cast<filter_streambuf*>(&buffer);
}


void filter_ostream::rdbuf(filter_streambuf *buffer)
{
    ios::rdbuf(buffer);
}


void filter_ostream::close()
{
    buffer.close();
}


void filter_ostream::swap(filter_ostream &rhs)
{
    using PYCPP_NAMESPACE::swap;

    // swap
    ostream::swap(rhs);
    swap(buffer, rhs.buffer);
    swap(stream, rhs.stream);
}

// IFSTREAM

filter_ifstream::filter_ifstream(filter_callback c):
    filter_istream(c)
{}


filter_ifstream::~filter_ifstream()
{
    close();
}


filter_ifstream::filter_ifstream(filter_ifstream &&rhs):
    filter_ifstream()
{
    swap(rhs);
}


filter_ifstream & filter_ifstream::operator=(filter_ifstream &&rhs)
{
    swap(rhs);
    return *this;
}


filter_ifstream::filter_ifstream(const string_view& name, ios_base::openmode mode, filter_callback c):
    filter_ifstream(c)
{
    open(name, mode, c);
}


void filter_ifstream::open(const string_view& name, ios_base::openmode mode, filter_callback c)
{
    file.open(name, mode);
    filter_istream::open(file, c);
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

filter_ifstream::filter_ifstream(const wstring_view& name, ios_base::openmode mode, filter_callback c):
    filter_ifstream(c)
{
    open(name, mode, c);
}

void filter_ifstream::open(const wstring_view& name, ios_base::openmode mode, filter_callback c)
{
    file.open(name, mode);
    filter_istream::open(file, c);
}


filter_ifstream::filter_ifstream(const u16string_view& name, ios_base::openmode mode, filter_callback c):
    filter_ifstream(c)
{
    open(name, mode, c);
}

void filter_ifstream::open(const u16string_view& name, ios_base::openmode mode, filter_callback c)
{
    file.open(name, mode);
    filter_istream::open(file, c);
}

#endif                                          // WINDOWS


bool filter_ifstream::is_open() const
{
    return file.is_open();
}


void filter_ifstream::close()
{
    filter_istream::close();
    file.close();
}


void filter_ifstream::swap(filter_ifstream &rhs)
{
    // swap the underlying files
    file.swap(rhs.file);

    // update the underlying file buffers, since these might have changed
    rdbuf()->set_filebuf(rhs.file.rdbuf());
    rhs.rdbuf()->set_filebuf(file.rdbuf());

    // swap the underlying streams
    filter_istream::swap(rhs);
}

// OFSTREAM

filter_ofstream::filter_ofstream(filter_callback c):
    filter_ostream(c)
{}


filter_ofstream::~filter_ofstream()
{
    close();
}


filter_ofstream::filter_ofstream(filter_ofstream &&rhs):
    filter_ofstream()
{
    swap(rhs);
}


filter_ofstream & filter_ofstream::operator=(filter_ofstream &&rhs)
{
    swap(rhs);
    return *this;
}


filter_ofstream::filter_ofstream(const string_view& name, ios_base::openmode mode, filter_callback c):
    filter_ofstream(c)
{
    open(name, mode, c);
}

void filter_ofstream::open(const string_view& name, ios_base::openmode mode, filter_callback c)
{
    file.open(name, mode);
    filter_ostream::open(file, c);
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

filter_ofstream::filter_ofstream(const wstring_view& name, ios_base::openmode mode, filter_callback c):
    filter_ofstream(c)
{
    open(name, mode, c);
}

void filter_ofstream::open(const wstring_view& name, ios_base::openmode mode, filter_callback c)
{
    file.open(name, mode);
    filter_ostream::open(file, c);
}


filter_ofstream::filter_ofstream(const u16string_view& name, ios_base::openmode mode, filter_callback c):
    filter_ofstream(c)
{
    open(name, mode, c);
}

void filter_ofstream::open(const u16string_view& name, ios_base::openmode mode, filter_callback c)
{
    file.open(name, mode);
    filter_ostream::open(file, c);
}

#endif                                          // WINDOWS


bool filter_ofstream::is_open() const
{
    return file.is_open();
}


void filter_ofstream::close()
{
    filter_ostream::close();
    file.close();
}


void filter_ofstream::swap(filter_ofstream &rhs)
{
    // swap the underlying files
    file.swap(rhs.file);

    // update the underlying file buffers, since these might have changed
    rdbuf()->set_filebuf(rhs.file.rdbuf());
    rhs.rdbuf()->set_filebuf(file.rdbuf());

    // swap the underlying streams
    filter_ostream::swap(rhs);
}

PYCPP_END_NAMESPACE
