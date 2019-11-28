#ifndef RHEA_AST_PARSE_TREE_NODE_HPP
#define RHEA_AST_PARSE_TREE_NODE_HPP

#include <memory>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "nodes/node_base.hpp"

/*
 * Here we derive a custom node class for the PEGTL parse tree.
 * This allows us a little more flexibility when we go to annotate
 * the generated AST.
 * 
 * At the moment, this only adds an owning pointer to an annotation
 * object, but we may expand it later.
 */

namespace rhea { namespace ast {

    struct parser_node : tao::pegtl::parse_tree::basic_node<parser_node>
    {
        std::unique_ptr<ASTNode> annotation;
    };
}}

#endif /* RHEA_AST_PARSE_TREE_NODE_HPP */