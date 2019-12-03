#ifndef RHEA_NODES_BINOP_HPP
#define RHEA_NODES_BINOP_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"

/*
 * AST nodes for binary operators. These are all pretty much the
 * same thing, just with different code generation.
 */
namespace rhea { namespace ast {
    // All the operators
    enum class BinaryOperators
    {
        Add,
        Subtract,
        Multiply,
        Divide,
        Modulus,
        Exponent,
        LeftShift,
        RightShift,
        Equals,
        NotEqual,
        LessThan,
        GreaterThan,
        LessThanOrEqual,
        GreaterThanOrEqual,
        BitAnd,
        BitOr,
        BitXor,
        BooleanAnd,
        BooleanOr
    };

    // The base class
    class BinaryOp : public Expression
    {
        public:
        BinaryOp(BinaryOperators o, expression_ptr& l, expression_ptr& r)
            : op(o), left(std::move(l)), right(std::move(r)) {}

        const BinaryOperators op;
        const expression_ptr left;
        const expression_ptr right;

        std::string to_string()
            { return fmt::format("(BinaryOp,{0},{1},{2})",
                static_cast<int>(op), left->to_string(), right->to_string()); }
    };
}}

#endif /* RHEA_NODES_BINOP_HPP */