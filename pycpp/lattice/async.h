//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Asynchronous requests.
 */

#pragma once

#include <pycpp/lattice/request.h>
#include <pycpp/lattice/response.h>
#include <chrono>
#include <deque>
#include <future>
#include <thread>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

typedef std::deque<response_t> response_list_t;


/**
 *  \brief Thread pool for asynchronous requests.
 *
 *  \warning The asynchronous approach is rudimentary, with a
 *  single thread per socket. This is not intended to replace
 *  a true, asynchronous library like Boost::asio or Casablanca.
 */
class pool_t
{
public:
    pool_t() = default;
    pool_t(const pool_t &other) = delete;
    pool_t & operator=(const pool_t&) = delete;
    pool_t(pool_t&&) = default;
    pool_t & operator=(pool_t&&) = default;

    template <typename... Ts> void get(Ts&&... ts);
    template <typename... Ts> void head(Ts&&... ts);
    template <typename... Ts> void options(Ts&&... ts);
    template <typename... Ts> void patch(Ts&&... ts);
    template <typename... Ts> void post(Ts&&... ts);
    template <typename... Ts> void put(Ts&&... ts);
    template <typename... Ts> void trace(Ts&&... ts);

    response_list_t perform();

    template <typename Duration>
    typename std::enable_if<std::is_integral<Duration>::value, response_t>::type
    next(Duration seconds = 1);

    template <typename Duration>
    typename std::enable_if<is_specialization<Duration, std::chrono::duration>::value, response_t>::type
    next(const Duration& duration = std::chrono::seconds(1));

    explicit operator bool() const;

protected:
    std::deque<std::future<response_t>> futures;
};


// IMPLEMENTATION
// --------------


/**
 *  \brief Initialize thread with GET request.
 */
template <typename... Ts>
void pool_t::get(Ts&&... ts)
{
    request_t request;
    set_option(request, std::forward<Ts>(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](request_t &&request) {
        request.set_method(GET);
        return request.exec();
    }, std::move(request)));
}


/**
 *  \brief Initialize thread with HEAD request.
 */
template <typename... Ts>
void pool_t::head(Ts&&... ts)
{
    request_t request;
    set_option(request, std::forward<Ts>(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](request_t &&request) {
        request.set_method(HEAD);
        return request.exec();
    }, std::move(request)));
}


/**
 *  \brief Initialize thread with OPTIONS request.
 */
template <typename... Ts>
void pool_t::options(Ts&&... ts)
{
    request_t request;
    set_option(request, std::forward<Ts>(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](request_t &&request) {
        request.set_method(OPTIONS);
        return request.exec();
    }, std::move(request)));
}


/**
 *  \brief Initialize thread with PATCH request.
 */
template <typename... Ts>
void pool_t::patch(Ts&&... ts)
{
    request_t request;
    set_option(request, std::forward<Ts>(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](request_t &&request) {
        request.set_method(PATCH);
        return request.exec();
    }, std::move(request)));
}


/**
 *  \brief Initialize thread with POST request.
 */
template <typename... Ts>
void pool_t::post(Ts&&... ts)
{
    request_t request;
    set_option(request, std::forward<Ts>(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](request_t &&request) {
        request.set_method(POST);
        return request.exec();
    }, std::move(request)));
}


/**
 *  \brief Initialize thread with PUT request.
 */
template <typename... Ts>
void pool_t::put(Ts&&... ts)
{
    request_t request;
    set_option(request, std::forward<Ts>(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](request_t &&request) {
        request.set_method(PUT);
        return request.exec();
    }, std::move(request)));
}


/**
 *  \brief Initialize thread with TRACE request.
 */
template <typename... Ts>
void pool_t::trace(Ts&&... ts)
{
    request_t request;
    set_option(request, std::forward<Ts>(ts)...);

    futures.emplace_back(std::async(std::launch::async, [](request_t &&request) {
        request.set_method(TRACE);
        return request.exec();
    }, std::move(request)));
}


/**
 *  \brief Block until next query is ready (using a seconds overload).
 */
template <typename Duration>
typename std::enable_if<std::is_integral<Duration>::value, response_t>::type
pool_t::next(Duration seconds)
{
    return next(std::chrono::seconds(seconds));
}


/**
 *  \brief Block until next query is ready.
 */
template <typename Duration>
typename std::enable_if<is_specialization<Duration, std::chrono::duration>::value, response_t>::type
pool_t::next(const Duration& duration)
{
    // set our starting timepoint
    typedef std::chrono::high_resolution_clock highres_clock;

    auto now = highres_clock::now();
    auto stop = now + duration;

    auto it = futures.begin();
    while (!futures.empty() && now < stop) {
        auto status = it->wait_for(std::chrono::milliseconds(50));
        if (status == std::future_status::ready) {
            auto response = it->get();
            futures.erase(it);
            return response;
        }

        // increment our conditions
        now = highres_clock::now();
        ++it;
        if (it == futures.end()) {
            it = futures.begin();
        }
    }

    return response_t();
}

PYCPP_END_NAMESPACE
