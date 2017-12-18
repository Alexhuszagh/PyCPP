//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#if defined(HAVE_LZMA)

#include <pycpp/compression/core.h>
#include <pycpp/compression/lzma.h>
#include <lzma.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

static size_t lzma_compress_bound(size_t size)
{
    return lzma_stream_buffer_bound(size);
}


void check_xzstatus(int error)
{
    switch (error) {
        case LZMA_OK:
        case LZMA_STREAM_END:
        case LZMA_NO_CHECK:
        case LZMA_UNSUPPORTED_CHECK:
        case LZMA_GET_CHECK:
        case LZMA_BUF_ERROR:
            return;
        case LZMA_MEM_ERROR:
        case LZMA_MEMLIMIT_ERROR:
            throw compression_error(compression_out_of_memory);
        case LZMA_FORMAT_ERROR:
        case LZMA_DATA_ERROR:
            throw compression_error(compression_data_error);
        case LZMA_OPTIONS_ERROR:
            throw compression_error(compression_invalid_parameter);
        case LZMA_PROG_ERROR:
            throw compression_error(compression_internal_error);
        default:
            throw compression_error(compression_unexpected_error);
    }
}


// OBJECTS
// -------

/**
 *  \brief Implied base class for the LZMA2 compressor.
 */
struct lzma_compressor_impl: filter_impl<lzma_stream>
{
    using base = filter_impl<lzma_stream>;
    lzma_compressor_impl(int level = LZMA_PRESET_DEFAULT);
    ~lzma_compressor_impl();

    virtual void call();
    bool flush(void*& dst, size_t dstlen);
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};



lzma_compressor_impl::lzma_compressor_impl(int level)
{
    stream = LZMA_STREAM_INIT;
    status = LZMA_OK;
    CHECK(lzma_easy_encoder(&stream, level, LZMA_CHECK_CRC64));
}


lzma_compressor_impl::~lzma_compressor_impl()
{
    lzma_end(&stream);
}


void lzma_compressor_impl::call()
{
    while (stream.avail_in && stream.avail_out && status != LZMA_STREAM_END) {
        status = lzma_code(&stream, LZMA_RUN);
        check_xzstatus(status);
    }
}


bool lzma_compressor_impl::flush(void*& dst, size_t dstlen)
{
    return base::flush(dst, dstlen, [&]()
    {
        if (dstlen) {
            status = lzma_code(&stream, LZMA_FINISH);
            return status == LZMA_STREAM_END || status == LZMA_OK;
        } else {
            status = lzma_code(&stream, LZMA_FULL_FLUSH);
            return status == LZMA_STREAM_END || status == LZMA_OK;
        }
    });
}


compression_status lzma_compressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return base::operator()(src, srclen, dst, dstlen, LZMA_STREAM_END);
}


/**
 *  \brief Implied base class for the LZMA2 decompressor.
 */
struct lzma_decompressor_impl: filter_impl<lzma_stream>
{
    using base = filter_impl<lzma_stream>;
    static const uint64_t memlimit = UINT64_MAX;
    static const uint32_t flags = LZMA_TELL_ANY_CHECK | LZMA_TELL_NO_CHECK;

    lzma_decompressor_impl();
    ~lzma_decompressor_impl();

    virtual void call();
    bool flush(void*& dst, size_t dstlen);
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


lzma_decompressor_impl::lzma_decompressor_impl()
{
    stream = LZMA_STREAM_INIT;
    status = LZMA_OK;
    CHECK(lzma_stream_decoder(&stream, memlimit, flags));
}


lzma_decompressor_impl::~lzma_decompressor_impl()
{
    lzma_end(&stream);
}


void lzma_decompressor_impl::call()
{
    while (stream.avail_in && stream.avail_out && status != LZMA_STREAM_END) {
        status = lzma_code(&stream, LZMA_RUN);
        check_xzstatus(status);
    }
}


bool lzma_decompressor_impl::flush(void*& dst, size_t dstlen)
{
    return base::flush(dst, dstlen, [&]()
    {
        if (dstlen) {
            status = lzma_code(&stream, LZMA_FINISH);
            return status == LZMA_STREAM_END || status == LZMA_OK;
        } else {
            status = lzma_code(&stream, LZMA_FULL_FLUSH);
            return status == LZMA_STREAM_END || status == LZMA_OK;
        }
    });
}


compression_status lzma_decompressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return base::operator()(src, srclen, dst, dstlen, LZMA_STREAM_END);
}


