//  :copyright: (c) 2015-2017 Michael Park.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Variant implementation for C++11.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler.h>

#if defined(HAVE_CPP17)             // HAVE_CPP17
#   include <variant>
#else                               // !HAVE_CPP17
#   include <pycpp/collections/utility.h>
#   include <cstddef>
#   include <exception>
#   include <functional>
#   include <initializer_list>
#   include <memory>
#   include <new>
#   include <type_traits>
#   include <utility>
#endif                              // HAVE_CPP17

PYCPP_BEGIN_NAMESPACE

#if defined(HAVE_CPP17)             // HAVE_CPP17

template <typename... Ts> using variant = std::variant<Ts...>;
using monostate = std::monostate;
using bad_variant_access = std::bad_variant_access;
template <size_t I, typename T> using variant_alternative = std::variant_alternative<I, T>;
template <size_t I, typename T> using variant_alternative_t = std::variant_alternative_t<I, T>;

#else                               // !HAVE_CPP17

// FORWARD
// -------

template <typename... Ts>
class variant;

template <typename T>
struct variant_size;

#if defined(HAVE_CPP14)             // HAVE_CPP14

template <typename T>
constexpr size_t variant_size_v = variant_size<T>::value;

#endif                              // HAVE_CPP14

// MACROS
// ------

/**
 *  \brief decltype(auto) for C++11.
 */
#define VARIANT_DECLTYPE_AUTO(...) -> decltype((__VA_ARGS__))           \
    {                                                                   \
        return __VA_ARGS__;                                             \
    }

/**
 *  \brief decay_t<decltype(auto)> for C++11.
 */
#define VARIANT_DECLTYPE_DECAY(...) -> decay_t<decltype((__VA_ARGS__))> \
    {                                                                   \
        return __VA_ARGS__;                                             \
    }

// TODO: remove
#define RETURN(...)                                          \
  noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__) { \
    return __VA_ARGS__;                                      \
  }

namespace detail
{
// DETAIL
// ------

template <typename T>
struct identity
{
    using type = T;
};

template <typename...>
struct voider: identity<void>
{};

template <typename... Ts>
using void_t = typename voider<Ts...>::type;

template <size_t N>
using size_constant = std::integral_constant<size_t, N>;

template <bool B>
using bool_constant = std::integral_constant<bool, B>;

template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template <typename T>
using remove_const_t = typename std::remove_const<T>::type;

template <typename T>
using decay_t = typename std::decay<T>::type;

template <typename... Ts>
using common_type_t = typename std::common_type<Ts...>::type;

template <typename T>
using add_pointer_t = typename std::add_pointer<T>::type;

template <typename T>
using is_trivially_copy_constructible = std::is_trivially_copy_constructible<T>;

template <typename T>
using is_trivially_move_constructible = std::is_trivially_move_constructible<T>;

template <typename T>
using is_trivially_copy_assignable = std::is_trivially_copy_assignable<T>;

template <typename T>
using is_trivially_move_assignable = std::is_trivially_move_assignable<T>;

template <size_t I, typename T>
struct indexed_type: size_constant<I>,
    identity<T>
{};

template <bool... Bs>
using bool_sequence = std::integer_sequence<bool, Bs...>;

template <bool... Bs>
using all_ = std::is_same<bool_sequence<true, Bs...>, bool_sequence<Bs..., true>>;

template <size_t I, typename... Ts>
struct type_pack_element_impl
{
private:
    template <typename> struct set;

    template <size_t... Is>
    struct set<std::index_sequence<Is...>>: indexed_type<Is, Ts>...
    {};

    template <typename T>
    inline static std::enable_if<true, T> impl(indexed_type<I, T>);

    inline static std::enable_if<false> impl(...);

public:
    using type = decltype(impl(set<std::index_sequence_for<Ts...>>{}));
};

template <size_t I, typename... Ts>
using type_pack_element = typename type_pack_element_impl<I, Ts...>::type;

template <size_t I, typename... Ts>
using type_pack_element_t = typename type_pack_element<I, Ts...>::type;

template <typename T, size_t N>
struct array
{
    constexpr const T &operator[](size_t index) const
    {
        return data[index];
    }

    T data[N == 0 ? 1 : N];
};

template <typename T>
struct remove_all_extents: identity<T>
{};

template <typename T, size_t N>
struct remove_all_extents<array<T, N>>: remove_all_extents<T>
{};

template <typename T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

#include <warnings/push.h>
#include <warnings/unused-parameter.h>

template <typename F, typename... As>
inline constexpr auto invoke(F &&f, As &&... as) noexcept
VARIANT_DECLTYPE_AUTO(std::forward<F>(f)(std::forward<As>(as)...))

#include <warnings/pop.h>

template <typename B, typename T, typename D>
inline constexpr auto invoke(T B::*pmv, D &&d) noexcept
VARIANT_DECLTYPE_AUTO(std::forward<D>(d).*pmv)

template <typename Pmv, typename Ptr>
inline constexpr auto invoke(Pmv pmv, Ptr &&ptr) noexcept
VARIANT_DECLTYPE_AUTO((*std::forward<Ptr>(ptr)).*pmv)

template <typename B, typename T, typename D, typename... As>
inline constexpr auto invoke(T B::*pmf, D &&d, As &&... as) noexcept
VARIANT_DECLTYPE_AUTO((std::forward<D>(d).*pmf)(std::forward<As>(as)...))

template <typename Pmf, typename Ptr, typename... As>
inline constexpr auto invoke(Pmf pmf, Ptr &&ptr, As &&... as) noexcept
VARIANT_DECLTYPE_AUTO(((*std::forward<Ptr>(ptr)).*pmf)(std::forward<As>(as)...))

namespace invoker
{
// INVOKER
// -------

template <typename Void, typename, typename...>
struct invoke_result
{};

template <typename F, typename... Args>
struct invoke_result<void_t<decltype(invoke(std::declval<F>(), std::declval<Args>()...))>, F, Args...>:
    identity<decltype(invoke(std::declval<F>(), std::declval<Args>()...))>
{};

}   /* invoker */

template <typename F, typename... Args>
using invoke_result = invoker::invoke_result<void, F, Args...>;

template <typename F, typename... Args>
using invoke_result_t = typename invoke_result<F, Args...>::type;

namespace invocable
{
// INVOCABLE
// ---------

template <typename Void, typename, typename...>
struct is_invocable: std::false_type
{};

template <typename F, typename... Args>
struct is_invocable<void_t<invoke_result_t<F, Args...>>, F, Args...>: std::true_type
{};

template <typename Void, typename, typename, typename...>
struct is_invocable_r: std::false_type {};

template <typename R, typename F, typename... Args>
struct is_invocable_r<void_t<invoke_result_t<F, Args...>>, R, F, Args...>:
    std::is_convertible<invoke_result_t<F, Args...>, R>
{};

}   /* invocable */

template <typename F, typename... Args>
using is_invocable = invocable::is_invocable<void, F, Args...>;

template <typename R, typename F, typename... Args>
using is_invocable_r = invocable::is_invocable_r<void, R, F, Args...>;

template <typename T>
struct is_swappable_impl
{
    private:
    template <typename U, typename = decltype(std::swap(std::declval<U&>(), std::declval<U&>()))>
    inline static std::true_type test(int);

