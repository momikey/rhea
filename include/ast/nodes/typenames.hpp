#ifndef RHEA_NODES_TYPENAMES_HPP
#define RHEA_NODES_TYPENAMES_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"
#include "identifiers.hpp"
#include "literals.hpp"

/*
 * AST nodes for type name.
 */
namespace rhea { namespace ast {
    // Base class for type names, forward declaration
    class Typename;

    // The generic part of a type name can contain multiple types,
    // but it is itself *not* a type name.
    class GenericTypename : public ASTNode
    {
        public:
        GenericTypename(child_vector<Typename>& c);

        std::string to_string() override;

        private:
        child_vector<Typename> m_children;
    };

    // Base class for type names
    class Typename : public ASTNode
    {
        public:
        // Basic types will use this constructor
        Typename(std::unique_ptr<AnyIdentifier> n): name(std::move(n)) {}

        // More complex types will use one of these
        Typename(std::unique_ptr<AnyIdentifier> n, std::unique_ptr<GenericTypename> g)
            : name(std::move(n)), generic_part(std::move(g)), array_part(nullptr) {}
        Typename(std::unique_ptr<AnyIdentifier> n, expression_ptr a)
            : name(std::move(n)), generic_part(nullptr), array_part(std::move(a)) {}
        Typename(std::unique_ptr<AnyIdentifier> n, std::unique_ptr<GenericTypename> g, expression_ptr a)
            : name(std::move(n)), generic_part(std::move(g)), array_part(std::move(a)) {}

        Typename() = default;

        const std::unique_ptr<AnyIdentifier> name;

        // These are optional; if not present (as for a simple type),
        // they will be null.
        const std::unique_ptr<GenericTypename> generic_part;
        const expression_ptr array_part;

        std::string to_string() override
            { return fmt::format("(Typename,{0},{1},{2})",
                name->to_string(),
                generic_part != nullptr ? generic_part->to_string() : "null",
                array_part != nullptr ? array_part->to_string() : "null"
            ); }
    };

    // Class for variant types. These can take a number of different typenames,
    // so we have to store all of those.
    class Variant : public Typename
    {
        public:
        Variant(child_vector<Typename>& ts);

        std::string to_string() override;

        private:
        child_vector<Typename> m_children;
    };

    // An optional has only one possible type (besides nothing, which is implied).
    class Optional : public Typename
    {
        public:
        Optional(std::unique_ptr<Typename> t): type(std::move(t)) {}

        const std::unique_ptr<Typename> type;

        std::string to_string() override
            { return fmt::format("(Optional,{0})", type->to_string()); }
    };

    // The typecasting operator `as`, though a binary operator in form,
    // fits better here, as it directly deals with types instead of only
    // values.
    class Cast : public Expression
    {
        public:
        Cast(expression_ptr l, std::unique_ptr<Typename> r)
            : left(std::move(l)), right(std::move(r)) {}

        const expression_ptr left;
        const std::unique_ptr<Typename> right;

        std::string to_string() override
            { return fmt::format("(Cast,{0},{1})", left->to_string(), right->to_string()); }
    };

    // For the type *check* operator `is`, the same logic applies as above.
    class TypeCheck : public Expression
    {
        public:
        TypeCheck(expression_ptr l, std::unique_ptr<Typename> r)
            : left(std::move(l)), right(std::move(r)) {}

        const expression_ptr left;
        const std::unique_ptr<Typename> right;

        std::string to_string() override
            { return fmt::format("(TypeCheck,{0},{1})", left->to_string(), right->to_string()); }
    };

    // A type alias is nothing more than renaming a type, the same as
    // C++ `using` or C `typedef`. It generates no code, but affects
    // the symbol table of its scope.
    class Alias : public Statement
    {
        public:
        Alias(std::unique_ptr<Identifier> a, std::unique_ptr<AnyIdentifier> o)
            : alias(std::move(a)), original(std::move(o)) {}

        const std::unique_ptr<Identifier> alias;
        const std::unique_ptr<AnyIdentifier> original;

        std::string to_string() override
            { return fmt::format("(Alias,{0},{1})", alias->to_string(), original->to_string()); }
    };

    // Symbol lists are, at present, only used in enum declarations.
    // In the future, we might want to do more with them. For now,
    // they're just lists of identifiers, which we can convert into
    // symbols without having to go through Identifier nodes.
    class SymbolList : public Expression
    {
        public:
        SymbolList(std::vector<std::string>& ss);

        child_vector<Symbol> symbols;

        std::string to_string() override;
    };

    // A Rhea enum is just an alias given to a symbol list. Unlike C/C++,
    // Rhea programmers shouldn't care about the specific values. Thus,
    // we can simplify the AST by just tracking symbols.
    class Enum : public Statement
    {
        public:
        Enum(std::unique_ptr<Identifier> n, std::unique_ptr<SymbolList> ss)
            : name(std::move(n)), values(std::move(ss)) {}

        const std::unique_ptr<Identifier> name;
        const std::unique_ptr<SymbolList> values;

        std::string to_string() override
            { return fmt::format("(Enum,{0},{1})", name->to_string(), values->to_string()); }
    };

    // In many cases, we need a mapping between a local identifier
    // (or just a string) and a typename.
    class TypePair : public ASTNode
    {
        public:
        TypePair(std::string n, std::unique_ptr<Typename> v)
            : name(n), value(std::move(v)) {}
        
        // We store the name as just a string rather than an
        // Identifier AST node because we don't actually want
        // the extra functionality of the node class. This class
        // just creates a mapping, not any actual code.
        std::string name;
        std::unique_ptr<Typename> value;

        std::string to_string() override
            { return fmt::format("(TypePair,{0},{1})", name, value->to_string()); }
    };

    // Structure declaration AST. This uses the TypePair node, defined
    // above, to map fields to their desired types.
    class Structure : public Statement
    {
        public:
        Structure(std::unique_ptr<Identifier> n, child_vector<TypePair>& fs);

        const std::unique_ptr<Identifier> name;
        child_vector<TypePair> fields;

        std::string to_string() override;
    };
}}

#endif /* RHEA_NODES_TYPENAMES_HPP */