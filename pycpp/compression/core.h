//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Private core module for encoding and decoding routines.
 */

#pragma once

#include <pycpp/compression/exception.h>
#include <cstdlib>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// CONSTANTS
// ---------

static constexpr int BUFFER_SIZE = 4096;

// DECLARATION
// -----------

/**
 *  \brief Implied base class for a compressor.
 */
template <typename Stream>
struct compressor_impl
{
    using char_type = typename std::remove_reference<decltype(std::declval<Stream>().next_in)>::type;

    int status;
    Stream stream;

    compressor_impl();

    void before(void* dst, size_t dstlen);
    void before(const void* src, size_t srclen, void* dst, size_t dstlen);
    void after(void*& dst);
    void after(const void*& src, void*& dst);
    compression_status check_status(const void* src, void* dst, int stream_end) const;

    template <typename CallBack>
    bool flush(void*& dst, size_t dstlen, CallBack cb);

    virtual void compress() = 0;
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen, int stream_end);
};

// DEFINITION
// ----------

template <typename S>
compressor_impl<S>::compressor_impl()
{
    stream.avail_in = 0;
    stream.next_in = nullptr;
    stream.avail_out = 0;
    stream.next_out = nullptr;
}


template <typename S>
void compressor_impl<S>::before(void* dst, size_t dstlen)
{
    stream.next_out = (char_type) dst;
    stream.avail_out = dstlen;
}


template <typename S>
void compressor_impl<S>::before(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    stream.next_in = (char_type) src;
    stream.avail_in = srclen;
    stream.next_out = (char_type) dst;
    stream.avail_out = dstlen;
}


template <typename S>
void compressor_impl<S>::after(void*& dst)
{
    dst = stream.next_out;
}


template <typename S>
void compressor_impl<S>::after(const void*& src, void*& dst)
{
    src = stream.next_in;
    dst = stream.next_out;
}


template <typename S>
compression_status compressor_impl<S>::check_status(const void* src, void* dst, int stream_end) const
{
    if (status == stream_end) {
        return compression_eof;
    } else if (stream.next_out == dst) {
        return compression_need_input;
    } else if (stream.next_in == src) {
        return compression_need_output;
    }
}


template <typename S>
template <typename CallBack>
bool compressor_impl<S>::flush(void*& dst, size_t dstlen, CallBack cb)
{
    if (dst == nullptr) {
        return false;
    }
    before(dst, dstlen);
    bool code = cb();
    after(dst);

    return code;
}


template <typename S>
compression_status compressor_impl<S>::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen, int stream_end)
{
    // no input data, or already reached stream end
    if (status == stream_end) {
        return compression_eof;
    } else if (srclen == 0 && stream.avail_in == 0) {
        return compression_need_input;
    } else if (dst == nullptr || dstlen == 0) {
        return compression_need_output;
    }

    bool use_src = (stream.next_in == nullptr || stream.avail_in == 0);
    if (use_src) {
        before(src, srclen, dst, dstlen);
    } else {
        // have remaining input data
        before(dst, dstlen);
    }

    compress();

    compression_status code = check_status(src, dst, stream_end);
    if (use_src) {
        after(src, dst);
    } else {
        after(dst);
    }

    return code;
}

PYCPP_END_NAMESPACE
