//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/lattice/async.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------


response_list_t pool_t::perform()
{
    response_list_t list;
    for (auto &&future: futures) {
        if (future.valid()) {
            list.emplace_back(future.get());
        }
    }

    return list;
}


pool_t::operator bool() const
{
    return !futures.empty();
}

PYCPP_END_NAMESPACE
