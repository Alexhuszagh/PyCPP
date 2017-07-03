//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <json/dom.h>
#include <json/object.h>


// OBJECTS
// -------


json_object_t_impl::~json_object_t_impl()
{
    for (auto& pair: map_) {
        delete pair.second;
    }
}


auto json_object_t_impl::size() const -> size_type
{
    return map_.size();
}


auto json_object_t_impl::max_size() const -> size_type
{
    return map_.max_size();
}


bool json_object_t_impl::empty() const
{
    return map_.empty();
}


auto json_object_t_impl::at(const key_type& key) -> mapped_type&
{
    return *map_.at(key);
}


auto json_object_t_impl::at(const key_type& key) const -> const mapped_type&
{
    return *map_.at(key);
}