    template <typename U>
    inline static std::false_type test(...);

public:
    using type = decltype(test<T>(0));
};

template <typename T>
using is_swappable = typename is_swappable_impl<T>::type;

template <typename T, bool = is_swappable<T>::value>
struct is_nothrow_swappable
{
    static constexpr bool value = noexcept(std::swap(std::declval<T&>(), std::declval<T&>()));
};

template <typename T>
struct is_nothrow_swappable<T, false>: std::false_type
{};


struct equal_to
{
    template <typename T, typename U>
    inline constexpr bool operator()(T&& lhs, U&& rhs) const
    {
        return std::forward<T>(lhs) == std::forward<U>(rhs);
    }
};


struct not_equal_to
{
    template <typename T, typename U>
    inline constexpr bool operator()(T&& lhs, U&& rhs) const
    {
        return std::forward<T>(lhs) != std::forward<U>(rhs);
    }
};


struct less
{
    template <typename T, typename U>
    inline constexpr bool operator()(T&& lhs, U&& rhs) const
    {
        return std::forward<T>(lhs) < std::forward<U>(rhs);
    }
};


struct greater
{
    template <typename T, typename U>
    inline constexpr bool operator()(T&& lhs, U&& rhs) const
    {
        return std::forward<T>(lhs) > std::forward<U>(rhs);
    }
};


struct less_equal
{
    template <typename T, typename U>
    inline constexpr bool operator()(T&& lhs, U&& rhs) const
    {
        return std::forward<T>(lhs) <= std::forward<U>(rhs);
    }
};


struct greater_equal
{
    template <typename T, typename U>
    inline constexpr bool operator()(T&& lhs, U&& rhs) const
    {
        return std::forward<T>(lhs) >= std::forward<U>(rhs);
    }
};


inline constexpr bool all()
{
    return true;
}

template <typename... Bs>
inline constexpr bool all(bool b, Bs... bs)
{
  return b && all(bs...);
}

constexpr size_t not_found = static_cast<size_t>(-1);
constexpr size_t ambiguous = static_cast<size_t>(-2);

inline constexpr size_t find_index_impl(size_t result, size_t)
{
    return result;
}

template <typename... Bs>
inline constexpr size_t find_index_impl(size_t result, size_t idx, bool b, Bs... bs)
{
    return b ? (result != not_found ? ambiguous : find_index_impl(idx, idx + 1, bs...)) : find_index_impl(result, idx + 1, bs...);
}

template <typename T, typename... Ts>
inline constexpr size_t find_index()
{
    return find_index_impl(not_found, 0, std::is_same<T, Ts>::value...);
}

template <size_t I>
using find_index_sfinae_impl = enable_if_t<I != not_found && I != ambiguous, size_constant<I>>;

template <typename T, typename... Ts>
using find_index_sfinae = find_index_sfinae_impl<find_index<T, Ts...>()>;

template <size_t I>
struct find_index_checked_impl: size_constant<I>
{
    static_assert(I != not_found, "the specified type is not found.");
    static_assert(I != ambiguous, "the specified type is ambiguous.");
};

template <typename T, typename... Ts>
using find_index_checked = find_index_checked_impl<find_index<T, Ts...>()>;

struct valueless_t
{};

enum class Trait
{
    TriviallyAvailable,
    Available,
    Unavailable
};

template <
    typename T,
    template <typename> class IsTriviallyAvailable,
    template <typename> class IsAvailable
>
inline constexpr Trait trait()
{
  return IsTriviallyAvailable<T>::value
             ? Trait::TriviallyAvailable
             : IsAvailable<T>::value ? Trait::Available
                                     : Trait::Unavailable;
}

inline constexpr Trait common_trait_impl(Trait result)
{
    return result;
}

template <typename... Traits>
inline constexpr Trait common_trait_impl(Trait result, Trait t, Traits... ts)
{
    return static_cast<int>(t) > static_cast<int>(result)
        ? common_trait_impl(t, ts...)
        : common_trait_impl(result, ts...);
}

template <typename... Traits>
inline constexpr Trait common_trait(Traits... ts)
{
    return common_trait_impl(Trait::TriviallyAvailable, ts...);
}

template <typename... Ts>
struct traits
{
    static constexpr Trait copy_constructible_trait = common_trait(trait<Ts, is_trivially_copy_constructible, std::is_copy_constructible>()...);

    static constexpr Trait move_constructible_trait = common_trait(trait<Ts, is_trivially_move_constructible, std::is_move_constructible>()...);

    static constexpr Trait copy_assignable_trait = common_trait(copy_constructible_trait, trait<Ts, is_trivially_copy_assignable, std::is_copy_assignable>()...);

    static constexpr Trait move_assignable_trait = common_trait(move_constructible_trait, trait<Ts, is_trivially_move_assignable, std::is_move_assignable>()...);

    static constexpr Trait destructible_trait = common_trait(trait<Ts, std::is_trivially_destructible, std::is_destructible>()...);
};

namespace access
{
// ACCESS
// ------

struct recursive_union
{
    template <size_t I, bool Dummy = true>
    struct get_alt_impl
    {
        template <typename V>
        inline constexpr auto operator()(V &&v) const
        VARIANT_DECLTYPE_AUTO(get_alt_impl<I - 1>{}(std::forward<V>(v).tail_))
    };

    template <bool Dummy>
    struct get_alt_impl<0, Dummy>
    {
        template <typename V>
        inline constexpr auto operator()(V &&v) const
        VARIANT_DECLTYPE_AUTO(std::forward<V>(v).head_)
    };

    template <typename V, size_t I>
    inline static constexpr auto get_alt(V &&v, in_place_index_t<I>)
    VARIANT_DECLTYPE_AUTO(get_alt_impl<I>{}(std::forward<V>(v)))
};

struct base
{
    template <size_t I, typename V>
    inline static constexpr auto get_alt(V &&v)
    VARIANT_DECLTYPE_AUTO(recursive_union::get_alt(data(std::forward<V>(v)), in_place_index_t<I>{}))
};

struct variant
{
    template <size_t I, typename V>
    inline static constexpr auto get_alt(V &&v)
    VARIANT_DECLTYPE_AUTO(base::get_alt<I>(std::forward<V>(v).impl_))
};

}   /* access */

namespace visitation
{
// VISITATION
// ----------


struct base
{
private:
    template <typename T>
    inline static constexpr const T& at(const T& elem)
    {
      return elem;
    }

    template <typename T, size_t N, typename... Is>
    inline static constexpr const remove_all_extents_t<T>& at(const array<T, N>& elems, size_t i, Is... is)
    {
        return at(elems[i], is...);
    }

    template <typename F, typename... Fs>
    inline static constexpr int visit_visitor_return_type_check()
    {
        static_assert(all(std::is_same<F, Fs>::value...), "`mpark::visit` requires the visitor to have a single return type.");
        return 0;
    }

