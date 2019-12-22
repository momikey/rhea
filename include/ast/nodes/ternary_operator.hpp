#ifndef RHEA_NODES_TERNARY_HPP
#define RHEA_NODES_TERNARY_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"

/*
 * AST node for the ternary operator. Note that this is for the
 * if-then-else *expression*, not the if-else *statement*, which
 * can be found with other control flow nodes.
 */
namespace rhea { namespace ast {
    class TernaryOp : public Expression
    {
        public:
        TernaryOp(expression_ptr c, expression_ptr t, expression_ptr f) :
            condition(std::move(c)), true_branch(std::move(t)), false_branch(std::move(f)) {}

        const expression_ptr condition;
        const expression_ptr true_branch;
        const expression_ptr false_branch;

        std::string to_string()
         { return fmt::format("(TernaryOp,{0},{1},{2})",
            condition->to_string(), true_branch->to_string(), false_branch->to_string()); }
    };
}}

#endif /* RHEA_NODES_TERNARY_HPP */