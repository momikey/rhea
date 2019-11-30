#ifndef RHEA_NODES_LITERAL_HPP
#define RHEA_NODES_LITERAL_HPP

#include <memory>
#include <string>
#include <type_traits>
#include <fmt/format.h>

#include "node_base.hpp"

/*
 * AST node classes for literal types, including integer and
 * floating-point numbers (in varying widths), booleans, symbols,
 * and strings.
 */

namespace rhea { namespace ast {
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
        Unknown         // Error case
    };

    // Get the proper enum member for a given ype. 
    template <typename T> BasicType numeric_type()    { return BasicType::Unknown; }
    template<> inline BasicType numeric_type<int32_t>()      { return BasicType::Integer; }
    template<> inline BasicType numeric_type<int8_t>()       { return BasicType::Byte;  }
    template<> inline BasicType numeric_type<float>()        { return BasicType::Float; }
    template<> inline BasicType numeric_type<double>()       { return BasicType::Double; }
    template<> inline BasicType numeric_type<int64_t>()      { return BasicType::Long; }
    template<> inline BasicType numeric_type<uint32_t>()     { return BasicType::UnsignedInteger; }
    template<> inline BasicType numeric_type<uint8_t>()      { return BasicType::UnsignedByte; }
    template<> inline BasicType numeric_type<uint64_t>()     { return BasicType::UnsignedLong; }

    // This node class represents any integral literal. We template it
    // on the underlying value type.
    template <typename Int>
    class Integral : public Expression
    {
        static_assert(std::is_integral<Int>::value, "Only integral types are allowed");

        public:
        Integral(Int v): value(v), type(numeric_type<Int>()) {}

        using template_type = Int;

        const Int value;
        const BasicType type;

        std::string to_string() override
            { return fmt::format("(Integral,{0},{1})", value, static_cast<int>(type)); }
    };

    // This node class represents floating-point types. Again, it's
    // templated on the underlying value type.
    template <typename Float>
    class FloatingPoint : public Expression
    {
        static_assert(std::is_floating_point<Float>::value, "Only floating-point types are allowed");

        public:
        FloatingPoint(Float v): value(v), type(numeric_type<Float>()) {}

        using template_type = Float;

        const Float value;
        const BasicType type;

        std::string to_string() override
            { return fmt::format("(FloatingPoint,{0},{1})", value, static_cast<int>(type)); }
    };

    // Convenience declarations for the defined types.
    using Integer = Integral<int32_t>;
    using Byte = Integral<int8_t>;
    using Long = Integral<int64_t>;
    using UnsignedInteger = Integral<uint32_t>;
    using UnsignedByte = Integral<uint8_t>;
    using UnsignedLong = Integral<uint64_t>;
    using Float = FloatingPoint<float>;
    using Double = FloatingPoint<double>;

    // This node class is for boolean literals. It's a lot simpler than
    // the numeric types, as booleans don't have to worry about width.
    class Boolean : public Expression
    {
        public:
        Boolean(bool v): value(v) {}

        const bool value;
        const BasicType type = BasicType::Boolean;

        std::string to_string() { return fmt::format("(Boolean,{0})", value); }
    };

    // For the string literal class, we have to think about encodings.
    // Rhea is UTF-8 by default, at least for strings.
    class String : public Expression
    {
        public:
        String(std::string v): value(v) {}

        const std::string value;
        const BasicType type = BasicType::String;

        std::string to_string() override
            { return fmt::format("(String,\"{0}\")", value); }
    };

    // The symbol node class stores the name of the symbol. We can use that
    // to generate a hash code or some other identifier in a later phase.
    class Symbol : public Expression
    {
        public:
        Symbol(std::string v): value(v) {}

        const std::string value;
        const BasicType type = BasicType::Symbol;

        std::string to_string() override
            { return fmt::format("(Symbol,{0})", value); }
    };

    // For the "nothing" node class, we don't have store do much of anything.
    class Nothing : public Expression
    {
        public:
        Nothing() {}

        // Literals need a value member, but I'm not sure what would work here.
        const void* value = nullptr;
        const BasicType type = BasicType::Nothing;

        std::string to_string() override
            { return fmt::format("(Nothing)"); }
    };
}}

#endif /* RHEA_NODES_LITERAL_HPP */