    template <typename... Fs>
    inline static constexpr array<common_type_t<decay_t<Fs>...>, sizeof...(Fs)>
    make_farray(Fs &&... fs)
    {
        using result = array<common_type_t<decay_t<Fs>...>, sizeof...(Fs)>;
        return visit_visitor_return_type_check<decay_t<Fs>...>(), result{{std::forward<Fs>(fs)...}};
    }

    template <size_t... Is>
    struct dispatcher
    {
        template <typename F, typename... Vs>
        struct impl
        {
            inline static constexpr auto dispatch(F f, Vs... vs)
            VARIANT_DECLTYPE_AUTO(invoke(static_cast<F>(f), access::base::get_alt<Is>(static_cast<Vs>(vs))...))
        };
    };

    template <typename F, typename... Vs, size_t... Is>
    inline static constexpr auto make_dispatch(std::index_sequence<Is...>)
    VARIANT_DECLTYPE_DECAY(&dispatcher<Is...>::template impl<F, Vs...>::dispatch)

    template <size_t I, typename F, typename... Vs>
    inline static constexpr auto make_fdiagonal_impl()
    VARIANT_DECLTYPE_DECAY(make_dispatch<F, Vs...>(std::index_sequence<indexed_type<I, Vs>::value...>{}))

    template <typename F, typename... Vs, size_t... Is>
    inline static constexpr auto make_fdiagonal_impl(std::index_sequence<Is...>)
    VARIANT_DECLTYPE_DECAY(make_farray(make_fdiagonal_impl<Is, F, Vs...>()...))

    template <typename F, typename V, typename... Vs>
    inline static constexpr auto make_fdiagonal()
        -> decltype(make_fdiagonal_impl<F, V, Vs...>(std::make_index_sequence<decay_t<V>::size()>{}))
    {
        static_assert(all((decay_t<V>::size() == decay_t<Vs>::size())...), "all of the variants must be the same size.");
        return make_fdiagonal_impl<F, V, Vs...>(std::make_index_sequence<decay_t<V>::size()>{});
    }

    template <typename F, typename... Vs>
    struct make_fmatrix_impl
    {
        template <typename...>
        struct impl;

        template <size_t... Is>
        struct impl<std::index_sequence<Is...>>
        {
            inline constexpr auto operator()() const
            VARIANT_DECLTYPE_DECAY(make_dispatch<F, Vs...>(std::index_sequence<Is...>{}))
        };

        template <size_t... Is, size_t... Js, typename... Ls>
        struct impl<std::index_sequence<Is...>, std::index_sequence<Js...>, Ls...>
        {
            inline constexpr auto operator()() const
            VARIANT_DECLTYPE_DECAY(make_farray(impl<std::index_sequence<Is..., Js>, Ls...>{}()...))
        };
    };

    template <typename F, typename... Vs>
    inline static constexpr auto make_fmatrix()
    VARIANT_DECLTYPE_DECAY(typename make_fmatrix_impl<F, Vs...>::template impl<std::index_sequence<>, std::make_index_sequence<decay_t<Vs>::size()>...>{}())

public:
    template <typename Visitor, typename... Vs>
    inline static constexpr auto visit_alt_at(size_t index, Visitor&& visitor, Vs&&... vs)
    VARIANT_DECLTYPE_AUTO(
        at(make_fdiagonal<Visitor&&, decltype(as_base(std::forward<Vs>(vs)))...>(), index)
        (std::forward<Visitor>(visitor), as_base(std::forward<Vs>(vs))...)
    )

    template <typename Visitor, typename... Vs>
    inline static constexpr auto visit_alt(Visitor &&visitor, Vs&&... vs)
    VARIANT_DECLTYPE_AUTO(
        at(make_fmatrix<Visitor &&, decltype(as_base(std::forward<Vs>(vs)))...>(), vs.index()...)
        (std::forward<Visitor>(visitor), as_base(std::forward<Vs>(vs))...)
    )
};

struct variant
{
private:
    template <typename Visitor, typename... Values>
    struct visit_exhaustive_visitor_check
    {
        static_assert(is_invocable<Visitor, Values...>::value, "`mpark::visit` requires the visitor to be exhaustive.");

#include <warnings/push.h>
#include <warnings/unused-parameter.h>

        inline constexpr auto operator()(Visitor&& visitor, Values&&... values) const
        VARIANT_DECLTYPE_AUTO(invoke(std::forward<Visitor>(visitor), std::forward<Values>(values)...))

#include <warnings/pop.h>

    };

    template <typename Visitor>
    struct value_visitor
    {
        Visitor &&visitor_;

        template <typename... Alts>
        inline constexpr auto operator()(Alts &&... alts) const
        VARIANT_DECLTYPE_AUTO(
            visit_exhaustive_visitor_check<Visitor, decltype((std::forward<Alts>(alts).value))...>{}(
                std::forward<Visitor>(visitor_),
                std::forward<Alts>(alts).value...
            )
        )
    };

    template <typename Visitor>
    inline static constexpr auto make_value_visitor(Visitor &&visitor)
    VARIANT_DECLTYPE_DECAY(value_visitor<Visitor>{std::forward<Visitor>(visitor)})

public:
    template <typename Visitor, typename... Vs>
    inline static constexpr auto visit_alt_at(size_t index, Visitor&& visitor, Vs&&... vs)
    VARIANT_DECLTYPE_AUTO(base::visit_alt_at(index, std::forward<Visitor>(visitor), std::forward<Vs>(vs).impl_...))

    template <typename Visitor, typename... Vs>
    inline static constexpr auto visit_alt(Visitor &&visitor, Vs&&... vs)
    VARIANT_DECLTYPE_AUTO(base::visit_alt(std::forward<Visitor>(visitor), std::forward<Vs>(vs).impl_...))

    template <typename Visitor, typename... Vs>
    inline static constexpr auto visit_value_at(size_t index, Visitor&& visitor, Vs&&... vs)
    VARIANT_DECLTYPE_AUTO(visit_alt_at(index, make_value_visitor(std::forward<Visitor>(visitor)), std::forward<Vs>(vs)...))

    template <typename Visitor, typename... Vs>
    inline static constexpr auto visit_value(Visitor&& visitor, Vs&&... vs)
    VARIANT_DECLTYPE_AUTO(visit_alt(make_value_visitor(std::forward<Visitor>(visitor)), std::forward<Vs>(vs)...))
};

}   /* visitation */

template <size_t Index, typename T>
struct alt
{
    using value_type = T;

#include <warnings/push.h>
#include <warnings/conversion.h>

    template <typename... Args>
    inline explicit constexpr alt(in_place_t, Args &&... args):
        value(std::forward<Args>(args)...)
    {}

#include <warnings/pop.h>

