#ifndef RHEA_NODES_BINOP_HPP
#define RHEA_NODES_BINOP_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"
#include "identifiers.hpp"

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
        BinaryOp(BinaryOperators o, expression_ptr l, expression_ptr r)
            : op(o), left(std::move(l)), right(std::move(r)) {}

        const BinaryOperators op;
        const expression_ptr left;
        const expression_ptr right;

        std::string to_string()
            { return fmt::format("(BinaryOp,{0},{1},{2})",
                static_cast<int>(op), left->to_string(), right->to_string()); }
    };

    // The following have different semantics, but they can still be
    // considered binary operators, as they operate on two arguments.

    // AST for member access operation. Note that the order of the
    // arguments seems reversed. This is mainly for parsing reasons.
    class Member : public Expression
    {
        public:
        Member(std::unique_ptr<Identifier> m, expression_ptr o)
            : member(std::move(m)), object(std::move(o)) {}

        const std::unique_ptr<Identifier> member;
        const expression_ptr object;

        std::string to_string() override
            { return fmt::format("(Member,{0},{1})", member->to_string(), object->to_string()); }
    };

    // AST for subscript operation.
    class Subscript : public Expression
    {
        public:
        Subscript(expression_ptr c, expression_ptr i)
            : container(std::move(c)), index(std::move(i)) {}

        const expression_ptr container;
        const expression_ptr index;

        std::string to_string() override
            { return fmt::format("(Subscript,{0},{1})", container->to_string(), index->to_string()); }
    };
}}

#endif /* RHEA_NODES_BINOP_HPP */