lzma_compressor::lzma_compressor(int level):
    ptr_(make_unique<lzma_compressor_impl>(level))
{}


lzma_compressor::lzma_compressor(lzma_compressor&& rhs):
    ptr_(move(rhs.ptr_))
{}


lzma_compressor & lzma_compressor::operator=(lzma_compressor&& rhs)
{
    swap(rhs);
    return *this;
}


lzma_compressor::~lzma_compressor()
{}


compression_status lzma_compressor::compress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return (*ptr_)(src, srclen, dst, dstlen);
}


bool lzma_compressor::flush(void*& dst, size_t dstlen)
{
    return ptr_->flush(dst, dstlen);
}


void lzma_compressor::close()
{
    ptr_.reset();
}


void lzma_compressor::swap(lzma_compressor& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(ptr_, rhs.ptr_);
}


lzma_decompressor::lzma_decompressor():
    ptr_(make_unique<lzma_decompressor_impl>())
{}


lzma_decompressor::lzma_decompressor(lzma_decompressor&& rhs):
    ptr_(move(rhs.ptr_))
{}


lzma_decompressor & lzma_decompressor::operator=(lzma_decompressor&& rhs)
{
    swap(rhs);
    return *this;
}


lzma_decompressor::~lzma_decompressor()
{}


compression_status lzma_decompressor::decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return (*ptr_)(src, srclen, dst, dstlen);
}


bool lzma_decompressor::flush(void*& dst, size_t dstlen)
{
    return ptr_->flush(dst, dstlen);
}


void lzma_decompressor::close()
{
    ptr_.reset();
}


void lzma_decompressor::swap(lzma_decompressor& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(ptr_, rhs.ptr_);
}

// FUNCTIONS
// ---------


void lzma_compress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    static uint32_t level = LZMA_PRESET_DEFAULT;
    static lzma_check check = LZMA_CHECK_CRC64;
    size_t dstpos = 0;
    if (srclen) {
        CHECK(lzma_easy_buffer_encode(level, check, nullptr, (const uint8_t*) src, srclen, (uint8_t*) dst, &dstpos, dstlen));
    } else {
        // compression no bytes
        char c = 0;
        CHECK(lzma_easy_buffer_encode(level, check, nullptr, (const uint8_t*) &c, 0, (uint8_t*) dst, &dstpos, dstlen));
    }

    // update pointers
    src = ((char*) src) + srclen;
    dst = ((char*) dst) + dstpos;
}


std::string lzma_compress(const string_wrapper& str)
{
    size_t dstlen = lzma_compress_bound(str.size());
    return compress_bound(str, dstlen, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        lzma_compress(src, srclen, dst, dstlen);
    });
}


std::string lzma_decompress(const string_wrapper& str)
{
    return ctx_decompress<lzma_decompressor>(str);
}


void lzma_decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t bound)
{
    static uint64_t memlimit = UINT64_MAX;
    size_t srcpos = 0;
    size_t dstpos = 0;
    if (srclen) {
        CHECK(lzma_stream_buffer_decode(&memlimit, 0, nullptr, (const uint8_t*) src, &srcpos, srclen, (uint8_t*) dst, &dstpos, dstlen));
    } else {
        // compression no bytes
        char c = 0;
        CHECK(lzma_stream_buffer_decode(&memlimit, 0, nullptr, (const uint8_t*) &c, &srcpos, 0, (uint8_t*) dst, &dstpos, dstlen));
    }

    // update pointers
    src = ((char*) src) + srcpos;
    dst = ((char*) dst) + dstpos;
}


std::string lzma_decompress(const string_wrapper& str, size_t bound)
{
    return decompress_bound(str, bound, [](const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t bound) {
        lzma_decompress(src, srclen, dst, dstlen, bound);
    });
}

PYCPP_END_NAMESPACE

#endif                  // HAVE_LZMA