    T value;
};

template <Trait DestructibleTrait, size_t Index, typename... Ts>
union recursive_union;

template <Trait DestructibleTrait, size_t Index>
union recursive_union<DestructibleTrait, Index>
{};

#define VARIANT_RECURSIVE_UNION(destructible_trait, destructor)                         \
    template <size_t Index, typename T, typename... Ts>                                 \
    union recursive_union<destructible_trait, Index, T, Ts...>                          \
    {                                                                                   \
    public:                                                                             \
        inline explicit constexpr recursive_union(valueless_t) noexcept:                \
            dummy_{}                                                                    \
        {}                                                                              \
                                                                                        \
        template <typename... Args>                                                     \
        inline explicit constexpr recursive_union(in_place_index_t<0>, Args&&... args): \
            head_(in_place_t{}, std::forward<Args>(args)...)                            \
        {}                                                                              \
                                                                                        \
        template <size_t I, typename... Args>                                           \
        inline explicit constexpr recursive_union(in_place_index_t<I>, Args&&... args): \
            tail_(in_place_index_t<I - 1>{}, std::forward<Args>(args)...)               \
        {}                                                                              \
                                                                                        \
        recursive_union(const recursive_union &) = default;                             \
        recursive_union(recursive_union &&) = default;                                  \
                                                                                        \
        destructor                                                                      \
                                                                                        \
        recursive_union &operator=(const recursive_union&) = default;                   \
        recursive_union &operator=(recursive_union&&) = default;                        \
                                                                                        \
    private:                                                                            \
        char dummy_;                                                                    \
        alt<Index, T> head_;                                                            \
        recursive_union<destructible_trait, Index + 1, Ts...> tail_;                    \
                                                                                        \
        friend struct access::recursive_union;                                          \
  }

VARIANT_RECURSIVE_UNION(Trait::TriviallyAvailable, ~recursive_union() = default;);
VARIANT_RECURSIVE_UNION(Trait::Available, ~recursive_union() {});
VARIANT_RECURSIVE_UNION(Trait::Unavailable, ~recursive_union() = delete;);

#undef VARIANT_RECURSIVE_UNION

}   /* detail */

// DECLARATIONS
// ------------

struct bad_variant_access: std::exception
{
    virtual const char *what() const noexcept;
};

template <typename T>
struct variant_size<const T>: variant_size<T>
{};

template <typename T>
struct variant_size<volatile T>: variant_size<T>
{};

template <typename T>
struct variant_size<const volatile T>: variant_size<T>
{};

template <typename... Ts>
struct variant_size<variant<Ts...>>: detail::size_constant<sizeof...(Ts)>
{};

template <size_t I, typename T>
struct variant_alternative;

template <size_t I, typename T>
using variant_alternative_t = typename variant_alternative<I, T>::type;

template <size_t I, typename T>
struct variant_alternative<I, const T>: std::add_const<variant_alternative_t<I, T>>
{};

template <size_t I, typename T>
struct variant_alternative<I, volatile T>: std::add_volatile<variant_alternative_t<I, T>>
{};

template <size_t I, typename T>
struct variant_alternative<I, const volatile T>: std::add_cv<variant_alternative_t<I, T>>
{};

template <size_t I, typename... Ts>
struct variant_alternative<I, variant<Ts...>>
{
    static_assert(I < sizeof...(Ts), "Index out of bounds in std::variant_alternative<>");
    using type = detail::type_pack_element_t<I, Ts...>;
};

constexpr size_t variant_npos = static_cast<size_t>(-1);

namespace detail
{
// DETAIL
// ------

using index_t = unsigned int;

template <Trait DestructibleTrait, typename... Ts>
class base
{
public:
    inline explicit constexpr base(valueless_t tag) noexcept:
        data_(tag),
        index_(static_cast<index_t>(-1))
    {}

    template <size_t I, typename... Args>
    inline explicit constexpr base(in_place_index_t<I>, Args&&... args):
        data_(in_place_index_t<I>{}, std::forward<Args>(args)...),
        index_(I)
    {}

    inline constexpr bool valueless_by_exception() const noexcept
    {
        return index_ == static_cast<index_t>(-1);
    }

    inline constexpr size_t index() const noexcept
    {
        return valueless_by_exception() ? variant_npos : index_;
    }

protected:
    using data_t = recursive_union<DestructibleTrait, 0, Ts...>;

    friend inline constexpr base& as_base(base& b)
    {
        return b;
    }

    friend inline constexpr const base& as_base(const base& b)
    {
        return b;
    }

    friend inline constexpr base&& as_base(base&& b)
    {
        return std::move(b);
    }

    friend inline constexpr const base&& as_base(const base&& b)
    {
        return std::move(b);
    }

    friend inline constexpr data_t& data(base& b)
    {
        return b.data_;
    }

    friend inline constexpr const data_t& data(const base& b)
    {
        return b.data_;
    }

    friend inline constexpr data_t&& data(base&& b)
    {
        return std::move(b).data_;
    }

    friend inline constexpr const data_t&& data(const base&& b)
    {
        return std::move(b).data_;
    }

    inline static constexpr size_t size()
    {
        return sizeof...(Ts);
    }

    data_t data_;
    index_t index_;

    friend struct access::base;
    friend struct visitation::base;
};

struct dtor
{
#include <warnings/push.h>
#include <warnings/unused-parameter.h>

    template <typename Alt>
    inline void operator()(Alt &alt) const noexcept
    {
        alt.~Alt();
    }

#include <warnings/pop.h>
};

#if defined(HAVE_MSVC) && _MSC_VER < 1910
#   define INHERITING_CTOR(type, base)                      \
        template <typename... Args>                         \
        inline explicit constexpr type(Args &&... args):    \
            base(std::forward<Args>(args)...)               \
        {}
#else
#   define INHERITING_CTOR(type, base) using base::base;
#endif

template <typename Traits, Trait = Traits::destructible_trait>
class destructor;

#define VARIANT_DESTRUCTOR(destructible_trait, definition, destroy)     \
    template <typename... Ts>                                           \
    class destructor<traits<Ts...>, destructible_trait>:                \
        public base<destructible_trait, Ts...>                          \
    {                                                                   \
        using super = base<destructible_trait, Ts...>;                  \
                                                                        \
    public:                                                             \
        INHERITING_CTOR(destructor, super)                              \
        using super::operator=;                                         \
                                                                        \
        destructor(const destructor &) = default;                       \
        destructor(destructor &&) = default;                            \
        definition                                                      \
        destructor &operator=(const destructor &) = default;            \
        destructor &operator=(destructor &&) = default;                 \
                                                                        \
    protected:                                                          \
        destroy                                                         \
    }

VARIANT_DESTRUCTOR(
    Trait::TriviallyAvailable,
    ~destructor() = default;,
    inline void destroy() noexcept
    {
        this->index_ = static_cast<index_t>(-1);
    }
);

VARIANT_DESTRUCTOR(
    Trait::Available,
    ~destructor() { destroy(); },
    inline void destroy() noexcept
    {
        if (!this->valueless_by_exception()) {
            visitation::base::visit_alt(dtor{}, *this);
        }
        this->index_ = static_cast<index_t>(-1);
    }
);

VARIANT_DESTRUCTOR(
    Trait::Unavailable,
    ~destructor() = delete;,
    inline void destroy() noexcept = delete;
);

#undef VARIANT_DESTRUCTOR

template <typename Traits>
class constructor: public destructor<Traits>
{
    using super = destructor<Traits>;

public:
    INHERITING_CTOR(constructor, super)
    using super::operator=;

protected:
    struct ctor
    {
        template <typename LhsAlt, typename RhsAlt>
        inline void operator()(LhsAlt& lhs_alt, RhsAlt&& rhs_alt) const
        {
            constructor::construct_alt(lhs_alt, std::forward<RhsAlt>(rhs_alt).value);
        }
    };

