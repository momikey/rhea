#ifndef RHEA_TYPES_INFO_HPP
#define RHEA_TYPES_INFO_HPP

#include "../util/compat.hpp"

/*
 * Type objects for Rhea data types. These use static polymorphism (variants),
 * rather than virtual functions.
 */
namespace rhea { namespace types {
    // This enum holds all the basic literal types Rhea understands.
    enum class BasicType
    {
        Integer,
        Byte,
        Float,
        Double,
        Long,
        UnsignedInteger,
        UnsignedByte,
        UnsignedLong,

        Boolean,
        String,
        Symbol,
        Any,
        Nothing,
        
        Other,          // Used for structures, user-defined types, generics, etc.
        
        Promoted,       // Used for the coercion opertor `^`
        Unknown = -1    // Error case
    };

    // An unknown type, which can go anywhere, but can't be compared. It's
    // basically a NULL type.
    struct UnknownType
    {
        template <typename T>
        bool is_compatible(T other) { return false; }
    };

    // Simple types are those of literals, such as integers, doubles, strings, etc.
    struct SimpleType
    {
        SimpleType(BasicType t) : type(t) {}
        SimpleType(BasicType t, bool n): type(t), is_numeric(n), is_integral(false) {}
        SimpleType(BasicType t, bool n, bool i) : type(t), is_numeric(n), is_integral(i) {}
        BasicType type;
        bool is_numeric;
        bool is_integral;

        template <typename T>
        bool is_compatible(T other) { return false; }
    };

    // The nothing type is special, because it can't be converted to/from, but it can
    // be used as a return type.
    struct NothingType
    {
        template <typename T>
        bool is_compatible(T other) { return false; }
    };

    ////
    // Specializations for type classes
    ////

    // Scalar types are only compatible with themselves. (We can do things like promotion
    // elsewhere in the compiler.)
    template <>
    inline bool SimpleType::is_compatible(SimpleType other) { return type == other.type; }

    // The nothing type is only compatible with itself.
    template <>
    inline bool NothingType::is_compatible(NothingType other) { return true; }

    // The "base" for all types. This is a variant covering all defined structs.
    using TypeInfo = util::variant<
        UnknownType,
        SimpleType,
        NothingType
    >;

    // Comparison function. This *only* checks for exact matches at this time.
    inline bool compatible(TypeInfo& lhs, TypeInfo& rhs)
    {
        auto fn = [&](auto& l, auto& r) { return l.is_compatible(r); };
        return util::visit(fn, lhs, rhs);
    }
}}

#endif /* RHEA_TYPES_INFO_HPP */