//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Private core module for encoding and decoding routines.
 */

#pragma once

#include <pycpp/compression/exception.h>
#include <pycpp/misc/safe_stdlib.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/string/string.h>
#include <cstdlib>

PYCPP_BEGIN_NAMESPACE

// MACROS
// ------

#define CHECK(EX) (void)((EX) >= 0 || (throw std::runtime_error(#EX), 0))

// CONSTANTS
// ---------

static constexpr int BUFFER_SIZE = 8092;

// DECLARATION
// -----------

/**
 *  \brief Implied base class for a compressor/decompressor.
 */
template <typename Stream>
struct filter_impl
{
    using in_type = typename remove_reference<decltype(std::declval<Stream>().next_in)>::type;
    using out_type = typename remove_reference<decltype(std::declval<Stream>().next_out)>::type;

    int status;
    Stream stream;

    filter_impl();

    void before(void* dst, size_t dstlen);
    void before(const void* src, size_t srclen, void* dst, size_t dstlen);
    void after(void*& dst);
    void after(const void*& src, void*& dst);

    virtual void call() = 0;
    compression_status check_status(const void* src, void* dst, int stream_end) const;
    template <typename Cb> bool flush(void*& dst, size_t dstlen, Cb cb);
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen, int stream_end);
};


template <typename S>
filter_impl<S>::filter_impl()
{
    stream.avail_in = 0;
    stream.next_in = nullptr;
    stream.avail_out = 0;
    stream.next_out = nullptr;
}


template <typename S>
void filter_impl<S>::before(void* dst, size_t dstlen)
{
    stream.next_in = nullptr;
    stream.avail_in = 0;
    stream.next_out = (out_type) dst;
    stream.avail_out = dstlen;
}


template <typename S>
void filter_impl<S>::before(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    stream.next_in = (in_type) src;
    stream.avail_in = srclen;
    stream.next_out = (out_type) dst;
    stream.avail_out = dstlen;
}


template <typename S>
void filter_impl<S>::after(void*& dst)
{
    dst = stream.next_out;
}


template <typename S>
void filter_impl<S>::after(const void*& src, void*& dst)
{
    src = stream.next_in;
    dst = stream.next_out;
}


template <typename S>
compression_status filter_impl<S>::check_status(const void* src, void* dst, int stream_end) const
{
    if (status == stream_end) {
        return compression_eof;
    } else if (stream.next_out == dst) {
        return compression_need_input;
    } else if (stream.next_in == src) {
        return compression_need_output;
    }
    return compression_ok;
}


template <typename S>
template <typename Cb>
bool filter_impl<S>::flush(void*& dst, size_t dstlen, Cb cb)
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
compression_status filter_impl<S>::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen, int stream_end)
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

    call();

    compression_status code = check_status(src, dst, stream_end);
    if (use_src) {
        after(src, dst);
    } else {
        after(dst);
    }

    return code;
}

// FUNCTIONS
// ---------


template <typename Ctx>
std::string ctx_decompress(const string_wrapper& str)
{
    // configurations
    size_t dstlen = BUFFER_SIZE;
    size_t srclen = str.size();
    size_t dst_pos = 0;
    size_t src_pos = 0;
    char* buffer = (char*) safe_malloc(dstlen);
    void* dst = (void*) buffer;
    const void* src = (const void*) str.data();

    // initialize our decompression
    compression_status status = compression_ok;
    try {
        Ctx ctx;
        while (status != compression_eof) {
            dstlen *= 2;
            buffer = (char*) safe_realloc(buffer, dstlen);
            dst = (void*) (buffer + dst_pos);
            status = ctx.decompress(src, srclen - src_pos, dst, dstlen - dst_pos);
            dst_pos = std::distance(buffer, (char*) dst);
            src_pos = std::distance(str.data(), (const char*) src);
        }

        // flush remaining buffer
        ctx.flush(dst, dstlen - dst_pos);
        dst_pos = std::distance(buffer, (char*) dst);

    } catch (...) {
        safe_free(dst);
        throw;
    }

    // create our output string
    size_t out = std::distance(buffer, (char*) dst);
    std::string output(buffer, out);
    safe_free(buffer);

    return output;
}


template <typename Function>
std::string compress_bound(const string_wrapper& str, size_t dstlen, Function function)
{
    const char* src = str.data();
    char *dst = (char*) safe_malloc(dstlen);
    const void* src_first = (const void*) src;
    void* dst_first = (void*) dst;

    try {
        function(src_first, str.size(), dst_first, dstlen);
    } catch (std::exception&) {
        safe_free(dst);
        throw;
    }
    size_t length = std::distance(dst, (char*) dst_first);
    std::string output(dst, length);
    safe_free(dst);

    return output;
}


template <typename Function>
std::string decompress_bound(const string_wrapper& str, size_t bound, Function function)
{
    const char* src = str.data();
    char *dst = (char*) safe_malloc(bound);
    const void* src_first = (const void*) src;
    void* dst_first = (void*) dst;

    try {
        function(src_first, str.size(), dst_first, bound, bound);
    } catch (std::exception&) {
        safe_free(dst);
        throw;
    }
    size_t length = std::distance(dst, (char*) dst_first);
    std::string output(dst, length);
    safe_free(dst);

    return output;
}


PYCPP_END_NAMESPACE
