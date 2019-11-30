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
    // A basic identifier is just a string of alphanumeric characters
    // or _, starting with a letter. All the node needs to store is the name;
    // symbol table lookup, etc., can come during analysis and codegen.
    class Identifier : public ASTNode
    {
        public:
        Identifier(std::string n): name(n) {}

        const std::string name;

        std::string to_string() override
            { return fmt::format("(Identifier, {0})", name); }
    };

    // A fully-qualified identifier is used for accessing identifiers
    // in modules. For it, we need to store a list of basic identifiers.
    class FullyQualified : public ASTNode
    {
        public:
        FullyQualified(std::vector<std::string>& ns);
        FullyQualified(child_vector<Identifier>&& ids): m_children(std::move(ids)) {}

        child_vector<Identifier> const& children() const { return m_children; }

        std::string to_string() override
            // TODO
            { return fmt::format("(FullyQualified)"); }

        private:
        child_vector<Identifier> m_children;
    };
}}

#endif /* RHEA_NODES_IDENTIFIER_HPP */