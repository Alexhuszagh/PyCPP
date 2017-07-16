//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

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


// TODO: need lzma_decompressor_impl


lzma_compressor::lzma_compressor(int level):
    ptr_(new lzma_compressor_impl(level))
{}


lzma_compressor::lzma_compressor(lzma_compressor&& rhs):
    ptr_(std::move(rhs.ptr_))
{}


lzma_compressor & lzma_compressor::operator=(lzma_compressor&& rhs)
{
    std::swap(ptr_, rhs.ptr_);
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

// TODO: need lzma_decompressor


PYCPP_END_NAMESPACE
