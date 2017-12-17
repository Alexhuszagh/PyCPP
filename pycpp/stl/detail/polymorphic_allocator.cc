//  :copyright: (c) 2012 Pablo Halpern.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.

#include <pycpp/stl/detail/polymorphic_allocator.h>
#if !defined(HAVE_CPP17)
#   include <limits>
#endif


PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

bool operator==(const memory_resource& lhs, const memory_resource& rhs)
{
    return &lhs == &rhs || lhs.is_equal(rhs);
}


bool operator!=(const memory_resource& lhs, const memory_resource& rhs)
{
    return !(lhs == rhs);
}


#if defined(HAVE_CPP17)             // HAVE_CPP17

// FUNCTIONS
// ---------


memory_resource* new_delete_resource() noexcept
{
    return std::pmr::new_delete_resource();
}


memory_resource* null_memory_resource() noexcept
{
    return std::pmr::null_memory_resource();
}


memory_resource* get_default_resource() noexcept
{
    return std::pmr::get_default_resource();
}


memory_resource* set_default_resource(memory_resource* r) noexcept
{
    return std::pmr::set_default_resource(r);
}

#else                               // !HAVE_CPP17

namespace polymorphic_detail
{
// FUNCTIONS
// ---------

size_t aligned_allocation_size(size_t n, size_t alignment) noexcept
{
    return (n + alignment - 1) & ~(alignment - 1);
}


size_t max_size(size_t align)
{
    return std::numeric_limits<size_t>::max() - align;
}

// DECLARATIONS
// ------------


struct new_delete_resource: public memory_resource
{
public:
    ~new_delete_resource() = default;

protected:
    virtual void* do_allocate(size_t, size_t);
    virtual void do_deallocate(void*, size_t, size_t);
    virtual bool do_is_equal(const memory_resource&) const noexcept;
};


struct null_memory_resource: public memory_resource
{
public:
    ~null_memory_resource() = default;

protected:
    virtual void* do_allocate(size_t, size_t);
    virtual void do_deallocate(void*, size_t, size_t);
    virtual bool do_is_equal(const memory_resource&) const noexcept;
};

// IMPLEMENTATION
// --------------

void* new_delete_resource::do_allocate(size_t n, size_t alignment)
{
    return operator new(n);
}


void new_delete_resource::do_deallocate(void* p, size_t n, size_t alignment)
{
    operator delete(p);
}


bool new_delete_resource::do_is_equal(const memory_resource& rhs) const noexcept
{
    return this == &rhs;
}


void* null_memory_resource::do_allocate(size_t, size_t)
{
    throw std::bad_alloc();
}


void null_memory_resource::do_deallocate(void*, size_t, size_t)
{}


bool null_memory_resource::do_is_equal(const memory_resource& rhs) const noexcept
{
    return this == &rhs;
}

}   /* polymorphic_detail */

// FUNCTIONS
// ---------

std::atomic<memory_resource*> memory_resource::default_resource_(nullptr);


memory_resource* new_delete_resource() noexcept
{
    static polymorphic_detail::new_delete_resource singleton;
    return &singleton;
}


memory_resource* null_memory_resource() noexcept
{
    static polymorphic_detail::null_memory_resource singleton;
    return &singleton;
}


memory_resource* get_default_resource() noexcept
{
    memory_resource* r = memory_resource::default_resource_.load();
    if (r == nullptr) {
        r = new_delete_resource();
    }

    return r;
}

memory_resource* set_default_resource(memory_resource* r) noexcept
{
    if (r == nullptr) {
        r = new_delete_resource();
    }

    return memory_resource::default_resource_.exchange(r);
}

// OBJECTS
// -------

memory_resource::~memory_resource()
{}


void* memory_resource::allocate(size_t n, size_t alignment)
{
    return do_allocate(n, alignment);
}


void memory_resource::deallocate(void *p, size_t n, size_t alignment)
{
    do_deallocate(p, n, alignment);
}


bool memory_resource::is_equal(const memory_resource& rhs) const noexcept
{
    return do_is_equal(rhs);
}


#endif                              // HAVE_CPP17

PYCPP_END_NAMESPACE