    template <size_t I, typename T, typename... Args>
    inline static T& construct_alt(alt<I, T> &a, Args &&... args)
    {
        ::new (static_cast<void *>(std::addressof(a))) alt<I, T>(in_place_t{}, std::forward<Args>(args)...);
        return a.value;
    }

    template <typename Rhs>
    inline static void generic_construct(constructor &lhs, Rhs &&rhs)
    {
        lhs.destroy();
        if (!rhs.valueless_by_exception()) {
            visitation::base::visit_alt_at(rhs.index(), ctor{}, lhs, std::forward<Rhs>(rhs));
            lhs.index_ = rhs.index_;
        }
    }
};

template <typename Traits, Trait = Traits::move_constructible_trait>
class move_constructor;

#define VARIANT_MOVE_CONSTRUCTOR(move_constructible_trait, definition)  \
    template <typename... Ts>                                           \
    class move_constructor<traits<Ts...>, move_constructible_trait>:    \
        public constructor<traits<Ts...>>                               \
    {                                                                   \
        using super = constructor<traits<Ts...>>;                       \
                                                                        \
    public:                                                             \
        INHERITING_CTOR(move_constructor, super)                        \
        using super::operator=;                                         \
                                                                        \
        move_constructor(const move_constructor &) = default;           \
        definition                                                      \
        ~move_constructor() = default;                                  \
        move_constructor &operator=(const move_constructor &) = default;\
        move_constructor &operator=(move_constructor &&) = default;     \
    }

VARIANT_MOVE_CONSTRUCTOR(
    Trait::TriviallyAvailable,
    move_constructor(move_constructor &&that) = default;
);

VARIANT_MOVE_CONSTRUCTOR(
    Trait::Available,
    move_constructor(move_constructor &&that) noexcept(all(std::is_nothrow_move_constructible<Ts>::value...)):
        move_constructor(valueless_t{})
    {
        this->generic_construct(*this, std::move(that));
    }
);

VARIANT_MOVE_CONSTRUCTOR(
    Trait::Unavailable,
    move_constructor(move_constructor &&) = delete;
);

#undef VARIANT_MOVE_CONSTRUCTOR

template <typename Traits, Trait = Traits::copy_constructible_trait>
class copy_constructor;

#define VARIANT_COPY_CONSTRUCTOR(copy_constructible_trait, definition)  \
    template <typename... Ts>                                           \
    class copy_constructor<traits<Ts...>, copy_constructible_trait>:    \
        public move_constructor<traits<Ts...>>                          \
    {                                                                   \
        using super = move_constructor<traits<Ts...>>;                  \
                                                                        \
    public:                                                             \
        INHERITING_CTOR(copy_constructor, super)                        \
        using super::operator=;                                         \
                                                                        \
        definition                                                      \
        copy_constructor(copy_constructor &&) = default;                \
        ~copy_constructor() = default;                                  \
        copy_constructor &operator=(const copy_constructor &) = default;\
        copy_constructor &operator=(copy_constructor &&) = default;     \
    }

VARIANT_COPY_CONSTRUCTOR(
    Trait::TriviallyAvailable,
    copy_constructor(const copy_constructor &that) = default;
);

VARIANT_COPY_CONSTRUCTOR(
    Trait::Available,
    copy_constructor(const copy_constructor &that): copy_constructor(valueless_t{})
    {
        this->generic_construct(*this, that);
    }
);

VARIANT_COPY_CONSTRUCTOR(
    Trait::Unavailable,
    copy_constructor(const copy_constructor &) = delete;
);

#undef VARIANT_COPY_CONSTRUCTOR

template <typename Traits>
class assignment : public copy_constructor<Traits>
{
    using super = copy_constructor<Traits>;

public:
    INHERITING_CTOR(assignment, super)
    using super::operator=;

    template <size_t I, typename... Args>
    inline auto emplace(Args &&... args)
        -> decltype(this->construct_alt(access::base::get_alt<I>(*this), std::forward<Args>(args)...))
    {
        this->destroy();
        auto &result = this->construct_alt(access::base::get_alt<I>(*this), std::forward<Args>(args)...);
        this->index_ = I;
        return result;
    }

protected:
    template <typename That>
    struct assigner
    {
        template <typename ThisAlt, typename ThatAlt>
        inline void operator()(ThisAlt &this_alt, ThatAlt &&that_alt) const
        {
            self->assign_alt(this_alt, std::forward<ThatAlt>(that_alt).value);
        }
        assignment *self;
    };

    template <size_t I, typename T, typename Arg>
    inline void assign_alt(alt<I, T> &a, Arg &&arg)
    {
#include <warnings/push.h>
#include <warnings/conversion.h>
        if (this->index() == I) {
            a.value = std::forward<Arg>(arg);
        } else {
            struct {
                void operator()(std::true_type) const
                {
                    this_->emplace<I>(std::forward<Arg>(arg_));
                }

                void operator()(std::false_type) const
                {
                    this_->emplace<I>(T(std::forward<Arg>(arg_)));
                }
                assignment *this_;
                Arg &&arg_;
          } impl {this, std::forward<Arg>(arg)};

          impl(bool_constant<std::is_nothrow_constructible<T, Arg>::value || !std::is_nothrow_move_constructible<T>::value>{});
        }
#include <warnings/pop.h>
    }

