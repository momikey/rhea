#ifndef RHEA_AST_NODE_BASE_HPP
#define RHEA_AST_NODE_BASE_HPP

#include <string>
#include <vector>
#include <memory>

#include <tao/pegtl.hpp>

#include "../parse_tree_node.hpp"

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

        const tao::pegtl::position position;
    };

    // "Top-level" node types
    class Expression : public ASTNode
    {

    };

    class Statement : public ASTNode
    {

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
}}

#endif /* RHEA_AST_NODE_BASE_HPP */