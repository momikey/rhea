#include "types/types.hpp"

namespace rhea { namespace types {
    // Comparison function. This *only* checks for exact matches at this time.
    bool compatible(TypeInfo& lhs, TypeInfo& rhs)
    {
        auto fn = [&](auto& l, auto& r) { return l.is_compatible(r); };
        return util::visit(fn, lhs.type(), rhs.type());
    }

    ////
    // Specializations for type classes
    ////

    // Scalar types are only compatible with themselves. (We can do things like promotion
    // elsewhere in the compiler.)
    template <>
    bool SimpleType::is_compatible(SimpleType& other)
    {
        return type == other.type;
    }

    // The nothing type is only compatible with itself.
    template <>
    bool NothingType::is_compatible(NothingType& other)
    {
        return true;
    }

    // Function types are compatible if their signatures are.
    template <>
    bool FunctionType::is_compatible(FunctionType& other)
    {
         return (argument_types == other.argument_types && return_type == other.return_type);
    }
    
    // Optionals types are compatible with other optionals holding the same type
    // *or* that type.
    template <typename T>
    bool OptionalType::is_compatible(T& other)
    {
        return util::visit(
            [&](auto& ct) { return other.is_compatible(ct); },
            contained_type->type()
        );
    }

    template <>
    bool OptionalType::is_compatible(OptionalType& other)
    {
        return *contained_type == other->contained_type;
    }

    // Structures are compatible with structures that have the exact same layout.
    template <>
    bool StructureType::is_compatible(StructureType& other)
    {
         return fields == other.fields;
    }    
}}