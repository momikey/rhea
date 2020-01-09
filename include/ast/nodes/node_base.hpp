#ifndef RHEA_AST_NODE_BASE_HPP
#define RHEA_AST_NODE_BASE_HPP

#include <string>
#include <vector>
#include <memory>
#include <utility>

#include <tao/pegtl.hpp>

#include "../parse_tree_node.hpp"
#include "../../types/types.hpp"
#include "../../util/compat.hpp"
#include "../../visitor/visitor_fwd.hpp"

/*
 * The base node for the annotated AST. This is different from
 * the AST skeleton generated by the parser, but it follows the
 * structure of that tree, and each node in the annotated AST
 * will contain an observing pointer to the corresponding node
 * in the generated AST.
 * 
 * Note that this node and its children are *classes* rather than
 * structs. This, IMO, better communicates their status as a
 * polymorphic hierarchy, since the generated AST will store pointers
 * to this base class and access the children through its interface.
 * 
 * Although I haven't decided on code style guidelines for Rhea,
 * I feel that the annotated AST classes should be capitalized. Again,
 * this is for clarity's sake: they're not the same as, e.g., grammar
 * rules.
 */

namespace rhea { namespace ast {
    // The base type for all AST nodes
    class ASTNode
    {
        public:
        ASTNode(): position({}, "") {}
        virtual ~ASTNode() {}

        virtual std::string to_string() = 0;
        virtual util::any visit(visitor::Visitor* v);

        tao::pegtl::position position;
    };

    // "Top-level" node types
    class Expression : public ASTNode
    {
        public:
        // All expressions have a type. This may be fixed or calculated,
        // so we make it a method.
        virtual types::TypeInfo expression_type() { return types::UnknownType{}; }

        util::any visit(visitor::Visitor* v) override;
    };

    class Statement : public ASTNode
    {
        public:
        util::any visit(visitor::Visitor* v) override;
        // TODO: Some statements can declare symbols. If we make a quick
        // pass through some levels of the AST, we can pick these out,
        // which saves us from having to forward declare as in C/C++.
        // virtual <SomeScopeType>& declare(<SomeScopeType>& scope) = 0;
    };

    // A lot of nodes need to store lists of children. These will
    // always be unique_ptrs to AST nodes, but we may want to restrict
    // that to a specific subtype, so we use a template alias here.
    template <typename T>
    using child_vector = std::vector<std::unique_ptr<T>>;

    // std::unique_ptrs to the main node types. We use these a lot,
    // so it helps to have aliases.
    using node_ptr = std::unique_ptr<ASTNode>;
    using expression_ptr = std::unique_ptr<Expression>;
    using statement_ptr = std::unique_ptr<Statement>;

    // Maker functions, in the vein of make_unique.
    template <typename Expr, typename... Args>
    inline expression_ptr make_expression(Args&&... args)
        { return std::make_unique<Expr>(std::forward<Args>(args)...); }

    template <typename Stmt, typename... Args>
    inline statement_ptr make_statement(Args&&... args)
        { return std::make_unique<Stmt>(std::forward<Args>(args)...); }
}}

#endif /* RHEA_AST_NODE_BASE_HPP */