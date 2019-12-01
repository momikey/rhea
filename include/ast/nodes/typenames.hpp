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

        // More complex types will use this one
        Typename(std::unique_ptr<AnyIdentifier>& n, std::unique_ptr<GenericTypename>& g, std::unique_ptr<Expression>& a)
            : name(std::move(n)), generic_part(std::move(g)), array_part(std::move(a)) {}

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
}}

#endif /* RHEA_NODES_TYPENAMES_HPP */