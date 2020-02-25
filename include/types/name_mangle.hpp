#ifndef RHEA_TYPES_NAME_MANGLE_HPP
#define RHEA_TYPES_NAME_MANGLE_HPP

#include <string>

#include <fmt/format.h>

#include "ast/error.hpp"

#include "types.hpp"
#include "to_string.hpp"

namespace rhea { namespace types {
    /*
     * As Rhea supports functions overloaded on the basis of their argument and
     * return types, it must have some form of name-mangling to allow the linker
     * to keep track of which overload is being called.
     */

    // The different classes of function that may be mangled. (Generics aren't
    // listed, because they're converted into regular functions beforehand.)
    enum class FunctionClass { Function, Predicate, Operator, Unchecked };

    // Given the unmangled name of a function and a type info object describing it,
    // produce a mangled name.
    std::string mangle_function_name(std::string name, FunctionType function_type,
        FunctionClass function_class);

    namespace internal {
        template <typename T>
        std::string mangle_argument_name(T& argument_type)
        { throw rhea::ast::unimplemented_type(to_string(argument_type)); }
    }

    // TODO: Go the other way, from a mangled function name into a type info object.
    // This might be needed for overload resolution or something.
}}

#endif /* RHEA_TYPES_NAME_MANGLE_HPP */