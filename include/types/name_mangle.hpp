#ifndef RHEA_TYPES_NAME_MANGLE_HPP
#define RHEA_TYPES_NAME_MANGLE_HPP

#include <string>
#include <type_traits>

#include <fmt/format.h>

#include "../ast/error.hpp"
#include "../ast/nodes/function.hpp"
#include "../util/compat.hpp"

#include "types.hpp"
#include "to_string.hpp"

namespace rhea { namespace types {
    using namespace std::string_literals;

    // We need to give this a different name, because the type info object is
    // more important.
    using FunctionClass = ast::FunctionType;

    /*
     * As Rhea supports functions overloaded on the basis of their argument and
     * return types, it must have some form of name-mangling to allow the linker
     * to keep track of which overload is being called.
     */

    // Given the unmangled name of a function and a type info object describing it,
    // produce a mangled name.
    std::string mangle_function_name(std::string name, FunctionType function_type,
        FunctionClass function_class = FunctionClass::Basic);

    namespace internal {
        template <typename T>
        std::string mangle_argument_name(T& argument_type)
        { throw rhea::ast::unimplemented_type(to_string(argument_type)); }

        // Overloads for mangling different types. These aren't specializations,
        // because they sometimes have to recurse, and that takes more template
        // trickery than I feel like doing.

        inline std::string mangle_argument_name(NothingType& argument_type)
        { return "v"; }

        inline std::string mangle_argument_name(AnyType& argument_type)
        { return "a"; }

        inline std::string mangle_argument_name(SimpleType& argument_type)
        {
            switch (argument_type.type)
            {
                case BasicType::Integer:
                    return "i";
                case BasicType::Byte:
                    return "c";
                case BasicType::Long:
                    return "l";
                case BasicType::UnsignedInteger:
                    return "I";
                case BasicType::UnsignedByte:
                    return "C";
                case BasicType::UnsignedLong:
                    return "L";
                case BasicType::Float:
                    return "Df";
                case BasicType::Double:
                    return "Dd";
                case BasicType::Boolean:
                    return "b";
                case BasicType::Symbol:
                    return "Sy";
                case BasicType::String:
                    return "s";
                default:
                    throw ast::unimplemented_type(to_string(argument_type));
            }
        }

        inline std::string mangle_argument_name(OptionalType& argument_type)
        {
            return "Op"s + util::visit(
                [&] (auto& e) { return mangle_argument_name(e); },
                argument_type.contained_type->type()
            );
        }

        inline std::string mangle_argument_name(VariantType& argument_type)
        {
            auto result = "V"s + std::to_string(argument_type.types.size());
            for (auto&& t : argument_type.types)
            {
                result += util::visit(
                    [&] (auto& e) { return mangle_argument_name(e); },
                    t->type()
                );
            }
            return result;
        }

        // TODO: Functions, structures, arrays, lists, tuples, enums, dictionaries, ref/ptrs
    }

    // TODO: Go the other way, from a mangled function name into a type info object.
    // This might be needed for overload resolution or something.
}}

#endif /* RHEA_TYPES_NAME_MANGLE_HPP */