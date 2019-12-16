#ifndef RHEA_AST_BUILDER_INTERNAL_HPP
#define RHEA_AST_BUILDER_INTERNAL_HPP

#include <string>

#include "../../grammar.hpp"
#include "../nodes.hpp"
#include "../error.hpp"
#include "../parse_tree_node.hpp"

/*
 * Internal detail for the AST builder.
 */
namespace rhea { namespace ast {
    namespace internal {        
        // Builder for identifiers, for when a more general expression can't be used.
        std::unique_ptr<AnyIdentifier> create_identifier_node(parser_node* node);

        // Builder for expressions.
        expression_ptr create_expression_node(parser_node* node);

        // Builder for statements.
        statement_ptr create_statement_node(parser_node* node);

    }
}}

#endif /* RHEA_AST_BUILDER_INTERNAL_HPP */