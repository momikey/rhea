#ifndef RHEA_TYPES_INFO_HPP
#define RHEA_TYPES_INFO_HPP

#include <map>
#include <vector>
#include <string>
#include <memory>

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

    // Forward definition for our main type info container class
    struct TypeInfo;
    bool compatible(TypeInfo& lhs, TypeInfo& rhs);

    // An unknown type, which can go anywhere, but can't be compared. It's
    // basically a NULL type.
    struct UnknownType
    {
        template <typename T>
        bool is_compatible(T& other) { return false; }
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
        bool is_compatible(T& other) { return false; }
    };

    // The nothing type is special, because it can't be converted to/from, but it can
    // be used as a return type.
    struct NothingType
    {
        template <typename T>
        bool is_compatible(T& other) { return false; }
    };

    // Function types need a map of strings to argument types, plus a return type.
    struct FunctionType
    {
        // We have to use pointers here because of the circular dependency. Using shared
        // instead of unique pointers lets us avoid a lot of messy move stuff that would
        // require some serious rewriting throughout the compiler.
        std::map<std::string, std::shared_ptr<TypeInfo>> argument_types;
        std::shared_ptr<TypeInfo> return_type;

        template <typename T>
        bool is_compatible(T& other) { return false; }
    };

    // An optional type just needs the contained type.
    struct OptionalType
    {
        std::shared_ptr<TypeInfo> contained_type;

        template <typename T>
        bool is_compatible(T& other);
    };

    // A variant needs multiple types.
    struct VariantType
    {
        std::vector<std::shared_ptr<TypeInfo>> types;

        template <typename T>
        bool is_compatible(T& other) { return false; }
    };

    // A structure keeps a map of strings to field types.
    struct StructureType
    {
        std::map<std::string, std::shared_ptr<TypeInfo>> fields;

        template <typename T>
        bool is_compatible(T& other) { return false; }
    };

    // The "any" type can hold anything, but what it actually holds is an implemenation detail.
    struct AnyType
    {
        // Any is technically compatible with any other type, but only if it's the LHS.
        template <typename T>
        bool is_compatible(T& other) { return true; }
    };

    // The "base" for all types. This is a variant covering all defined structs.
    using TypeInfoVariant = util::variant<
        UnknownType,
        SimpleType,
        NothingType,
        FunctionType,
        OptionalType,
        VariantType,
        StructureType,
        AnyType
    >;

    // The type info container just holds an instance of the variant, and provides
    // a few helper methods to access it.
    struct TypeInfo
    {
        TypeInfo() : type_info_v(UnknownType()) {}

        template<typename T>
        TypeInfo(T t) : type_info_v(t) {}

        TypeInfoVariant& type() { return type_info_v; }

        private:
        TypeInfoVariant type_info_v;
    };
}}

#endif /* RHEA_TYPES_INFO_HPP */