#ifndef RHEA_NODES_CONTROL_HPP
#define RHEA_NODES_CONTROL_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"
#include "identifiers.hpp"
#include "simple_statement.hpp"
#include "typenames.hpp"

#include "util/serialize_array.hpp"

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

    // Helper node class for calls to predicates. These are slightly different from
    // regular function calls, as some of them can be evaluated at compile time,
    // and all involve only functions defined as predicates.
    class PredicateCall : public Expression
    {
        public:
        PredicateCall(expression_ptr t, child_vector<Expression>& as);

        expression_ptr target;
        child_vector<Expression> arguments;
        
        types::TypeInfo expression_type() override 
            { return types::SimpleType(types::BasicType::Boolean); }
        util::any visit(visitor::Visitor* v) override;
        std::string to_string();
    };

    // With statement. This takes one or more predicates
    // (in a list) that are used as loop/scope invariants.
    class With : public Statement
    {
        public:
        With(child_vector<PredicateCall>& ds, statement_ptr b);

        const statement_ptr body;
        child_vector<PredicateCall> predicates;

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
        std::string to_string() override { return "(Continue)"; }
    };

    // The `match` statement needs a lot of helper nodes, so here they are.

    // Base class for all the different case types.
    class Case : public ASTNode {};

    // "On" cases are like traditional C-style switch cases, but they can be
    // any constant expression.
    class On : public Case
    {
        public:
        On(expression_ptr c, statement_ptr b) : case_expr(std::move(c)), body(std::move(b)) {}

        expression_ptr case_expr;
        statement_ptr body;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(On,{0},{1})", case_expr->to_string(), body->to_string()); }
    };

    // "When" cases are predicate-based.
    class When : public Case
    {
        public:
        When(std::unique_ptr<PredicateCall> p, statement_ptr b)
            : predicate(std::move(p)), body(std::move(b)) {}

        std::unique_ptr<PredicateCall> predicate;
        statement_ptr body;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(When,{0},{1})", predicate->to_string(), body->to_string()); }
    };

    // "Type" cases are always type assertions or concept checks.
    class TypeCase : public Case
    {
        public:
        TypeCase(std::unique_ptr<Typename> tn, statement_ptr b)
         : type_name(std::move(tn)), body(std::move(b)) {}

        std::unique_ptr<Typename> type_name;
        statement_ptr body;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(TypeCase,{0},{1})", type_name->to_string(), body->to_string()); }
    };

    // The default case works just like in C/C++.
    class Default : public Case
    {
        public:
        Default(statement_ptr b) : body(std::move(b)) {}

        statement_ptr body;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(Default,{0})", body->to_string()); }
    };

    // The `match` statement, which can have multiple `on`, `when`, or `type`
    // cases, but can't mix them. Also, we can have at most one `default`.
    class Match : public Statement
    {
        public:
        Match(expression_ptr e, child_vector<Case>& cs);

        expression_ptr expression;
        child_vector<Case> cases;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };
}}

#endif /* RHEA_NODES_CONTROL_HPP */