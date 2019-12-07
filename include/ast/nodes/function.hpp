#ifndef RHEA_NODES_FUNCTION_HPP
#define RHEA_NODES_FUNCTION_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"
#include "identifiers.hpp"
#include "typenames.hpp"

/*
 * AST nodes for function definitions, calls, and arguments.
 */
namespace rhea { namespace ast {
    // A named argument pair: the argument name and an expression.
    class NamedArgument : public ASTNode
    {
        public:
        NamedArgument(std::string n, expression_ptr v)
            : name(n), value(std::move(v)) {}

        const std::string name;
        const expression_ptr value;

        std::string to_string() override
            { return fmt::format("(NamedArgument,{0},{1})", name, value->to_string()); }
    };

    // Convenience alias; functions can take either named or positional arguments.
    using function_argument = variant<std::unique_ptr<Expression>, std::unique_ptr<NamedArgument>>;

    // AST for function calls.
    class Call : public Expression
    {
        public:
        // Positional arguments
        Call(expression_ptr t, child_vector<Expression>& args);
        // Named arguments
        Call(expression_ptr t, child_vector<NamedArgument>& args);
        // No arguments
        Call(expression_ptr t): target(std::move(t)) {}

        const expression_ptr target;
        std::vector<function_argument> arguments;

        std::string to_string() override;
    };

    // AST for the argument list of a function definition.
    // We split this off because functions can have an arbitrary number
    // of arguments, and we might want to handle those in a specific manner.
    class Arguments : public ASTNode
    {
        // TODO: Once we add concepts, we'll need to change to a variant
        // of type pairs and concept matches. Also think about something
        // for the wildcard overload.
        public:
        Arguments(child_vector<TypePair>& args);

        child_vector<TypePair> arguments;

        std::string to_string() override;
    };

    // AST node for the return statement. We put it here because
    // it's function-oriented.
    class Return : public Statement
    {
        public:
        Return(expression_ptr v): value(std::move(v)) {}

        const expression_ptr value;

        std::string to_string() override { return fmt::format("(Return,{0})", value->to_string()); }
    };

    // Extern declarations are only allowed in unchecked functions.
    // They're necessary for FFI, which means they'll need codegen
    // and symbol stuff, but no real type checking.
    class Extern : public Statement
    {
        public:
        Extern(std::string n): name(n) {}

        const std::string name;

        std::string to_string() override { return fmt::format("(Extern,{0})", name); }
    };
}}

#endif /* RHEA_NODES_FUNCTION_HPP */