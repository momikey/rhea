#ifndef RHEA_NODES_STRUCTURE_HPP
#define RHEA_NODES_STRUCTURE_HPP

#include <memory>
#include <string>
#include <fmt/format.h>

#include "node_base.hpp"
#include "identifiers.hpp"
#include "literals.hpp"
#include "typenames.hpp"
#include "../../util/compat.hpp"

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
        Container() = default;
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

    // Dictionary entry AST. This pretty much requires a variant, because
    // it can be an integer or string literal *or* a symbol.
    using DictionaryKey = util::variant<
        std::unique_ptr<Integer>,
        std::unique_ptr<Byte>,
        std::unique_ptr<Long>,
        std::unique_ptr<UnsignedInteger>,
        std::unique_ptr<UnsignedByte>,
        std::unique_ptr<UnsignedLong>,
        std::unique_ptr<String>,
        std::unique_ptr<Symbol>
    >;
    
    class DictionaryEntry : public ASTNode
    {
        public:
        template <typename Key>
        DictionaryEntry(Key k, expression_ptr e) : key(std::move(k)), value(std::move(e)) {}

        DictionaryKey key;
        expression_ptr value;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };

    // Dictionary AST. This uses the entry class above.
    class Dictionary : public Expression
    {
        public:
        Dictionary(child_vector<DictionaryEntry>& es);

        child_vector<DictionaryEntry> items;

        util::any visit(visitor::Visitor* v) override;
        std::string to_string() override;
    };

    // Structure declaration AST. This uses the TypePair node, defined
    // in the "typenames" header, to map fields to their desired types.
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