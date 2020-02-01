#ifndef RHEA_NODES_CONTROL_HPP
#define RHEA_NODES_CONTROL_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"
#include "identifiers.hpp"
#include "simple_statement.hpp"
#include "typenames.hpp"

/*
 * AST nodes for control flow statements. In Rhea, this includes the
 * `if`, `unless`, `while`, `for`, `with`, and `match` statements,
 * as well as `break` and `continue`.
 */
namespace rhea { namespace ast {
    // Node class for `if` and `unless` statements. Note that the AST
    // conflates the two: `unless` is just `if` with a null true case.
    class If : public Statement
    {
        public:
        If(expression_ptr c, statement_ptr t, statement_ptr e) :
            condition(std::move(c)), then_case(std::move(t)), else_case(std::move(e)) {}

        const expression_ptr condition;
        const statement_ptr then_case;
        const statement_ptr else_case;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override { return fmt::format("(If,{0},{1},{2})", condition->to_string(),
            then_case != nullptr ? then_case->to_string() : "null",
            else_case != nullptr ? else_case->to_string() : "null"); }
    };

    // While loop AST node class.
    class While : public Statement
    {
        public:
        While(expression_ptr c, statement_ptr b)
            : condition(std::move(c)), body(std::move(b)) {}

        const expression_ptr condition;
        const statement_ptr body;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(While,{0},{1})", condition->to_string(), body->to_string()); }
    };

    // For loop AST node class. We store the index variable as a
    // simple string rather than an Identifier node for the same
    // reasons as with type pairs.
    class For : public Statement
    {
        public:
        For(std::string i, expression_ptr r, statement_ptr b)
            : index(i), range(std::move(r)), body(std::move(b)) {}

        const std::string index;
        const expression_ptr range;
        const statement_ptr body;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(For,{0},{1},{2})", index, range->to_string(), body->to_string()); }
    };

    // With statement. This creates a new scope, prepopulating it with
    // an arbitrary number of default-initialized variables. From the AST's
    // perspective, that's just a bunch of type pairs.
    // TODO: Change the semantics of `with` to take one or more predicates
    // (in a list) that are used as loop/scope invariants.
    class With : public Statement
    {
        public:
        With(child_vector<TypePair>& ds, statement_ptr b);

        const statement_ptr body;
        child_vector<TypePair> declarations;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };

    // Break and continue are relatively easy (and very similar).
    class Break : public Statement
    {
        public:
        Break() {}

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override { return "(Break)"; }
    };

    class Continue : public Statement
    {
        public:
        Continue() {}

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() { return "(Continue)"; }
    };
}}

#endif /* RHEA_NODES_CONTROL_HPP */