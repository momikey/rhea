#ifndef RHEA_TYPES_TO_STRING_HPP
#define RHEA_TYPES_TO_STRING_HPP

#include <string>

#include "types.hpp"
#include "../util/compat.hpp"

/*
 * Convert Rhea "type info" structures to strings, for use in error messages, etc.
 */
namespace rhea { namespace types {
    namespace internal {
        // These helper functions are overloads for the variant visitor.

        // First, the template itself.
        template <typename T>
        std::string to_string(T tinfo) { return "(unknown)"; }

        // Now, specializations for each type in the variant.
        template <>
        inline std::string to_string(NothingType tinfo) { return "nothing"; }

        template <>
        inline std::string to_string(SimpleType tinfo)
        {
            switch (tinfo.type)
            {
                case BasicType::Integer:            return "integer";
                case BasicType::Byte:               return "byte";
                case BasicType::Long:               return "long";
                case BasicType::UnsignedInteger:    return "uinteger";
                case BasicType::UnsignedByte:       return "ubyte";
                case BasicType::UnsignedLong:       return "ulong";
                case BasicType::Float:              return "float";
                case BasicType::Double:             return "double";
                case BasicType::Boolean:            return "boolean";
                case BasicType::Symbol:             return "symbol";
                case BasicType::String:             return "string";
                case BasicType::Any:                return "any";
                case BasicType::Nothing:            return "nothing";

                default:                            return "unknown";
            }
        }
    }

    std::string to_string(TypeInfo tinfo) { return util::visit(
        [&](auto const& t){ return internal::to_string(t); }
        , tinfo); }
}}

#endif /* RHEA_TYPES_TO_STRING_HPP */