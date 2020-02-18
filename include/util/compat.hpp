#ifndef RHEA_COMPAT_HPP
#define RHEA_COMPAT_HPP

/*
 * At present, Rhea is written in C++14. However, a few things provided
 * in C++17 also have Boost versions that are more or less API compatible.
 * This includes, for instance, `any` and `variant` (which is Boost.Variant2).
 * Here, we conditionally include the C++17 header if it's available,
 * falling back to the Boost version if not. We wrap this up in a namespace,
 * too.
 */


// Variants don't enter the standard library until C++17.
// Boost.Variant2 fills the gap for C++14, which is the minimum
// we require.
#ifdef __cpp_lib_variant
#include <variant>
#else
#include <boost/variant2/variant.hpp>
#endif

// The same is true for `any`: it's in the standard starting with C++17,
// but the API is essentially the same as Boost.Any, so we can interchange
// the two.
#ifdef __cpp_lib_any
#include <any>
#else
#include <boost/any.hpp>
#endif

// Optionals aren't until C++17, but the Boost one works about the same.
#ifdef __cpp_lib_optional
#include <optional>
using std::optional;
#else
#include <boost/optional/optional.hpp>
using boost::optional;
#endif
namespace rhea { namespace util { 

#ifdef __cpp_lib_variant
    using std::variant;
    using std::visit;
    using std::monostate;
    using std::get_if;
    using std::get;
#else
    using boost::variant2::variant;
    using boost::variant2::visit;
    using boost::variant2::monostate;
    using boost::variant2::get_if;
    using boost::variant2::get;
#endif

#ifdef __cpp_lib_any
    using std::any;
    using std::any_cast;
    using std::bad_any_cast;
#else
    using boost::any;
    using boost::any_cast;
    using boost::bad_any_cast;
#endif

#ifdef __cpp_lib_optional
    using std::optional;
#else
    using boost::optional;
#endif

}}

#endif /* RHEA_COMPAT_HPP */