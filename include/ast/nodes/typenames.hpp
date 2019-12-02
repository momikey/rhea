#ifndef RHEA_NODES_TYPENAMES_HPP
#define RHEA_NODES_TYPENAMES_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"
#include "identifiers.hpp"

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
        Typename(std::unique_ptr<AnyIdentifier>& n): name(std::move(n)) {}

        // More complex types will use one of these
        Typename(std::unique_ptr<AnyIdentifier>& n, std::unique_ptr<GenericTypename>& g)
            : name(std::move(n)), generic_part(std::move(g)), array_part(nullptr) {}
        Typename(std::unique_ptr<AnyIdentifier>& n, std::unique_ptr<Expression>& a)
            : name(std::move(n)), generic_part(nullptr), array_part(std::move(a)) {}
        Typename(std::unique_ptr<AnyIdentifier>& n, std::unique_ptr<GenericTypename>& g, std::unique_ptr<Expression>& a)
            : name(std::move(n)), generic_part(std::move(g)), array_part(std::move(a)) {}

        Typename() = default;

        const std::unique_ptr<AnyIdentifier> name;

        // These are optional; if not present (as for a simple type),
        // they will be null.
        const std::unique_ptr<GenericTypename> generic_part;
        const std::unique_ptr<Expression> array_part;

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
        Optional(std::unique_ptr<Typename>& t): type(std::move(t)) {}

        const std::unique_ptr<Typename> type;

        std::string to_string() override
            { return fmt::format("(Optional,{0})", type->to_string()); }
    };
}}

#endif /* RHEA_NODES_TYPENAMES_HPP */