    template <typename That>
    inline void generic_assign(That &&that)
    {
        if (this->valueless_by_exception() && that.valueless_by_exception()) {
            // do nothing.
        } else if (that.valueless_by_exception()) {
            this->destroy();
        } else {
            visitation::base::visit_alt_at(that.index(), assigner<That>{this}, *this, std::forward<That>(that));
        }
    }
};

template <typename Traits, Trait = Traits::move_assignable_trait>
class move_assignment;

#define VARIANT_MOVE_ASSIGNMENT(move_assignable_trait, definition)      \
    template <typename... Ts>                                           \
    class move_assignment<traits<Ts...>, move_assignable_trait>:        \
        public assignment<traits<Ts...>>                                \
    {                                                                   \
        using super = assignment<traits<Ts...>>;                        \
                                                                        \
    public:                                                             \
        INHERITING_CTOR(move_assignment, super)                         \
        using super::operator=;                                         \
                                                                        \
        move_assignment(const move_assignment &) = default;             \
        move_assignment(move_assignment &&) = default;                  \
        ~move_assignment() = default;                                   \
        move_assignment &operator=(const move_assignment &) = default;  \
        definition                                                      \
    }

VARIANT_MOVE_ASSIGNMENT(
    Trait::TriviallyAvailable,
    move_assignment &operator=(move_assignment &&that) = default;
);

VARIANT_MOVE_ASSIGNMENT(
    Trait::Available,
    move_assignment&
    operator=(move_assignment &&that) noexcept(
        all((std::is_nothrow_move_constructible<Ts>::value &&
             std::is_nothrow_move_assignable<Ts>::value)...))
    {
        this->generic_assign(std::move(that));
        return *this;
    }
);

VARIANT_MOVE_ASSIGNMENT(
    Trait::Unavailable,
    move_assignment &operator=(move_assignment &&) = delete;
);

#undef VARIANT_MOVE_ASSIGNMENT

template <typename Traits, Trait = Traits::copy_assignable_trait>
class copy_assignment;

#define VARIANT_COPY_ASSIGNMENT(copy_assignable_trait, definition)      \
    template <typename... Ts>                                           \
    class copy_assignment<traits<Ts...>, copy_assignable_trait>:        \
        public move_assignment<traits<Ts...>>                           \
    {                                                                   \
        using super = move_assignment<traits<Ts...>>;                   \
                                                                        \
    public:                                                             \
        INHERITING_CTOR(copy_assignment, super)                         \
        using super::operator=;                                         \
                                                                        \
        copy_assignment(const copy_assignment &) = default;             \
        copy_assignment(copy_assignment &&) = default;                  \
        ~copy_assignment() = default;                                   \
        definition                                                      \
        copy_assignment &operator=(copy_assignment &&) = default;       \
    }

VARIANT_COPY_ASSIGNMENT(
    Trait::TriviallyAvailable,
    copy_assignment &operator=(const copy_assignment &that) = default;
);

VARIANT_COPY_ASSIGNMENT(
    Trait::Available,
    copy_assignment &operator=(const copy_assignment &that)
    {
        this->generic_assign(that);
        return *this;
    }
);

VARIANT_COPY_ASSIGNMENT(
    Trait::Unavailable,
    copy_assignment &operator=(const copy_assignment &) = delete;
);

#undef VARIANT_COPY_ASSIGNMENT

template <typename... Ts>
class impl: public copy_assignment<traits<Ts...>>
{
    using super = copy_assignment<traits<Ts...>>;

public:
    INHERITING_CTOR(impl, super)
    using super::operator=;

    template <std::size_t I, typename Arg>
    inline void assign(Arg&& arg)
    {
        this->assign_alt(access::base::get_alt<I>(*this), std::forward<Arg>(arg));
    }

    inline void swap(impl &that)
    {
        if (this->valueless_by_exception() && that.valueless_by_exception()) {
            // do nothing.
        } else if (this->index() == that.index()) {
            visitation::base::visit_alt_at(this->index(), swapper{}, *this, that);
        } else {
            impl *lhs = this;
            impl *rhs = std::addressof(that);
            if (lhs->move_nothrow() && !rhs->move_nothrow()) {
                std::swap(lhs, rhs);
            }
            impl tmp(std::move(*rhs));
            // EXTENSION: When the move construction of `lhs` into `rhs` throws
            // and `tmp` is nothrow move constructible then we move `tmp` back
            // into `rhs` and provide the strong exception safety guarantee.
            try {
                this->generic_construct(*rhs, std::move(*lhs));
            } catch (...) {
                if (tmp.move_nothrow()) {
                    this->generic_construct(*rhs, std::move(tmp));
                }
                throw;
            }
            this->generic_construct(*lhs, std::move(tmp));
        }
    }

private:
    struct swapper
    {
        template <typename ThisAlt, typename ThatAlt>
        inline void operator()(ThisAlt &this_alt, ThatAlt &that_alt) const
        {
            std::swap(this_alt.value, that_alt.value);
        }
    };

    inline constexpr bool move_nothrow() const
    {
        return this->valueless_by_exception() || array<bool, sizeof...(Ts)>{
            {std::is_nothrow_move_constructible<Ts>::value...}
        }[this->index()];
    }
};

template <typename... Ts>
struct overload;

template <>
struct overload<>
{
    void operator()() const
    {}
};

template <typename T, typename... Ts>
struct overload<T, Ts...>: overload<Ts...>
{
    using overload<Ts...>::operator();
    identity<T> operator()(T) const
    {
        return {};
    }
};

template <typename T, typename... Ts>
using best_match_t = typename invoke_result_t<overload<Ts...>, T&&>::type;

template <typename T>
struct is_in_place_index: std::false_type
{};

template <std::size_t I>
struct is_in_place_index<in_place_index_t<I>>: std::true_type
{};

template <typename T>
struct is_in_place_type: std::false_type
{};

template <typename T>
struct is_in_place_type<in_place_type_t<T>>: std::true_type
{};

}   /* detail */

template <typename... Ts>
class variant
{
    static_assert(0 < sizeof...(Ts), "variant must consist of at least one alternative.");
    static_assert(detail::all_<!std::is_array<Ts>::value...>::value, "variant can not have an array type as an alternative.");
    static_assert(detail::all_<!std::is_reference<Ts>::value...>::value, "variant can not have a reference type as an alternative.");
    static_assert(detail::all_<!std::is_void<Ts>::value...>::value, "variant can not have a void type as an alternative.");

public:
    template <
        typename Front = detail::type_pack_element_t<0, Ts...>,
        detail::enable_if_t<std::is_default_constructible<Front>::value, int> = 0
    >
    inline constexpr variant() noexcept(std::is_nothrow_default_constructible<Front>::value):
        impl_(in_place_index_t<0>{})
    {}

    variant(const variant&) = default;
    variant(variant&&) = default;

    template <
        typename Arg,
        typename Decayed = detail::decay_t<Arg>,
        detail::enable_if_t<!std::is_same<Decayed, variant>::value, int> = 0,
        detail::enable_if_t<!detail::is_in_place_index<Decayed>::value, int> = 0,
        detail::enable_if_t<!detail::is_in_place_type<Decayed>::value, int> = 0,
        typename T = detail::best_match_t<Arg, Ts...>,
        size_t I = detail::find_index_sfinae<T, Ts...>::value,
        detail::enable_if_t<std::is_constructible<T, Arg>::value, int> = 0
    >
    inline constexpr variant(Arg&& arg) noexcept(std::is_nothrow_constructible<T, Arg>::value):
        impl_(in_place_index_t<I>{}, std::forward<Arg>(arg))
    {}

    template <
        size_t I,
        typename... Args,
        typename T = detail::type_pack_element_t<I, Ts...>,
        detail::enable_if_t<std::is_constructible<T, Args...>::value, int> = 0
    >
    inline explicit constexpr variant(in_place_index_t<I>, Args&&... args)
    noexcept(std::is_nothrow_constructible<T, Args...>::value):
        impl_(in_place_index_t<I>{}, std::forward<Args>(args)...)
    {}

