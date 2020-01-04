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

namespace rhea { namespace util { 

#ifdef __cpp_lib_variant
    using std::variant;
    using std::visit;
    using std::monostate;
#else
    using boost::variant2::variant;
    using boost::variant2::visit;
    using boost::variant2::monostate;
#endif

}}

#endif /* RHEA_COMPAT_HPP */