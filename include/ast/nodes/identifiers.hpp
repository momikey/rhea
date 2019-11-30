#ifndef RHEA_NODES_IDENTIFIER_HPP
#define RHEA_NODES_IDENTIFIER_HPP

#include <memory>
#include <string>
#include <vector>
#include <fmt/format.h>

#include "node_base.hpp"

/*
 * AST node classes for identifier types.
 */

namespace rhea { namespace ast {
    // The base for all identifier node types
    class AnyIdentifier : public Expression
    {};

    // A basic identifier is just a string of alphanumeric characters
    // or _, starting with a letter. All the node needs to store is the name;
    // symbol table lookup, etc., can come during analysis and codegen.
    class Identifier : public AnyIdentifier
    {
        public:
        Identifier(std::string n): name(n) {}

        const std::string name;

        std::string to_string() override
            { return fmt::format("(Identifier, {0})", name); }
    };

    // A fully-qualified identifier is used for accessing identifiers
    // in modules. For it, we need to store a list of basic identifiers.
    class FullyQualified : public AnyIdentifier
    {
        public:
        FullyQualified(std::vector<std::string>& ns);
        FullyQualified(child_vector<Identifier>& ids);

        child_vector<Identifier> const& children() const { return m_children; }

        std::string to_string() override;

        private:
        child_vector<Identifier> m_children;
    };

    // A relative identifier is a qualified identifier relative to the
    // current module.
    class RelativeIdentifier : public AnyIdentifier
    {
        public:
        RelativeIdentifier(std::unique_ptr<Identifier>& id): identifier(std::move(id)) {}
        RelativeIdentifier(std::unique_ptr<FullyQualified>& id): identifier(std::move(id)) {}
        // RelativeIdentifier(RelativeIdentifier&& rhs) noexcept : identifier(std::move(rhs.identifier)) {}

        const std::unique_ptr<AnyIdentifier> identifier;

        std::string to_string() override
            { return fmt::format("(RelativeIdentifier,{0})", identifier->to_string()); }
    };
}}

#endif /* RHEA_NODES_IDENTIFIER_HPP */