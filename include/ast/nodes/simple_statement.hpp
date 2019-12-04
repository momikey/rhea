#ifndef RHEA_NODES_STATEMENT_HPP
#define RHEA_NODES_STATEMENT_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"
#include "identifiers.hpp"
#include "typenames.hpp"

/*
 * AST nodes for "simple" statements: assignment, variable declaration,
 * bare expressions, the `do` statemment, and the statement block.
 */
namespace rhea { namespace ast {
    // Node class for when an expression is used in statement context.
    // The parser picks up on this, and it may have side effects, so
    // that's why we want to put it here.
    class BareExpression : public Statement
    {
        public:
        BareExpression(expression_ptr e): expression(std::move(e)) {}

        const expression_ptr expression;

        std::string to_string() override
            { return fmt::format("(BareExpression,{0})", expression->to_string()); }
    };

    // Statement blocks are commonly used throughout Rhea's grammar.
    // They're nothing more than a container for statements, really,
    // but they also create a new scope.
    class Block : public Statement
    {
        public:
        Block(child_vector<Statement>& ss);

        std::string to_string() override;

        private:
        child_vector<Statement> m_children;
    };

    // Not all operators are available for use in compound assignments.
    enum class AssignOperator
    {
        Add,
        Subtract,
        Multiply,
        Divide,
        Modulus,
        Exponent,
        LeftShift,
        RightShift,
        BitAnd,
        BitOr,
        BitXor
    };

    // Simple assigment node class.
    class Assign : public Statement
    {
        public:
        // Note that we have to declare the LHS as an expression,
        // because it may be, for example, a subscript: `x[1] = 'foo'`
        // is valid code, but wouldn't work if we only accepted
        // identifiers.
        Assign(expression_ptr l, expression_ptr r): lhs(std::move(l)), rhs(std::move(r)) {}

        const expression_ptr lhs;
        const expression_ptr rhs;

        std::string to_string() override
            { return fmt::format("(Assign,{0},{1})", lhs->to_string(), rhs->to_string()); }
    };

    // Compound assignment adds the operation to the mix. The logic
    // is mostly the same as above.
    class CompoundAssign : public Statement
    {
        public:
        CompoundAssign(expression_ptr l, AssignOperator o, expression_ptr r)
            : lhs(std::move(l)), op(o), rhs(std::move(r)) {}

        const expression_ptr lhs;
        const AssignOperator op;
        const expression_ptr rhs;

        // Note reordering of members in the string representation.
        // String version is (LHS, RHS, OP).
        std::string to_string() override
            { return fmt::format("(CompoundAssign,{0},{1},{2})",
                lhs->to_string(), rhs->to_string(), static_cast<int>(op)); }
    };

    // Type declaration, as in `var foo as string`. In this case,
    // we *can* simply use an identifier for the LHS, because a
    // declaration has to be in the local scope.
    class TypeDeclaration : public Statement
    {
        public:
        TypeDeclaration(std::unique_ptr<AnyIdentifier> l, std::unique_ptr<Typename> r)
            : lhs(std::move(l)), rhs(std::move(r)) {}

        const std::unique_ptr<AnyIdentifier> lhs;
        const std::unique_ptr<Typename> rhs;

        std::string to_string() override
            { return fmt::format("(TypeDeclaration,{0},{1})", lhs->to_string(), rhs->to_string()); }
    };

    // For variable and constant declarations, we use mostly the same
    // logic as for type declarations, but we can have an arbitrary
    // expression on the RHS. Other than that, they're as different as
    // variables and constants. XD
    class Variable : public Statement
    {
        public:
        Variable(std::unique_ptr<AnyIdentifier> l, expression_ptr r)
            : lhs(std::move(l)), rhs(std::move(r)) {}

        const std::unique_ptr<AnyIdentifier> lhs;
        const expression_ptr rhs;

        std::string to_string() override
            { return fmt::format("(Variable,{0},{1})", lhs->to_string(), rhs->to_string()); }
    };

    // From the AST's perspective, declaring a constant is literally
    // the same thing as declaring a variable. Later phases very much
    // care about the distinction, though.
    class Constant : public Statement
    {
        public:
        Constant(std::unique_ptr<AnyIdentifier> l, expression_ptr r)
            : lhs(std::move(l)), rhs(std::move(r)) {}

        const std::unique_ptr<AnyIdentifier> lhs;
        const expression_ptr rhs;

        std::string to_string() override
            { return fmt::format("(Constant,{0},{1})", lhs->to_string(), rhs->to_string()); }
    };

    // The `do` statement doesn't do much of anything, but it can
    // call a bare identifier as if it wree a function taking no
    // arguments. AST doesn't account for this, but codegen does.
    class Do : public Statement
    {
        public:
        Do(expression_ptr e) : expression(std::move(e)) {}

        const expression_ptr expression;

        std::string to_string() override { return fmt::format("(Do,{0})", expression->to_string()); }
    };
}}

#endif /* RHEA_NODES_STATEMENT_HPP */