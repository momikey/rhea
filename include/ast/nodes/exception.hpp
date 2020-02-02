#ifndef RHEA_NODES_EXCEPTION_HPP
#define RHEA_NODES_EXCEPTION_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"
#include "typenames.hpp"

/*
 * AST nodes for exception-related statements.
 */
namespace rhea { namespace ast {

    // Catch statement. If an exception occurs within a `try` block,
    // a `catch` taking the same type as the exception that was thrown
    // will be executed.
    class Catch : public Statement
    {
        public:
        Catch(std::unique_ptr<TypePair> ct, statement_ptr b)
            : catch_type(std::move(ct)), body(std::move(b)) {}

        std::unique_ptr<TypePair> catch_type;
        statement_ptr body;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(Catch,{0},{1})", catch_type->to_string(), body->to_string()); }
    };

    // Throw statement. If executed, this causes an exception structure
    // to be generated. As Rhea is not an OO language, exceptions are
    // not members of a class, but they are always of a specific type.
    class Throw : public Statement
    {
        public:
        Throw(expression_ptr e) : exception(std::move(e)) {}

        expression_ptr exception;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override { return fmt::format("(Throw,{0})", exception->to_string()); }
    };

    // Finally statement. This is executed after a `try` block, whether
    // or not an exception occurred.
    class Finally : public Statement
    {
        public:
        Finally(statement_ptr b) : body(std::move(b)) {}

        statement_ptr body;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override { return fmt::format("(Finally,{0})", body->to_string()); }
    };

    // Try statement (this comes last because it depends on catch/finally).
    class Try : public Statement
    {
        public:
        // Two constructors: one with finally, one without.
        Try(statement_ptr b, child_vector<Catch>& cs, std::unique_ptr<Finally> f);
        Try(statement_ptr b, child_vector<Catch>& cs);

        statement_ptr body;
        child_vector<Catch> catches;
        // Note: We name it like this just in case C++ ever adds a `finally` keyword.
        std::unique_ptr<Finally> finally_block;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };

}}

#endif /* RHEA_NODES_EXCEPTION_HPP */