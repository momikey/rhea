#ifndef RHEA_NODES_NUMERIC_HPP
#define RHEA_NODES_NUMERIC_HPP

#include <memory>
#include <type_traits>
#include <fmt/format.h>

#include "node_base.hpp"

/*
 * AST node classes for numeric types, including both integer and
 * floating-point, in varying widths.
 */

namespace rhea { namespace ast {
    // This enum holds all the basic numeric types Rhea understands.
    enum class Numeric
    {
        Integer,
        Byte,
        Float,
        Double,
        Long,
        UnsignedInteger,
        UnsignedByte,
        UnsignedLong,
        
        Promoted,       // Used for the coercion opertor `^`
        Unknown         // Error case
    };

    // Get the proper enum member for a type. 
    template <typename Int> Numeric numeric_type()  { return Numeric::Unknown; }
    template<> Numeric numeric_type<int32_t>()      { return Numeric::Integer; }
    template<> Numeric numeric_type<int8_t>()       { return Numeric::Byte;  }
    template<> Numeric numeric_type<float>()        { return Numeric::Float; }
    template<> Numeric numeric_type<double>()       { return Numeric::Double; }
    template<> Numeric numeric_type<int64_t>()      { return Numeric::Long; }
    template<> Numeric numeric_type<uint32_t>()     { return Numeric::UnsignedInteger; }
    template<> Numeric numeric_type<uint8_t>()      { return Numeric::UnsignedByte; }
    template<> Numeric numeric_type<uint64_t>()     { return Numeric::UnsignedLong; }

    // This node class represents any integral literal. We template it
    // on the underlying value type.
    template <typename Int>
    class Integral : public ASTNode
    {
        static_assert(std::is_integral<Int>::value, "Only integral types are allowed");

        public:
        Integral(Int v): value(v), type(numeric_type<Int>()) {}

        using template_type = Int;

        const Int value;
        const Numeric type;

        std::string to_string() { return fmt::format("(Integral,{0},{1})", value, static_cast<int>(type)); }
    };

    // This node class represents floating-point types. Again, it's
    // templated on the underlying value type.
    template <typename Float>
    class FloatingPoint : public ASTNode
    {
        static_assert(std::is_floating_point<Float>::value, "Only floating-point types are allowed");

        public:
        FloatingPoint(Float v): value(v), type(numeric_type<Float>()) {}

        using template_type = Float;

        const Float value;
        const Numeric type;

        std::string to_string() { return fmt::format("(FloatingPoint,{0},{1})", value, static_cast<int>(type)); }
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
}}

#endif /* RHEA_NODES_NUMERIC_HPP */