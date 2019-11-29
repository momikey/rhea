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
 * At the moment, we don't do anything with it, but we can add in
 * extra information later. Note that we'll have to adapt the AST
 * builder to handle any additional information.
 */

namespace rhea { namespace ast {

    struct parser_node : tao::pegtl::parse_tree::basic_node<parser_node>
    {
        
    };
}}

#endif /* RHEA_AST_PARSE_TREE_NODE_HPP */