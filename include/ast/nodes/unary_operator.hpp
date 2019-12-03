#ifndef RHEA_NODES_UNARYOP_HPP
#define RHEA_NODES_UNARYOP_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"

namespace rhea { namespace ast {
    // All the operators
    enum class UnaryOperators
    {
        Plus,
        Minus,
        BooleanNot,
        BitNot,
        Coerce,
        Dereference,
        Ref,
        Ptr
    };

    // The base class
    class UnaryOp : public Expression
    {
        public:
        UnaryOp(UnaryOperators o, expression_ptr& r)
            : op(o), operand(std::move(r)) {}

        const UnaryOperators op;
        const expression_ptr operand;

        std::string to_string() override
            { return fmt::format("(UnaryOp,{0},{1})", static_cast<int>(op), operand->to_string()); }
    };
}}

#endif /* RHEA_NODES_UNARYOP_HPP */