    template <
        size_t I,
        typename U,
        typename... Args,
        typename T = detail::type_pack_element_t<I, Ts...>,
        detail::enable_if_t<std::is_constructible<T, std::initializer_list<U>&, Args...>::value, int> = 0
    >
    inline explicit constexpr variant(in_place_index_t<I>, std::initializer_list<U> list, Args&&... args)
    noexcept(std::is_nothrow_constructible<T, std::initializer_list<U>&, Args...>::value):
        impl_(in_place_index_t<I>{}, list, std::forward<Args>(args)...)
    {}

    template <
        typename T,
        typename... Args,
        size_t I = detail::find_index_sfinae<T, Ts...>::value,
        detail::enable_if_t<std::is_constructible<T, Args...>::value, int> = 0
    >
    inline explicit constexpr variant(in_place_type_t<T>, Args&&... args)
    noexcept(std::is_nothrow_constructible<T, Args...>::value):
        impl_(in_place_index_t<I>{}, std::forward<Args>(args)...)
    {}

    template <
        typename T,
        typename U,
        typename... Args,
        size_t I = detail::find_index_sfinae<T, Ts...>::value,
        detail::enable_if_t<std::is_constructible<T, std::initializer_list<U>&, Args...>::value, int> = 0
    >
    inline explicit constexpr variant(in_place_type_t<T>, std::initializer_list<U> list, Args&&... args)
    noexcept(std::is_nothrow_constructible<T, std::initializer_list<U>&, Args...>::value):
        impl_(in_place_index_t<I>{}, list, std::forward<Args>(args)...)
    {}

    ~variant() = default;

    variant& operator=(const variant&) = default;
    variant& operator=(variant&&) = default;

    template <
        typename Arg,
        detail::enable_if_t<!std::is_same<detail::decay_t<Arg>, variant>::value, int> = 0,
        typename T = detail::best_match_t<Arg, Ts...>,
        size_t I = detail::find_index_sfinae<T, Ts...>::value,
        detail::enable_if_t<(std::is_assignable<T &, Arg>::value && std::is_constructible<T, Arg>::value), int> = 0
    >
    inline variant& operator=(Arg&& arg) noexcept((std::is_nothrow_assignable<T&, Arg>::value && std::is_nothrow_constructible<T, Arg>::value))
    {
        impl_.template assign<I>(std::forward<Arg>(arg));
        return *this;
    }

    template <
        size_t I,
        typename... Args,
        typename T = detail::type_pack_element_t<I, Ts...>,
        detail::enable_if_t<std::is_constructible<T, Args...>::value, int> = 0
    >
    inline T& emplace(Args &&... args)
    {
        return impl_.template emplace<I>(std::forward<Args>(args)...);
    }

    template <
        size_t I,
        typename U,
        typename... Args,
        typename T = detail::type_pack_element_t<I, Ts...>,
        detail::enable_if_t<std::is_constructible<T, std::initializer_list<U>&, Args...>::value, int> = 0
    >
    inline T& emplace(std::initializer_list<U> list, Args &&... args)
    {
        return impl_.template emplace<I>(list, std::forward<Args>(args)...);
    }

    template <
        typename T,
        typename... Args,
        size_t I = detail::find_index_sfinae<T, Ts...>::value,
        detail::enable_if_t<std::is_constructible<T, Args...>::value, int> = 0
    >
    inline T& emplace(Args &&... args)
    {
        return impl_.template emplace<I>(std::forward<Args>(args)...);
    }

    template <
        typename T,
        typename U,
        typename... Args,
        size_t I = detail::find_index_sfinae<T, Ts...>::value,
        detail::enable_if_t<std::is_constructible<T, std::initializer_list<U>&, Args...>::value, int> = 0
    >
    inline T& emplace(std::initializer_list<U> list, Args&&... args)
    {
        return impl_.template emplace<I>(list, std::forward<Args>(args)...);
    }

    inline constexpr bool valueless_by_exception() const noexcept
    {
        return impl_.valueless_by_exception();
    }

    inline constexpr size_t index() const noexcept
    {
        return impl_.index();
    }

    template <
        bool Dummy = true,
        detail::enable_if_t<detail::all_<Dummy, (std::is_move_constructible<Ts>::value && detail::is_swappable<Ts>::value)...>::value, int> = 0
    >
    inline void swap(variant &that) noexcept(detail::all_<(std::is_nothrow_move_constructible<Ts>::value && detail::is_nothrow_swappable<Ts>::value)...>::value)
    {
        impl_.swap(that.impl_);
    }

private:
    detail::impl<Ts...> impl_;

    friend struct detail::access::variant;
    friend struct detail::visitation::variant;
};

template <size_t I, typename... Ts>
inline constexpr bool holds_alternative(const variant<Ts...> &v) noexcept
{
    return v.index() == I;
}

template <typename T, typename... Ts>
inline constexpr bool holds_alternative(const variant<Ts...> &v) noexcept
{
    return holds_alternative<detail::find_index_checked<T, Ts...>::value>(v);
}

namespace detail
{
// DETAIL
// ------

template <size_t I, typename V>
struct generic_get_impl
{
    constexpr generic_get_impl(int)
    {}

    constexpr auto operator()(V &&v) const
    VARIANT_DECLTYPE_AUTO(access::variant::get_alt<I>(std::forward<V>(v)).value)
};

template <size_t I, typename V>
inline constexpr auto generic_get(V&& v)
VARIANT_DECLTYPE_AUTO(generic_get_impl<I, V>(holds_alternative<I>(v) ? 0 : (throw bad_variant_access(), 0))(std::forward<V>(v)))

}  /* detail */

template <size_t I, typename... Ts>
inline constexpr variant_alternative_t<I, variant<Ts...>>& get(variant<Ts...> &v)
{
    return detail::generic_get<I>(v);
}

template <size_t I, typename... Ts>
inline constexpr variant_alternative_t<I, variant<Ts...>>&& get(variant<Ts...>&& v)
{
    return detail::generic_get<I>(std::move(v));
}

template <size_t I, typename... Ts>
inline constexpr const variant_alternative_t<I, variant<Ts...>>& get(const variant<Ts...>& v)
{
    return detail::generic_get<I>(v);
}

template <size_t I, typename... Ts>
inline constexpr const variant_alternative_t<I, variant<Ts...>>&& get(const variant<Ts...>&& v)
{
    return detail::generic_get<I>(std::move(v));
}

template <typename T, typename... Ts>
inline constexpr T& get(variant<Ts...>& v)
{
    return get<detail::find_index_checked<T, Ts...>::value>(v);
}

template <typename T, typename... Ts>
inline constexpr T&& get(variant<Ts...>&& v)
{
    return get<detail::find_index_checked<T, Ts...>::value>(std::move(v));
}

template <typename T, typename... Ts>
inline constexpr const T& get(const variant<Ts...>& v)
{
    return get<detail::find_index_checked<T, Ts...>::value>(v);
}

template <typename T, typename... Ts>
inline constexpr const T&& get(const variant<Ts...>&& v)
{
    return get<detail::find_index_checked<T, Ts...>::value>(std::move(v));
}

