//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Generic hash structure.
 */

#include <hashlib.h>

// FUNCTIONS
// ---------


/**
 *  \brief Allocate storage for hash.
 */
struct allocate_hash
{
    template <typename Hash>
    void operator()(Hash& hash)
    {
        new (&hash) Hash();
    }
};


/**
 *  \brief Update hash from storage.
 */
struct update_hash
{
    string_view str;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        hash.update(str);
    }
};


/**
 *  \brief Get digest via C-string.
 */
struct digest_cstring
{
    void* dst;
    size_t dstlen;
    size_t length;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        length = hash.digest(dst, dstlen);
    }
};


/**
 *  \brief Get hex digest via C-string.
 */
struct hexdigest_cstring
{
    void* dst;
    size_t dstlen;
    size_t length;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        length = hash.hexdigest(dst, dstlen);
    }
};


/**
 *  \brief Get digest via STL.
 */
struct digest_stl
{
    secure_string str;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        str = hash.digest();
    }
};


/**
 *  \brief Get hex digest via STL.
 */
struct hexdigest_stl
{
    secure_string str;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        str = hash.hexdigest();
    }
};


/**
 *  \brief Cast storage to the correct type.
 */
template <typename Memory, typename Function>
static void get_hash(Memory& mem, hash_algorithm algorithm, Function function)
{
    switch (algorithm) {
        case md2_hash_algorithm:
            function(reinterpret_cast<md2_hash&>(mem));
            break;

        case md4_hash_algorithm:
            function(reinterpret_cast<md4_hash&>(mem));
            break;

        case md5_hash_algorithm:
            function(reinterpret_cast<md5_hash&>(mem));
            break;

        case sha1_hash_algorithm:
            function(reinterpret_cast<sha1_hash&>(mem));
            break;

        case sha2_224_hash_algorithm:
            function(reinterpret_cast<sha2_224_hash&>(mem));
            break;

        case sha2_256_hash_algorithm:
            function(reinterpret_cast<sha2_256_hash&>(mem));
            break;

        case sha2_384_hash_algorithm:
            function(reinterpret_cast<sha2_384_hash&>(mem));
            break;

        case sha2_512_hash_algorithm:
            function(reinterpret_cast<sha2_512_hash&>(mem));
            break;

        // TODO: need to implement the SHA3 functions.
        case sha3_224_hash_algorithm:
            break;

        case sha3_256_hash_algorithm:
            break;

        case sha3_384_hash_algorithm:
            break;

        case sha3_512_hash_algorithm:
            break;

        case whirlpool_hash_algorithm:
            function(reinterpret_cast<whirlpool_hash&>(mem));
            break;

        default:
            throw std::runtime_error("Unrecognized hashing algorithm.");
    }
}

// OBJECTS
// -------


hash::hash(hash_algorithm algorithm):
    algorithm(algorithm)
{
    get_hash(mem, algorithm, allocate_hash());
}


hash::hash(hash_algorithm algorithm, const void* src, size_t srclen):
    algorithm(algorithm)
{
    get_hash(mem, algorithm, allocate_hash());
    update(src, srclen);
}


hash::hash(hash_algorithm algorithm, const string_view& str):
    algorithm(algorithm)
{
    get_hash(mem, algorithm, allocate_hash());
    update(str);
}


hash::hash(const hash& other):
    algorithm(other.algorithm),
    mem(other.mem)
{}


hash& hash::operator=(const hash& other)
{
    algorithm = other.algorithm;
    mem = other.mem;
    return *this;
}


hash::hash(hash&& other):
    algorithm(std::move(other.algorithm)),
    mem(std::move(other.mem))
{}


hash& hash::operator=(hash&& other)
{
    algorithm = std::move(other.algorithm);
    mem = std::move(other.mem);
    return *this;
}


void hash::update(const void* src, size_t srclen)
{
    update(string_view(reinterpret_cast<const char*>(src), srclen));
}


void hash::update(const string_view& str)
{
    update_hash functor = {str};
    get_hash(mem, algorithm, functor);
}


size_t hash::digest(void* dst, size_t dstlen) const
{
    digest_cstring functor = {dst, dstlen};
    get_hash(const_cast<memory_type&>(mem), algorithm, functor);
    return functor.length;
}


size_t hash::hexdigest(void* dst, size_t dstlen) const
{
    hexdigest_cstring functor = {dst, dstlen};
    get_hash(const_cast<memory_type&>(mem), algorithm, functor);
    return functor.length;
}


secure_string hash::digest() const
{
    digest_stl functor;
    get_hash(const_cast<memory_type&>(mem), algorithm, functor);
    return functor.str;
}


secure_string hash::hexdigest() const
{
    hexdigest_stl functor;
    get_hash(const_cast<memory_type&>(mem), algorithm, functor);
    return functor.str;
}
