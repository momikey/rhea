#ifndef RHEA_NODES_FUNCTION_HPP
#define RHEA_NODES_FUNCTION_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"
#include "identifiers.hpp"
#include "typenames.hpp"
#include "concept.hpp"

#include "util/serialize_array.hpp"

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

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(NamedArgument,{0},{1})", name, value->to_string()); }
    };

    // Convenience alias; functions can take either named or positional arguments.
    using function_argument = util::variant<std::unique_ptr<Expression>, std::unique_ptr<NamedArgument>>;

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

        util::any visit(visitor::Visitor* v) override;
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

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };

    // AST for function pre- and post-conditions.
    class Condition : public ASTNode
    {
        public:
        Condition(std::string t, expression_ptr p)
            : target(t), predicate(std::move(p)) {}

        const std::string target;
        const expression_ptr predicate;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override
            { return fmt::format("(Condition,{0},{1})", target, predicate->to_string()); }
    };

    // Functions can be of different types. This enum allows us
    // to choose which type.
    enum class FunctionType
    {
        Basic,      // i.e., not special
        Predicate,  // implied boolean return type
        Operator,   // operators are always called implicitly
        Unchecked   // can't take conditions
    };

    // AST for function definitions.
    class Def : public Statement
    {
        public:
        // Full function definition
        // TODO: Add simpler constructors that delegate to this one.
        Def(FunctionType t, std::string n, std::unique_ptr<Typename> rt,
            std::unique_ptr<Arguments> al, child_vector<Condition>& cs, statement_ptr b);

        const FunctionType type;
        const std::string name;
        // TODO: Change this to variant of type pair and concept match.
        // const std::unique_ptr<TypePair> generic_type;
        const std::unique_ptr<Typename> return_type;
        const std::unique_ptr<Arguments> arguments_list;
        child_vector<Condition> conditions;
        const statement_ptr body;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
        std::string function_type_string();
    };

    // Convenience alias for a choice between a type or concept match node.
    using GenericMatch = util::variant<
        std::unique_ptr<TypePair>,
        std::unique_ptr<ConceptMatch>
    >;

    // AST for generic function definitions. This is a separate node type because
    // a generic function definition doesn't actually contribute to codegen unless
    // it's instantiated, whereas a "concrete" definition *does*.
    class GenericDef : public Def
    {
        public:
        GenericDef(FunctionType t, std::string n, std::unique_ptr<TypePair> gt,
            std::unique_ptr<Typename> rt, std::unique_ptr<Arguments> al, child_vector<Condition>& cs,
            statement_ptr b) : Def(
                t,
                n,
                std::move(rt),
                std::move(al),
                cs,
                std::move(b)),
            generic_type(std::move(gt)) {}

        // TODO: Change this to variant of type pair and concept match.
        const std::unique_ptr<TypePair> generic_type;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };

    // AST node for the return statement. We put it here because
    // it's function-oriented.
    class Return : public Statement
    {
        public:
        Return(expression_ptr v): value(std::move(v)) {}

        const expression_ptr value;

        util::any visit(visitor::Visitor* v) override;
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

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override { return fmt::format("(Extern,{0})", name); }
    };
}}

#endif /* RHEA_NODES_FUNCTION_HPP */