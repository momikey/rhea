#ifndef RHEA_NODES_IDENTIFIER_HPP
#define RHEA_NODES_IDENTIFIER_HPP

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <fmt/format.h>

#include "node_base.hpp"

/*
 * AST node classes for identifier types.
 */

namespace rhea { namespace ast {
    // The base for all identifier node types
    class AnyIdentifier : public Expression
    {};

    // Maker function for identifiers.
    template <typename Id, typename... Args>
    std::unique_ptr<AnyIdentifier> make_identifier(Args&&... args)
        { return std::make_unique<Id>(std::forward<Args>(args)...); }

    // A basic identifier is just a string of alphanumeric characters
    // or _, starting with a letter. All the node needs to store is the name;
    // symbol table lookup, etc., can come during analysis and codegen.
    class Identifier : public AnyIdentifier
    {
        public:
        Identifier(std::string n): name(n) {}

        const std::string name;

        util::any visit(visitor::Visitor* v) override;
        types::TypeInfo expression_type() override
            { return m_expression_type; }
        std::string to_string() override
            { return fmt::format("(Identifier,{0})", name); }

        // Variables (and other uses of identifiers) do not have known types at compile time,
        // so we must have some way of identifying them.
        void set_expression_type(types::TypeInfo et)
            { m_expression_type = et; }
        private:
        types::TypeInfo m_expression_type;
    };

    // A fully-qualified identifier is used for accessing identifiers
    // in modules. For it, we need to store a list of basic identifiers.
    class FullyQualified : public AnyIdentifier
    {
        public:
        FullyQualified(std::vector<std::string>& ns);
        FullyQualified(child_vector<Identifier>& ids);

        child_vector<Identifier> children;

        util::any visit(visitor::Visitor* v) override;
        types::TypeInfo expression_type() override
            { return m_expression_type; }
        std::string to_string() override;

        // Variables (and other uses of identifiers) do not have known types at compile time,
        // so we must have some way of identifying them.
        void set_expression_type(types::TypeInfo et)
            { m_expression_type = et; }
        private:
        types::TypeInfo m_expression_type;
    };

    // A relative identifier is a qualified identifier relative to the
    // current module.
    class RelativeIdentifier : public AnyIdentifier
    {
        public:
        RelativeIdentifier(std::unique_ptr<Identifier> id): identifier(std::move(id)) {}
        RelativeIdentifier(std::unique_ptr<FullyQualified> id): identifier(std::move(id)) {}

        const std::unique_ptr<AnyIdentifier> identifier;

        util::any visit(visitor::Visitor* v) override;
        types::TypeInfo expression_type() override
            { return m_expression_type; }
        std::string to_string() override
            { return fmt::format("(RelativeIdentifier,{0})", identifier->to_string()); }
            
        // Variables (and other uses of identifiers) do not have known types at compile time,
        // so we must have some way of identifying them.
        void set_expression_type(types::TypeInfo et)
            { m_expression_type = et; }
        private:
        types::TypeInfo m_expression_type;
    };
}}

#endif /* RHEA_NODES_IDENTIFIER_HPP */