namespace detail
{
// DETAIL
// ------

template <size_t I, typename V>
inline constexpr auto generic_get_if(V *v) noexcept
VARIANT_DECLTYPE_DECAY(v && holds_alternative<I>(*v) ? std::addressof(access::variant::get_alt<I>(*v).value) : nullptr)

}   /* detail */

template <size_t I, typename... Ts>
inline constexpr detail::add_pointer_t<variant_alternative_t<I, variant<Ts...>>>
get_if(variant<Ts...>* v) noexcept
{
    return detail::generic_get_if<I>(v);
}

template <size_t I, typename... Ts>
inline constexpr detail::add_pointer_t<const variant_alternative_t<I, variant<Ts...>>>
get_if(const variant<Ts...>* v) noexcept
{
    return detail::generic_get_if<I>(v);
}

template <typename T, typename... Ts>
inline constexpr detail::add_pointer_t<T>
get_if(variant<Ts...> *v) noexcept
{
    return get_if<detail::find_index_checked<T, Ts...>::value>(v);
}

template <typename T, typename... Ts>
inline constexpr detail::add_pointer_t<const T>
get_if(const variant<Ts...>* v) noexcept
{
    return get_if<detail::find_index_checked<T, Ts...>::value>(v);
}

template <typename... Ts>
inline constexpr bool operator==(const variant<Ts...>& lhs, const variant<Ts...>& rhs)
{
    using detail::visitation::variant;
    using detail::equal_to;

    return lhs.index() == rhs.index() && (lhs.valueless_by_exception() || variant::visit_value_at(lhs.index(), equal_to{}, lhs, rhs));
}

template <typename... Ts>
inline constexpr bool operator!=(const variant<Ts...>& lhs, const variant<Ts...>& rhs)
{
    using detail::visitation::variant;
    using detail::not_equal_to;

    return lhs.index() != rhs.index() || (!lhs.valueless_by_exception() && variant::visit_value_at(lhs.index(), not_equal_to{}, lhs, rhs));
}

template <typename... Ts>
inline constexpr bool operator<(const variant<Ts...>& lhs, const variant<Ts...>& rhs)
{
    using detail::visitation::variant;
    using detail::less;

    return !rhs.valueless_by_exception() && (lhs.valueless_by_exception() || lhs.index() < rhs.index() || (lhs.index() == rhs.index() && variant::visit_value_at(lhs.index(), less{}, lhs, rhs)));
}

template <typename... Ts>
inline constexpr bool operator>(const variant<Ts...>& lhs, const variant<Ts...>& rhs)
{
    using detail::visitation::variant;
    using detail::greater;

    return !lhs.valueless_by_exception() && (rhs.valueless_by_exception() || lhs.index() > rhs.index() || (lhs.index() == rhs.index() && variant::visit_value_at(lhs.index(), greater{}, lhs, rhs)));
}

template <typename... Ts>
inline constexpr bool operator<=(const variant<Ts...>& lhs, const variant<Ts...>& rhs)
{
    using detail::visitation::variant;
    using detail::less_equal;

    return lhs.valueless_by_exception() || (!rhs.valueless_by_exception() && (lhs.index() < rhs.index() || (lhs.index() == rhs.index() && variant::visit_value_at(lhs.index(), less_equal{}, lhs, rhs))));
}

template <typename... Ts>
inline constexpr bool operator>=(const variant<Ts...>& lhs, const variant<Ts...>& rhs)
{
    using detail::visitation::variant;
    using detail::greater_equal;

    return rhs.valueless_by_exception() || (!lhs.valueless_by_exception() && (lhs.index() > rhs.index() || (lhs.index() == rhs.index() && variant::visit_value_at(lhs.index(), greater_equal{}, lhs, rhs))));
}

template <typename Visitor, typename... Vs>
inline constexpr auto visit(Visitor&& visitor, Vs&&... vs)
VARIANT_DECLTYPE_AUTO(
    (detail::all(!vs.valueless_by_exception()...) ? (void)0 : throw bad_variant_access()),
    detail::visitation::variant::visit_value(std::forward<Visitor>(visitor), std::forward<Vs>(vs)...)
)

struct monostate
{};

inline constexpr bool operator<(monostate, monostate) noexcept
{
    return false;
}

inline constexpr bool operator>(monostate, monostate) noexcept
{
    return false;
}

inline constexpr bool operator<=(monostate, monostate) noexcept
{
    return true;
}

inline constexpr bool operator>=(monostate, monostate) noexcept
{
    return true;
}

inline constexpr bool operator==(monostate, monostate) noexcept
{
    return true;
}

inline constexpr bool operator!=(monostate, monostate) noexcept
{
    return false;
}

template <typename... Ts>
inline auto swap(variant<Ts...>& lhs, variant<Ts...>& rhs) noexcept(noexcept(lhs.swap(rhs)))
    -> decltype(lhs.swap(rhs))
{
    lhs.swap(rhs);
}

namespace detail
{
// DETAIL
// ------

template <typename T, typename...>
using enabled_type = T;

namespace hash
{
// HASH
// ----

template <typename H, typename K>
constexpr bool meets_requirements()
{
    return std::is_copy_constructible<H>::value &&
        std::is_move_constructible<H>::value &&
        is_invocable_r<size_t, H, const K &>::value;
}

template <typename K>
constexpr bool is_enabled()
{
    using H = std::hash<K>;
    return meets_requirements<H, K>() &&
        std::is_default_constructible<H>::value &&
        std::is_copy_assignable<H>::value &&
        std::is_move_assignable<H>::value;
}

}   /* hash */
}   /* detail */

// CLEANUP
// -------

#undef VARIANT_DECLTYPE_AUTO
#undef VARIANT_DECLTYPE_DECAY

#endif                              // HAVE_CPP17

PYCPP_END_NAMESPACE

namespace std
{
// SPECIALIZATION
// --------------

PYCPP_USING_NAMESPACE

template <typename... Ts>
struct hash<detail::enabled_type<variant<Ts...>, detail::enable_if_t<detail::all_<detail::hash::is_enabled<detail::remove_const_t<Ts>>()...>::value>>>
{
    using argument_type = variant<Ts...>;
    using result_type = size_t;

    inline result_type operator()(const argument_type &v) const
    {
        using detail::visitation::variant;
        size_t result = v.valueless_by_exception() ? 299792458 : variant::visit_alt(hasher{}, v);
        return hash_combine(result, hash<size_t>{}(v.index()));
    }

private:
    struct hasher
    {
        template <typename Alt>
        inline size_t operator()(const Alt &alt) const
        {
            using alt_type = detail::decay_t<Alt>;
            using value_type = detail::remove_const_t<typename alt_type::value_type>;
            return hash<value_type>{}(alt.value);
        }
    };

    static size_t hash_combine(size_t lhs, size_t rhs)
    {
        return lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    }
};

template <>
struct hash<monostate>
{
    using argument_type = monostate;
    using result_type = size_t;

    inline result_type operator()(const argument_type&) const noexcept
    {
        return 66740831;  // return a fundamentally attractive random value.
    }
};

}   /* std */
