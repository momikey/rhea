#ifndef RHEA_NODES_STRUCTURE_HPP
#define RHEA_NODES_STRUCTURE_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"
#include "identifiers.hpp"
#include "literals.hpp"
#include "typenames.hpp"

/*
 * AST nodes for types representing data structures. In Rhea,
 * this includes structures (of course), arrays, lists, tuples, and
 * dictionaries. Symbol lists are *not* included, nor are enums.
 * These can be found in the `typenames` header.
 */
namespace rhea { namespace ast {
    // Arrays, lists, and tuples all follow the same pattern,
    // but they have different requirements for typing and codegen.
    class Container : public Expression
    {
        public:
        Container(child_vector<Expression>& es);

        child_vector<Expression> items;

        protected:
        // This is *not* an override, but the individual subtypes
        // will each call it to build their own `to_string` methods.
        std::string to_string_base(std::string node_type);
    };

    class Array : public Container
    {
        public:
        Array(child_vector<Expression>& es) : Container(es) {}

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override { return to_string_base("Array"); }
    };

    class List : public Container
    {
        public:
        List(child_vector<Expression>& es) : Container(es) {}

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override { return to_string_base("List"); }
    };

    class Tuple : public Container
    {
        public:
        Tuple(child_vector<Expression>& es) : Container(es) {}

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override { return to_string_base("Tuple"); }
    };

    // Structure declaration AST. This uses the TypePair node, defined
    // above, to map fields to their desired types.
    class Structure : public Statement
    {
        public:
        Structure(std::unique_ptr<Identifier> n, child_vector<TypePair>& fs);

        const std::unique_ptr<Identifier> name;
        child_vector<TypePair> fields;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };
}}

#endif /* RHEA_NODES_STRUCTURE_HPP */